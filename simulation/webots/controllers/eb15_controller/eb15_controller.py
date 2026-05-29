#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
=============================================================================
             EB-15 Robotic Arm — High-Fidelity Webots Controller
=============================================================================
Fase 8 Parte 2: Validação Algorítmica e Supervisão de Comandos (Mestre em Python)
Responsabilidade: Controla a simulação 3D no Webots, replicando fielmente as
                  leis de controle do ESP32-S3 e Arduino Uno, provendo
                  servidores TCP RTDE e HTTP Dashboard.
=============================================================================
"""

import sys
import os

try:
    # Redirect stdout and stderr immediately to catch startup/import errors
    log_path = r"c:\Users\ferna\OneDrive\Área de Trabalho\TCC_Fernando\scratch\controller_debug.log"
    sys.stdout = open(log_path, "w", encoding="utf-8", buffering=1)
    sys.stderr = sys.stdout
except Exception:
    pass

import time
import math
import struct
import socket
import threading
import http.server
from urllib.parse import urlparse
import json

# Importação da API do Webots
from controller import Robot, Motor, PositionSensor

# ============================================================================
# 1. PARÂMETROS GEOMÉTRICOS E MECÂNICOS DO ROBÔ (Conforme config.h)
# ============================================================================
L1 = 150.0  # Base ao Ombro (mm)
L2 = 200.0  # Ombro ao Cotovelo (mm)
L3 = 200.0  # Cotovelo ao Punho (mm)
L6 = 80.0   # Centro do punho ao TCP (mm)

LIMIT_MIN_DEG = [-170.0, -45.0, -120.0, -180.0, -90.0, -360.0]
LIMIT_MAX_DEG = [170.0, 180.0, 120.0, 180.0, 90.0, 360.0]
MAX_SPEED_DEG_S = [45.0, 45.0, 45.0, 90.0, 90.0, 90.0]

# ============================================================================
# 2. VARIÁVEIS GLOBAIS DE ESTADO E LOCKS DE THREAD
# ============================================================================
state_lock = threading.Lock()

# Estados reais em graus e pose cartesiana mm/graus
g_joint_angles = [0.0] * 6
g_pid_last_err = [0.0] * 6
g_current_pose = {
    "x": 0.0, "y": 0.0, "z": 0.0,
    "rx": 0.0, "ry": 0.0, "rz": 0.0
}

# Fila de trajetórias (fatias angulares em graus, taxa de 50 ms)
trajectory_queue = []

# ============================================================================
# 3. KINEMATICS ENGINE (PURA MATEMÁTICA SEM DEPENDÊNCIAS DE TERCEIROS)
# ============================================================================
def get_dh_matrix(theta, d, a, alpha):
    c_t = math.cos(theta)
    s_t = math.sin(theta)
    c_a = math.cos(alpha)
    s_a = math.sin(alpha)
    return [
        [c_t, -s_t * c_a,  s_t * s_a, a * c_t],
        [s_t,  c_t * c_a, -c_t * s_a, a * s_t],
        [0.0,  s_a,        c_a,       d      ],
        [0.0,  0.0,        0.0,       1.0    ]
    ]

def multiply_matrices(A, B):
    result = [[0.0]*4 for _ in range(4)]
    for i in range(4):
        for j in range(4):
            for k in range(4):
                result[i][j] += A[i][k] * B[k][j]
    return result

def forward_kinematics(q):
    dh_params = [
        [L1,   0.0,  math.pi/2, 0.0          ], # J1
        [0.0,  L2,   0.0,      -math.pi/2   ], # J2
        [0.0,  L3,   0.0,       0.0          ], # J3
        [0.0,  0.0, -math.pi/2, -math.pi/2   ], # J4
        [0.0,  0.0,  math.pi/2, 0.0          ], # J5
        [L6,   0.0,  0.0,       0.0          ]  # J6
    ]
    T = [[1.0 if i==j else 0.0 for j in range(4)] for i in range(4)]
    for i in range(6):
        theta = q[i] + dh_params[i][3]
        d = dh_params[i][0]
        a = dh_params[i][1]
        alpha = dh_params[i][2]
        A = get_dh_matrix(theta, d, a, alpha)
        T = multiply_matrices(T, A)
    return T

def matrix_to_euler(R):
    val = -R[2][0]
    val = max(-1.0, min(1.0, val))
    ry = math.asin(val)
    if math.cos(ry) > 0.001:
        rz = math.atan2(R[1][0], R[0][0])
        rx = math.atan2(R[2][1], R[2][2])
    else:
        rz = 0.0
        rx = math.atan2(-R[1][2], R[1][1])
    return math.degrees(rx), math.degrees(ry), math.degrees(rz)

def euler_to_matrix(rx_deg, ry_deg, rz_deg):
    rx = math.radians(rx_deg)
    ry = math.radians(ry_deg)
    rz = math.radians(rz_deg)
    cx, sx = math.cos(rx), math.sin(rx)
    cy, sy = math.cos(ry), math.sin(ry)
    cz, sz = math.cos(rz), math.sin(rz)
    R = [
        [cz * cy, cz * sy * sx - sz * cx, cz * sy * cx + sz * sx],
        [sz * cy, sz * sy * sx + cz * cx, sz * sy * cx - cz * sx],
        [-sy,     cy * sx,               cy * cx              ]
    ]
    return R

def inverse_kinematics(target_pos, R_target):
    z_axis = [R_target[0][2], R_target[1][2], R_target[2][2]]
    wcp = [
        target_pos[0] - L6 * z_axis[0],
        target_pos[1] - L6 * z_axis[1],
        target_pos[2] - L6 * z_axis[2]
    ]
    wx, wy, wz = wcp

    # 1. Base Rotation (J1)
    q1 = math.atan2(wy, wx)

    # 2. Planar 2R shoulder-elbow positioning (J2-J3)
    r = math.sqrt(wx**2 + wy**2)
    z_prime = wz - L1

    X_p = r
    Y_p = -z_prime

    d_sq = X_p**2 + Y_p**2
    d = math.sqrt(d_sq)

    if d > (L2 + L3) or d < abs(L2 - L3):
        raise ValueError("Target coordinate is out of the robot's physical reach!")

    # Cosine Law for J3
    cos_q3 = (d_sq - L2**2 - L3**2) / (2.0 * L2 * L3)
    cos_q3 = max(-1.0, min(1.0, cos_q3))
    q3 = -math.acos(cos_q3)

    # Shoulder Joint (J2)
    A = L2 + L3 * math.cos(q3)
    B = L3 * math.sin(q3)
    
    sin_q2 = (A * X_p - B * Y_p) / d_sq
    cos_q2 = (B * X_p + A * Y_p) / d_sq
    q2 = math.atan2(sin_q2, cos_q2)

    # 3. Spherical Wrist Orientation (J4-J6)
    dh_params = [
        [L1,   0.0,  math.pi/2, 0.0          ],
        [0.0,  L2,   0.0,      -math.pi/2   ],
        [0.0,  L3,   0.0,       0.0          ]
    ]
    T0_3 = [[1.0 if i==j else 0.0 for j in range(4)] for i in range(4)]
    for i in range(3):
        theta = [q1, q2, q3][i] + dh_params[i][3]
        d = dh_params[i][0]
        a = dh_params[i][1]
        alpha = dh_params[i][2]
        A_mat = get_dh_matrix(theta, d, a, alpha)
        T0_3 = multiply_matrices(T0_3, A_mat)
        
    R0_3 = [row[:3] for row in T0_3[:3]]
    R0_3_T = [[R0_3[j][i] for j in range(3)] for i in range(3)]
    
    R3_6 = [[0.0]*3 for _ in range(3)]
    for i in range(3):
        for j in range(3):
            for k in range(3):
                R3_6[i][j] += R0_3_T[i][k] * R_target[k][j]

    # Spherical joint extraction (ZYZ-like configuration)
    cos_q5 = R3_6[2][2]
    cos_q5 = max(-1.0, min(1.0, cos_q5))
    q5 = math.acos(cos_q5)

    if abs(q5) > 0.001:
        q4 = math.atan2(R3_6[0][2], -R3_6[1][2])
        q6 = math.atan2(R3_6[2][1], -R3_6[2][0])
    else:
        q4 = 0.0
        q6 = math.atan2(-R3_6[0][1], R3_6[0][0])

    return [q1, q2, q3, q4, q5, q6]

def check_soft_limits(q_deg):
    for i in range(6):
        if q_deg[i] < LIMIT_MIN_DEG[i] or q_deg[i] > LIMIT_MAX_DEG[i]:
            return False
    return True

# ============================================================================
# 4. S-CURVE MOTION PLANNER (TRAJECTORY GENERATOR)
# ============================================================================
def plan_move_j(target_joints_deg, speed_pct):
    global g_joint_angles, trajectory_queue
    
    with state_lock:
        start_joints = list(g_joint_angles)
    
    max_diff = 0.0
    for i in range(6):
        diff = abs(target_joints_deg[i] - start_joints[i])
        if diff > max_diff:
            max_diff = diff
            
    if max_diff < 0.01:
        return
        
    speed_factor = speed_pct / 100.0
    duration_s = max_diff / (MAX_SPEED_DEG_S[0] * speed_factor)
    if duration_s < 0.5:
        duration_s = 0.5
        
    slices = int(duration_s * 1000.0 / 50.0) # Slices of 50ms (20 Hz planner)
    if slices < 1:
        slices = 1
        
    new_slices = []
    
    for s in range(1, slices + 1):
        tau = s / slices
        # Minimum jerk trajectory profile: 5th-degree polynomial
        factor = 10 * (tau**3) - 15 * (tau**4) + 6 * (tau**5)
        slice_joints = [
            start_joints[i] + (target_joints_deg[i] - start_joints[i]) * factor
            for i in range(6)
        ]
        new_slices.append(slice_joints)
        
    with state_lock:
        trajectory_queue.extend(new_slices)
    print(f"[Planner] Joint motion planned: {slices} slices generated ({duration_s:.2f}s).")

def plan_move_l(target_pose, speed_pct):
    global g_current_pose, g_joint_angles, trajectory_queue
    
    target_pos = [target_pose["x"], target_pose["y"], target_pose["z"]]
    R_target = euler_to_matrix(target_pose["rx"], target_pose["ry"], target_pose["rz"])
    
    try:
        target_joints_rad = inverse_kinematics(target_pos, R_target)
        target_joints_deg = [math.degrees(q) for q in target_joints_rad]
    except Exception as e:
        raise ValueError(f"Unreachable coordinate: {e}")
        
    if not check_soft_limits(target_joints_deg):
        raise ValueError("Joint limits exceeded!")
        
    with state_lock:
        start_pos = [g_current_pose["x"], g_current_pose["y"], g_current_pose["z"]]
        start_rot = [g_current_pose["rx"], g_current_pose["ry"], g_current_pose["rz"]]
    
    diff_pos = math.sqrt(
        (target_pos[0] - start_pos[0])**2 +
        (target_pos[1] - start_pos[1])**2 +
        (target_pos[2] - start_pos[2])**2
    )
    
    if diff_pos < 0.1:
        return
        
    speed_factor = speed_pct / 100.0
    duration_s = diff_pos / (80.0 * speed_factor) # Max linear speed of 80mm/s
    if duration_s < 0.5:
        duration_s = 0.5
        
    slices = int(duration_s * 1000.0 / 50.0)
    if slices < 1:
        slices = 1
        
    new_slices = []
    for s in range(1, slices + 1):
        tau = s / slices
        factor = 10 * (tau**3) - 15 * (tau**4) + 6 * (tau**5)
        
        slice_pos = [
            start_pos[i] + (target_pos[i] - start_pos[i]) * factor
            for i in range(3)
        ]
        slice_rot = [
            start_rot[0] + (target_pose["rx"] - start_rot[0]) * factor,
            start_rot[1] + (target_pose["ry"] - start_rot[1]) * factor,
            start_rot[2] + (target_pose["rz"] - start_rot[2]) * factor
        ]
        
        try:
            R_slice = euler_to_matrix(slice_rot[0], slice_rot[1], slice_rot[2])
            slice_joints_rad = inverse_kinematics(slice_pos, R_slice)
            slice_joints_deg = [math.degrees(q) for q in slice_joints_rad]
            new_slices.append(slice_joints_deg)
        except Exception as e:
            raise ValueError(f"Unreachable coordinate along path at step {s}/{slices}: Pos={[round(p, 2) for p in slice_pos]}, Rot={[round(r, 2) for r in slice_rot]}. Error: {e}")
            
    with state_lock:
        trajectory_queue.extend(new_slices)
    print(f"[Planner] Linear motion planned: {len(new_slices)} slices generated ({duration_s:.2f}s).")

# ============================================================================
# 5. CLOSED-LOOP PID + TANH CONTROLLER
# ============================================================================
class JointPID:
    def __init__(self, kp, kd, gamma, limit_speed):
        self.kp = kp
        self.kd = kd
        self.gamma = gamma
        self.limit_speed = limit_speed # degrees/sec
        self.last_err = 0.0

    def step(self, desired_deg, real_deg, dt):
        error = desired_deg - real_deg
        err_diff = (error - self.last_err) / dt if dt > 0.0 else 0.0
        self.last_err = error
        
        # Hyperbolic tangent modulation to damp vibrations
        tanh_factor = math.tanh(self.gamma * abs(error))
        u_speed_deg_s = self.kp * error * tanh_factor + self.kd * err_diff
        
        # Clamp velocity output to software safety limits
        u_speed_deg_s = max(-self.limit_speed, min(self.limit_speed, u_speed_deg_s))
        return u_speed_deg_s, error

# ============================================================================
# 6. WI-FI & WEB HTTP DASHBOARD SERVER (Lightweight Pure Python Server)
# ============================================================================
INDEX_HTML = """<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>EB-15 Dashboard - Webots Simulation</title>
    <style>
        :root{--bg:#0f0f15;--panel:#151522;--text:#e8e8f0;--muted:#7c7d95;--acc:#6c5ce7;--acc-h:#5b43d6;--dang:#ff7675;--dang-h:#d63031;--bord:#28293d;--succ:#55efc4;}
        *{box-sizing:border-box;margin:0;padding:0;font-family:'Segoe UI',system-ui,sans-serif;}
        body{background:var(--bg);color:var(--text);padding:2rem;}
        header{display:flex;justify-content:space-between;align-items:center;border-bottom:2px solid var(--bord);padding-bottom:1.5rem;margin-bottom:2rem;}
        h1{font-weight:700;letter-spacing:-0.5px;background:linear-gradient(45deg, #a29bfe, #6c5ce7);-webkit-background-clip:text;-webkit-text-fill-color:transparent;}
        .badge{padding:0.5rem 1rem;border-radius:6px;font-weight:600;font-size:0.85rem;background:var(--bord);color:var(--muted);transition:all 0.3s;}
        #conn-state.connected{background:rgba(85,239,196,0.15);color:var(--succ);border:1px solid var(--succ);}
        #conn-state.error{background:rgba(255,118,117,0.15);color:var(--dang);border:1px solid var(--dang);}
        .dashboard{display:grid;grid-template-columns:repeat(auto-fit,minmax(340px,1fr));gap:2rem;}
        .panel{background:var(--panel);border-radius:12px;padding:2rem;box-shadow:0 8px 30px rgba(0,0,0,0.4);border:1px solid var(--bord);display:flex;flex-direction:column;}
        h2{border-bottom:1px solid var(--bord);padding-bottom:0.75rem;margin-bottom:1.5rem;font-size:1.3rem;font-weight:600;}
        h3{color:var(--muted);margin:1.5rem 0 0.5rem;font-size:0.85rem;text-transform:uppercase;letter-spacing:1px;}
        button{background:var(--acc);color:#fff;border:none;padding:0.75rem 1.25rem;border-radius:6px;cursor:pointer;font-weight:600;transition:all 0.2s;}
        button:hover{background:var(--acc-h);transform:translateY(-1px);}
        button.danger{background:var(--dang);} button.danger:hover{background:var(--dang-h);}
        .quick-actions{display:grid;grid-template-columns:1fr 1fr;gap:0.75rem;}
        .jog-row{display:flex;align-items:center;background:rgba(255,255,255,0.03);padding:0.6rem 1rem;border-radius:6px;margin-bottom:0.6rem;border:1px solid rgba(255,255,255,0.05);}
        .jog-row span{width:45px;font-weight:700;color:var(--muted);}
        .jog-row button{flex:1;margin:0 0.3rem;padding:0.4rem;}
        table{width:100%;border-collapse:collapse;margin-top:0.5rem;}
        th,td{padding:0.6rem 0.8rem;text-align:left;border-bottom:1px solid var(--bord);}
        th{color:var(--muted);font-weight:600;font-size:0.85rem;}
        td{font-family:monospace;font-size:1.15rem;}
        .speed-control{display:flex;gap:1rem;margin-top:1.2rem;align-items:center;}
        .speed-control input{flex:1;accent-color:var(--acc);}
        
        /* New Setpoints Styles */
        .tabs { display: flex; border-bottom: 2px solid var(--bord); margin-bottom: 1.5rem; }
        .tab-btn { flex: 1; background: none; border: none; padding: 0.75rem; border-radius: 0; color: var(--muted); border-bottom: 3px solid transparent; font-weight: 600; cursor: pointer; transition: all 0.2s; }
        .tab-btn:hover { background: rgba(255,255,255,0.02); color: var(--text); }
        .tab-btn.active { color: var(--acc); border-bottom: 3px solid var(--acc); }
        .setpoint-grid { display: grid; grid-template-columns: repeat(3, 1fr); gap: 0.75rem; }
        .input-field { display: flex; flex-direction: column; gap: 0.3rem; }
        .input-field label { font-size: 0.8rem; color: var(--muted); font-weight: 600; }
        .input-field input { background: #222230; color: var(--text); border: 1px solid var(--bord); padding: 0.5rem; border-radius: 6px; font-family: monospace; font-size: 1rem; outline: none; transition: border 0.2s; }
        .input-field input:focus { border-color: var(--acc); }
        #pose-check-badge.reachable { background: rgba(85,239,196,0.15); color: var(--succ); border: 1px solid var(--succ); }
        #pose-check-badge.unreachable { background: rgba(255,118,117,0.15); color: var(--dang); border: 1px solid var(--dang); }
    </style>
</head>
<body>
    <header>
        <h1>EB-15 Mechatronic Arm Dashboard</h1>
        <div class="status-indicator">
            <span id="conn-state" class="badge">CONNECTING...</span>
            <span id="robot-state" class="badge">SIMULATOR READY</span>
        </div>
    </header>
    <main class="dashboard">
        <section class="panel controls-panel">
            <h2>Command Center</h2>
            <div class="quick-actions">
                <button id="btn-home">Homing Sequence</button>
                <button id="btn-stop" class="danger">E-STOP</button>
            </div>
            <h3>Joint Jogging</h3>
            <div id="jog-controls"></div>
            <div class="speed-control">
                <label>Speed:</label>
                <input type="range" id="jog-speed" min="1" max="100" value="30">
                <span id="speed-val" style="width:40px;text-align:right;">30%</span>
            </div>
            <div class="speed-control">
                <label>Jog Increment:</label>
                <input type="number" id="jog-step" min="0.1" max="90.0" value="15.0" step="0.5" style="width: 80px; background:#222230; color:#fff; border:1px solid var(--bord); padding:0.4rem; border-radius:6px; font-family:monospace; text-align:center;">
                <span style="color:var(--muted); font-size:0.9rem;">degrees</span>
            </div>
        </section>

        <section class="panel setpoints-panel">
            <h2>Spatial Setpoints</h2>
            <div class="tabs">
                <button id="tab-btn-joint" class="tab-btn active" onclick="switchTab('joint')">Joint Space</button>
                <button id="tab-btn-cart" class="tab-btn" onclick="switchTab('cart')">Cartesian Space</button>
            </div>
            
            <div id="joint-tab-content" class="tab-content">
                <div class="setpoint-grid">
                    <div class="input-field"><label>J1 (°)</label><input type="number" id="inp-j1" value="0.0" step="5"></div>
                    <div class="input-field"><label>J2 (°)</label><input type="number" id="inp-j2" value="0.0" step="5"></div>
                    <div class="input-field"><label>J3 (°)</label><input type="number" id="inp-j3" value="0.0" step="5"></div>
                    <div class="input-field"><label>J4 (°)</label><input type="number" id="inp-j4" value="0.0" step="5"></div>
                    <div class="input-field"><label>J5 (°)</label><input type="number" id="inp-j5" value="0.0" step="5"></div>
                    <div class="input-field"><label>J6 (°)</label><input type="number" id="inp-j6" value="0.0" step="5"></div>
                </div>
                <button onclick="moveJointsSetpoint()" style="width: 100%; margin-top: 1.5rem;">Move to Joint Angles</button>
            </div>
            
            <div id="cart-tab-content" class="tab-content" style="display: none;">
                <div class="setpoint-grid">
                    <div class="input-field"><label>X (mm)</label><input type="number" id="inp-x" value="200.0" step="5" oninput="checkCartesianPose()"></div>
                    <div class="input-field"><label>Y (mm)</label><input type="number" id="inp-y" value="0.0" step="5" oninput="checkCartesianPose()"></div>
                    <div class="input-field"><label>Z (mm)</label><input type="number" id="inp-z" value="150.0" step="5" oninput="checkCartesianPose()"></div>
                    <div class="input-field"><label>Rx (°)</label><input type="number" id="inp-rx" value="0.0" step="5" oninput="checkCartesianPose()"></div>
                    <div class="input-field"><label>Ry (°)</label><input type="number" id="inp-ry" value="0.0" step="5" oninput="checkCartesianPose()"></div>
                    <div class="input-field"><label>Rz (°)</label><input type="number" id="inp-rz" value="0.0" step="5" oninput="checkCartesianPose()"></div>
                </div>
                <div style="margin-top: 1.5rem; display: flex; justify-content: space-between; align-items: center; gap:1rem;">
                    <span id="pose-check-badge" class="badge reachable">REACHABLE</span>
                    <button id="btn-move-cart" onclick="moveCartesianSetpoint()" style="flex:1;">Move to Position</button>
                </div>
            </div>
        </section>

        <section class="panel telemetry-panel">
            <h2>High-Fidelity Telemetry</h2>
            <div>
                <h3>Joint Angles (deg)</h3>
                <table>
                    <tr><th>Joint</th><th>Angle</th><th>Joint</th><th>Angle</th></tr>
                    <tr><td>J1</td><td id="t-j1">0.0</td><td>J4</td><td id="t-j4">0.0</td></tr>
                    <tr><td>J2</td><td id="t-j2">0.0</td><td>J5</td><td id="t-j5">0.0</td></tr>
                    <tr><td>J3</td><td id="t-j3">0.0</td><td>J6</td><td id="t-j6">0.0</td></tr>
                </table>
            </div>
            <div>
                <h3>Cartesian TCP Pose (mm/deg)</h3>
                <table>
                    <tr><th>Axis</th><th>Position</th><th>Orientation</th><th>Angle</th></tr>
                    <tr><th>X</th><td id="t-x">0.0</td><th>Rx</th><td id="t-rx">0.0</td></tr>
                    <tr><th>Y</th><td id="t-y">0.0</td><th>Ry</th><td id="t-ry">0.0</td></tr>
                    <tr><th>Z</th><td id="t-z">0.0</td><th>Rz</th><td id="t-rz">0.0</td></tr>
                </table>
            </div>
        </section>
    </main>
    <script>
        const $=(id)=>document.getElementById(id);
        async function api(ep,m='GET',b=null){
          try{
            const opt={method:m,headers:{}};
            if(b){opt.headers['Content-Type']='application/json';opt.body=JSON.stringify(b);}
            const r=await fetch(ep,opt); const d=await r.json();
            $('conn-state').className='badge connected'; $('conn-state').innerText='CONNECTED'; return d;
          }catch(e){
            $('conn-state').className='badge error'; $('conn-state').innerText='OFFLINE'; return null;
          }
        }
        function setup(){
          let jg=$('jog-controls');
          for(let i=1;i<=6;i++){
            jg.innerHTML+=`<div class="jog-row">
                <span>J${i}</span>
                <button onmousedown="jog(${i},-1)" onmouseup="jogStop()">-</button>
                <button onmousedown="jog(${i},1)" onmouseup="jogStop()">+</button>
            </div>`;
          }
          $('btn-home').onclick=()=>api('/api/home','POST');
          $('btn-stop').onclick=()=>api('/api/stop','POST');
          $('jog-speed').oninput=(e)=>$('speed-val').innerText=e.target.value+'%';
        }
        async function jog(joint, dir) {
          let step = parseFloat($('jog-step').value || 15.0);
          let spd = parseFloat($('jog-speed').value);
          let joints = await api('/api/joints');
          if (!joints) return;
          let current = joints.joints[`j${joint}`].deg;
          let target_ang = current + dir * step;
          let payload = { target: {} };
          for(let i=1; i<=6; i++) {
              payload.target[`j${i}`] = i === joint ? target_ang : joints.joints[`j${i}`].deg;
          }
          payload.speed_pct = spd;
          api('/api/move/joints','POST', payload);
        }
        function jogStop() {
        }
        function switchTab(type) {
            if (type === 'joint') {
                $('joint-tab-content').style.display = 'block';
                $('cart-tab-content').style.display = 'none';
                $('tab-btn-joint').classList.add('active');
                $('tab-btn-cart').classList.remove('active');
            } else {
                $('joint-tab-content').style.display = 'none';
                $('cart-tab-content').style.display = 'block';
                $('tab-btn-joint').classList.remove('active');
                $('tab-btn-cart').classList.add('active');
                checkCartesianPose();
            }
        }
        async function moveJointsSetpoint() {
            let target = {};
            for(let i=1; i<=6; i++) {
                target[`j${i}`] = parseFloat($(`inp-j${i}`).value || 0.0);
            }
            let spd = parseFloat($('jog-speed').value);
            let res = await api('/api/move/joints', 'POST', { target: target, speed_pct: spd });
            if (res && res.status === "error") {
                alert("Move Error: " + res.message);
            }
        }
        async function checkCartesianPose() {
            let target = {
                x: parseFloat($('inp-x').value || 0),
                y: parseFloat($('inp-y').value || 0),
                z: parseFloat($('inp-z').value || 0),
                rx: parseFloat($('inp-rx').value || 0),
                ry: parseFloat($('inp-ry').value || 0),
                rz: parseFloat($('inp-rz').value || 0)
            };
            let res = await api('/api/check/pose', 'POST', { target: target });
            let badge = $('pose-check-badge');
            if (res) {
                if (res.reachable) {
                    badge.className = 'badge reachable';
                    badge.innerText = 'REACHABLE';
                    $('btn-move-cart').disabled = false;
                } else {
                    badge.className = 'badge unreachable';
                    badge.innerText = 'OUT OF WORKSPACE';
                    $('btn-move-cart').disabled = true;
                }
            }
        }
        async function moveCartesianSetpoint() {
            let target = {
                x: parseFloat($('inp-x').value || 0),
                y: parseFloat($('inp-y').value || 0),
                z: parseFloat($('inp-z').value || 0),
                rx: parseFloat($('inp-rx').value || 0),
                ry: parseFloat($('inp-ry').value || 0),
                rz: parseFloat($('inp-rz').value || 0)
            };
            let spd = parseFloat($('jog-speed').value);
            let res = await api('/api/move/linear', 'POST', { target: target, speed_pct: spd });
            if (res && res.status === "error") {
                alert("Move Error: " + res.message);
            }
        }
        setup();
        setInterval(async()=>{
          let s=await api('/api/status'); 
          if(s) {
              $('robot-state').innerText=s.state;
              $('robot-state').className='badge connected';
          }
          let j=await api('/api/joints'); 
          if(j&&j.joints) {
              for(let i=1;i<=6;i++) {
                  if($(`t-j${i}`)) $(`t-j${i}`).innerText=j.joints[`j${i}`].deg.toFixed(2);
              }
          }
          let p=await api('/api/pose'); 
          if(p&&p.pose) {
              ['x','y','z','rx','ry','rz'].forEach(a=>$('t-'+a).innerText=p.pose[a].toFixed(1));
          }
        }, 150);
    </script>
</body>
</html>
"""

class RobotStateHandler(http.server.BaseHTTPRequestHandler):
    def log_message(self, format, *args):
        pass # Suppress standard print logs to prevent clogging simulation logs
        
    def address_string(self):
        return self.client_address[0] # Prevent reverse DNS lookups that hang offline environments
        
    def do_OPTIONS(self):
        self.send_response(204)
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        self.end_headers()

    def do_GET(self):
        parsed_path = urlparse(self.path)
        path = parsed_path.path
        
        if path == '/':
            self.send_response(200)
            self.send_header('Content-type', 'text/html; charset=utf-8')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.end_headers()
            self.wfile.write(INDEX_HTML.encode('utf-8'))
            
        elif path == '/api/status':
            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.end_headers()
            with state_lock:
                status = {
                    "state": "READY" if len(trajectory_queue) == 0 else "MOVING",
                    "fault": 0,
                    "arduino_connected": True
                }
            self.wfile.write(json.dumps(status).encode('utf-8'))
            
        elif path == '/api/joints':
            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.end_headers()
            with state_lock:
                joints_data = {
                    "joints": {
                        f"j{i+1}": {"deg": float(g_joint_angles[i]), "homed": True}
                        for i in range(6)
                    }
                }
            self.wfile.write(json.dumps(joints_data).encode('utf-8'))
            
        elif path == '/api/pose':
            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.end_headers()
            with state_lock:
                pose_data = {
                    "pose": {
                        "x": float(g_current_pose["x"]),
                        "y": float(g_current_pose["y"]),
                        "z": float(g_current_pose["z"]),
                        "rx": float(g_current_pose["rx"]),
                        "ry": float(g_current_pose["ry"]),
                        "rz": float(g_current_pose["rz"])
                    }
                }
            self.wfile.write(json.dumps(pose_data).encode('utf-8'))
            
        else:
            self.send_response(404)
            self.end_headers()
            self.wfile.write(b"Not Found")

    def do_POST(self):
        parsed_path = urlparse(self.path)
        path = parsed_path.path
        
        content_length = int(self.headers.get('Content-Length', 0))
        post_data = self.rfile.read(content_length) if content_length > 0 else b""
        
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.send_header('Access-Control-Allow-Origin', '*')
        self.end_headers()
        
        if path == '/api/home':
            with state_lock:
                trajectory_queue.clear()
            plan_move_j([0.0]*6, 30.0)
            self.wfile.write(json.dumps({"status": "ok"}).encode('utf-8'))
            
        elif path == '/api/stop':
            with state_lock:
                trajectory_queue.clear()
            self.wfile.write(json.dumps({"status": "ok"}).encode('utf-8'))
            
        elif path == '/api/move/joints':
            try:
                data = json.loads(post_data.decode('utf-8'))
                target = data.get("target", {})
                speed_pct = float(data.get("speed_pct", 30.0))
                
                with state_lock:
                    target_joints = [
                        float(target.get(f"j{i+1}", g_joint_angles[i]))
                        for i in range(6)
                    ]
                plan_move_j(target_joints, speed_pct)
                self.wfile.write(json.dumps({"status": "ok"}).encode('utf-8'))
            except Exception as e:
                self.wfile.write(json.dumps({"status": "error", "message": str(e)}).encode('utf-8'))
                
        elif path == '/api/move/linear':
            try:
                data = json.loads(post_data.decode('utf-8'))
                target = data.get("target", {})
                speed_pct = float(data.get("speed_pct", 30.0))
                
                with state_lock:
                    target_pose = {
                        "x": float(target.get("x", g_current_pose["x"])),
                        "y": float(target.get("y", g_current_pose["y"])),
                        "z": float(target.get("z", g_current_pose["z"])),
                        "rx": float(target.get("rx", g_current_pose["rx"])),
                        "ry": float(target.get("ry", g_current_pose["ry"])),
                        "rz": float(target.get("rz", g_current_pose["rz"]))
                    }
                plan_move_l(target_pose, speed_pct)
                self.wfile.write(json.dumps({"status": "ok"}).encode('utf-8'))
            except Exception as e:
                self.wfile.write(json.dumps({"status": "error", "message": str(e)}).encode('utf-8'))
                
        elif path == '/api/check/pose':
            try:
                data = json.loads(post_data.decode('utf-8'))
                target = data.get("target", {})
                
                with state_lock:
                    curr_pose = dict(g_current_pose)
                
                target_pos = [float(target.get("x", curr_pose["x"])),
                              float(target.get("y", curr_pose["y"])),
                              float(target.get("z", curr_pose["z"]))]
                R_target = euler_to_matrix(float(target.get("rx", curr_pose["rx"])),
                                           float(target.get("ry", curr_pose["ry"])),
                                           float(target.get("rz", curr_pose["rz"])))
                
                try:
                    target_joints_rad = inverse_kinematics(target_pos, R_target)
                    target_joints_deg = [math.degrees(q) for q in target_joints_rad]
                    reachable = check_soft_limits(target_joints_deg)
                    msg = "Reachable" if reachable else "Exceeds soft limits"
                except Exception as e:
                    reachable = False
                    msg = str(e)
                    
                self.wfile.write(json.dumps({"reachable": reachable, "message": msg}).encode('utf-8'))
            except Exception as e:
                self.wfile.write(json.dumps({"reachable": False, "message": str(e)}).encode('utf-8'))
                
        else:
            self.wfile.write(json.dumps({"status": "unknown_endpoint"}).encode('utf-8'))

class ReusableHTTPServer(http.server.HTTPServer):
    allow_reuse_address = True

def http_server_thread():
    # Use port 8080 to prevent permission conflicts with low-level port 80
    server_address = ('', 8080)
    httpd = ReusableHTTPServer(server_address, RobotStateHandler)
    print("[HTTP Server] Dashboard serving on http://localhost:8080/ ...")
    httpd.serve_forever()

# ============================================================================
# 7. RTDE TCP STREAMING SERVER (Streams telemetry at 50 Hz on Port 5000)
# ============================================================================
def rtde_server_thread():
    rtde_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    rtde_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    rtde_sock.bind(("0.0.0.0", 5000))
    rtde_sock.listen(1)
    print("[RTDE Server] TCP telemetry listening on Port 5000...")
    
    while True:
        try:
            conn, addr = rtde_sock.accept()
            print(f"[RTDE Server] Client connected: {addr}")
            while True:
                with state_lock:
                    joints = list(g_joint_angles)
                    errors = list(g_pid_last_err)
                    temp = 38.5 + 0.5 * math.sin(time.time() * 0.1)
                    
                payload = struct.pack("<ffffff ffffff f", *joints, *errors, temp)
                conn.sendall(payload)
                time.sleep(0.02) # Stable 50 Hz rate
        except (ConnectionResetError, ConnectionAbortedError, socket.error):
            pass
        except Exception as e:
            print(f"[RTDE Server] Streaming error: {e}")
            time.sleep(1.0)

# ============================================================================
# 8. PRINCIPAL CONTROLLER SIMULATION LOOP (Runs in Webots)
# ============================================================================
def run_simulation():
    print("=" * 80)
    print("      EB-15 HIGH-FIDELITY MECHATRONIC WEBOTS SIMULATOR CONTROLLER")
    print("=" * 80)

    # Starts Webots Robot instance
    robot = Robot()
    time_step = int(robot.getBasicTimeStep())
    dt = time_step / 1000.0 # Control cycle dt (seconds)
    
    # Initialize actutators and sensors
    motors = []
    sensors = []
    for i in range(6):
        motor = robot.getDevice(f"motor_j{i+1}")
        sensor = robot.getDevice(f"sensor_j{i+1}")
        
        # Configure motor for velocity mode (necessary for PID steps emulation)
        motor.setPosition(float('inf'))
        motor.setVelocity(0.0)
        
        sensor.enable(time_step)
        
        motors.append(motor)
        sensors.append(sensor)
        
    # Read initial sensor values to home the starting target position
    robot.step(time_step)
    
    global g_joint_angles, g_pid_last_err, g_current_pose
    with state_lock:
        for i in range(6):
            val = math.degrees(sensors[i].getValue())
            if i == 1:
                val += 90.0 # J2 Offset: Webots to DH space
            g_joint_angles[i] = val
            
        q_rad = [math.radians(ang) for ang in g_joint_angles]
        T_init = forward_kinematics(q_rad)
        g_current_pose["x"] = T_init[0][3]
        g_current_pose["y"] = T_init[1][3]
        g_current_pose["z"] = T_init[2][3]
        g_current_pose["rx"], g_current_pose["ry"], g_current_pose["rz"] = matrix_to_euler([row[:3] for row in T_init[:3]])
        
    print(f"[Init] Robot Base Homed: Joints: {[round(x, 2) for x in g_joint_angles]} deg")
    print(f"[Init] Robot TCP Position: X={g_current_pose['x']:.2f}, Y={g_current_pose['y']:.2f}, Z={g_current_pose['z']:.2f} mm")

    # Launch background communications threads
    threading.Thread(target=http_server_thread, daemon=True).start()
    threading.Thread(target=rtde_server_thread, daemon=True).start()

    # Initial targets
    with state_lock:
        current_target_joints = list(g_joint_angles)
    
    # PID controllers (Optimized for the Webots kinematic loop to avoid derivative chattering)
    pids = [
        JointPID(kp=4.0, kd=0.0, gamma=8.0, limit_speed=MAX_SPEED_DEG_S[i])
        for i in range(6)
    ]

    traj_timer = 0.0
    
    while robot.step(time_step) != -1:
        # 1. Physical sensing read
        real_deg = [math.degrees(sensors[i].getValue()) for i in range(6)]
        
        # Apply J2 offset mapping from Webots space to DH space
        real_deg_DH = list(real_deg)
        real_deg_DH[1] += 90.0
        
        # 2. Slice trajectory tick every 50ms
        traj_timer += dt
        if traj_timer >= 0.050:
            traj_timer = 0.0
            with state_lock:
                if len(trajectory_queue) > 0:
                    current_target_joints = trajectory_queue.pop(0)

        # 3. Malha Fechada (200Hz PID step simulation per loop)
        step_errors = [0.0] * 6
        step_joint_angles = [0.0] * 6
        debug_counter = getattr(run_simulation, '_dbg_cnt', 0) + 1
        run_simulation._dbg_cnt = debug_counter
        for i in range(6):
            desired_deg = current_target_joints[i] # desired_deg in DH space
            real_deg_val = real_deg_DH[i]          # real_deg in DH space
            
            # Step PID+Tanh control law in DH space
            u_speed_deg_s, error = pids[i].step(desired_deg, real_deg_val, dt)
            
            step_errors[i] = error
            step_joint_angles[i] = real_deg_val
            
            if i == 1 and debug_counter % 200 == 0:
                print(f"[DEBUG J2] desired_DH={desired_deg:.2f}, real_DH={real_deg_val:.2f}, speed_cmd={u_speed_deg_s:.2f}")
                
            # Direct motor velocity commands to Webots
            u_speed_rad_s = math.radians(u_speed_deg_s)
            motors[i].setVelocity(u_speed_rad_s)
            
        # 4. Update real-time spatial Forward Kinematics for telemetry
        q_rad = [math.radians(ang) for ang in step_joint_angles]
        T_curr = forward_kinematics(q_rad)
        rx, ry, rz = matrix_to_euler([row[:3] for row in T_curr[:3]])
        
        with state_lock:
            for i in range(6):
                g_pid_last_err[i] = step_errors[i]
                g_joint_angles[i] = step_joint_angles[i]
            g_current_pose["x"] = T_curr[0][3]
            g_current_pose["y"] = T_curr[1][3]
            g_current_pose["z"] = T_curr[2][3]
            g_current_pose["rx"] = rx
            g_current_pose["ry"] = ry
            g_current_pose["rz"] = rz

if __name__ == "__main__":
    run_simulation()
