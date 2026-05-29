import math

L1 = 150.0  # Base ao Ombro (mm)
L2 = 200.0  # Ombro ao Cotovelo (mm)
L3 = 200.0  # Cotovelo ao Punho (mm)
L6 = 80.0   # Centro do punho ao TCP (mm)

LIMIT_MIN_DEG = [-170.0, -45.0, -120.0, -180.0, -90.0, -360.0]
LIMIT_MAX_DEG = [170.0, 180.0, 120.0, 180.0, 90.0, 360.0]

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

def inverse_kinematics(target_pos, R_target):
    z_axis = [R_target[0][2], R_target[1][2], R_target[2][2]]
    wcp = [
        target_pos[0] - L6 * z_axis[0],
        target_pos[1] - L6 * z_axis[1],
        target_pos[2] - L6 * z_axis[2]
    ]
    wx, wy, wz = wcp
    q1 = math.atan2(wy, wx)
    r = math.sqrt(wx**2 + wy**2)
    z_prime = wz - L1
    X_p = r
    Y_p = -z_prime
    d_sq = X_p**2 + Y_p**2
    d = math.sqrt(d_sq)
    if d > (L2 + L3) or d < abs(L2 - L3):
        raise ValueError(f"Target coordinate out of reach! d={d:.1f}, L2+L3={L2+L3}")
    cos_q3 = (d_sq - L2**2 - L3**2) / (2.0 * L2 * L3)
    cos_q3 = max(-1.0, min(1.0, cos_q3))
    q3 = -math.acos(cos_q3)
    A = L2 + L3 * math.cos(q3)
    B = L3 * math.sin(q3)
    sin_q2 = (A * X_p - B * Y_p) / d_sq
    cos_q2 = (B * X_p + A * Y_p) / d_sq
    q2 = math.atan2(sin_q2, cos_q2)
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

def check_soft_limits(q_deg):
    for i in range(6):
        if q_deg[i] < LIMIT_MIN_DEG[i] or q_deg[i] > LIMIT_MAX_DEG[i]:
            return False
    return True

# Simulation of plan_move_l
g_current_pose = {"x": 400.0, "y": -80.0, "z": 150.0, "rx": 90.0, "ry": 90.0, "rz": 0.0}
g_joint_angles = [0.0, 90.0, 0.0, 0.0, 0.0, 0.0]

target_pose = {"x": 200.0, "y": 0.0, "z": 150.0, "rx": 0.0, "ry": 0.0, "rz": 0.0}
speed_pct = 30.0

target_pos = [target_pose["x"], target_pose["y"], target_pose["z"]]
R_target = euler_to_matrix(target_pose["rx"], target_pose["ry"], target_pose["rz"])

try:
    target_joints_rad = inverse_kinematics(target_pos, R_target)
    target_joints_deg = [math.degrees(q) for q in target_joints_rad]
    print(f"Target is Reachable: Joints = {[round(ang, 2) for ang in target_joints_deg]} deg")
except Exception as e:
    print(f"Target IS NOT REACHABLE! Error: {e}")
    exit(1)

if not check_soft_limits(target_joints_deg):
    print("Joint limits exceeded for target!")
    exit(1)

start_pos = [g_current_pose["x"], g_current_pose["y"], g_current_pose["z"]]
start_rot = [g_current_pose["rx"], g_current_pose["ry"], g_current_pose["rz"]]

diff_pos = math.sqrt(
    (target_pos[0] - start_pos[0])**2 +
    (target_pos[1] - start_pos[1])**2 +
    (target_pos[2] - start_pos[2])**2
)

speed_factor = speed_pct / 100.0
duration_s = diff_pos / (80.0 * speed_factor) # Max linear speed of 80mm/s
if duration_s < 0.5:
    duration_s = 0.5
    
slices = int(duration_s * 1000.0 / 50.0)
if slices < 1:
    slices = 1

print(f"Total distance: {diff_pos:.2f} mm, slices: {slices}")

new_slices = []
failures = 0
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
        failures += 1
        print(f"Slice {s}/{slices} failed! Pos={slice_pos}, Rot={slice_rot}, Error={e}")

print(f"Completed! Total slices planned: {len(new_slices)} / {slices}. Failures: {failures}")
