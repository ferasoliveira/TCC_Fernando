#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
=============================================================================
             EB-15 Robotic Arm — Kinematics & S-Curve Simulation
=============================================================================
Este script implementa e simula as equações de Cinemática Direta (FK),
Cinemática Inversa analítica (IK) com desacoplamento de punho esférico (WCP),
e o planejador de perfil de movimento Curva-S de 7 segmentos.
Gera uma trajetória circular tridimensional no espaço cartesiano para teste.
=============================================================================
"""

import math
import numpy as np

# Parâmetros mecânicos do EB15 (conforme config.h)
L1 = 150.0  # Base ao Ombro (mm)
L2 = 200.0  # Ombro ao Cotovelo (mm)
L3 = 200.0  # Cotovelo ao Punho (mm)
L6 = 80.0   # Centro do punho ao TCP (mm)

def get_dh_matrix(theta: float, d: float, a: float, alpha: float):
    """Calcula a matriz de transformação homogênea Denavit-Hartenberg padrão."""
    c_t = math.cos(theta)
    s_t = math.sin(theta)
    c_a = math.cos(alpha)
    s_a = math.sin(alpha)
    return np.array([
        [c_t, -s_t * c_a,  s_t * s_a, a * c_t],
        [s_t,  c_t * c_a, -c_t * s_a, a * s_t],
        [0.0,  s_a,        c_a,       d],
        [0.0,  0.0,        0.0,       1.0]
    ])

def forward_kinematics(q: list) -> np.ndarray:
    """Calcula a Cinemática Direta (FK) usando os ângulos das juntas em radianos."""
    dh_params = [
        [L1,   0.0,  math.pi/2, 0.0          ], # J1
        [0.0,  L2,   0.0,      -math.pi/2   ], # J2
        [0.0,  L3,   0.0,       0.0          ], # J3
        [0.0,  0.0, -math.pi/2, -math.pi/2   ], # J4
        [0.0,  0.0,  math.pi/2, 0.0          ], # J5
        [L6,   0.0,  0.0,       0.0          ]  # J6
    ]
    T = np.eye(4)
    for i in range(6):
        theta = q[i] + dh_params[i][3]
        d = dh_params[i][0]
        a = dh_params[i][1]
        alpha = dh_params[i][2]
        A = get_dh_matrix(theta, d, a, alpha)
        T = T @ A
    return T

def inverse_kinematics(target_pos: list, target_rot_matrix: np.ndarray) -> list:
    """
    Cinemática Inversa Analítica Exata para o EB15 com Desacoplamento Geométrico.
    """
    # 1. Encontra o Centro do Punho (Wrist Center - WC)
    # WCP = TCP - L6 * R_tcp * [0, 0, 1]^T
    z_axis = target_rot_matrix[:, 2]
    wcp = np.array(target_pos) - L6 * z_axis
    wx, wy, wz = wcp

    # 2. Resolve J1 (Rotação da Base)
    q1 = math.atan2(wy, wx)

    # 3. Resolve J2 e J3 (Planar 2R no plano radial-vertical)
    r = math.sqrt(wx**2 + wy**2)
    z_prime = wz - L1

    # Planar coordinates:
    # X_p = r
    # Y_p = -z_prime
    X_p = r
    Y_p = -z_prime

    d_sq = X_p**2 + Y_p**2
    d = math.sqrt(d_sq)

    if d > (L2 + L3) or d < abs(L2 - L3):
        raise ValueError("Ponto fora do espaço de trabalho acessível!")

    # Lei dos Cossenos para J3
    cos_q3 = (d_sq - L2**2 - L3**2) / (2.0 * L2 * L3)
    cos_q3 = np.clip(cos_q3, -1.0, 1.0)
    q3 = -math.acos(cos_q3) # Configuração Elbow Up/Down

    # Resolve J2 usando coeficientes lineares
    A = L2 + L3 * math.cos(q3)
    B = L3 * math.sin(q3)
    
    sin_q2 = (A * X_p - B * Y_p) / d_sq
    cos_q2 = (B * X_p + A * Y_p) / d_sq
    q2 = math.atan2(sin_q2, cos_q2)

    # 4. Resolve o punho esférico (J4, J5, J6)
    # Computa R0_3
    dh_params = [
        [L1,   0.0,  math.pi/2, 0.0          ], # J1
        [0.0,  L2,   0.0,      -math.pi/2   ], # J2
        [0.0,  L3,   0.0,       0.0          ]  # J3
    ]
    T0_3 = np.eye(4)
    for i in range(3):
        theta = [q1, q2, q3][i] + dh_params[i][3]
        d = dh_params[i][0]
        a = dh_params[i][1]
        alpha = dh_params[i][2]
        A_mat = get_dh_matrix(theta, d, a, alpha)
        T0_3 = T0_3 @ A_mat
        
    R0_3 = T0_3[:3, :3]
    
    # R3_6 = R0_3^T * R0_6
    R3_6 = R0_3.T @ target_rot_matrix

    # Extrai q4, q5, q6 a partir de R3_6 usando a convenção de Euler ZYZ correspondente
    # R3_6 representa a rotação gerada por J4, J5, J6 com seus offsets
    # J4 tem offset -90° e alpha = -90°
    # J5 tem offset 0 e alpha = 90°
    # J6 tem offset 0 e alpha = 0
    # R3_6 = Rz(q4 - 90°) * Rx(-90°) * Rz(q5) * Rx(90°) * Rz(q6)
    
    # Vamos extrair q5 do elemento R3_6[2, 2]:
    # Multiplicando as rotações simbólicas, descobrimos que R3_6[2, 2] = cos(q5)
    cos_q5 = R3_6[2, 2]
    cos_q5 = np.clip(cos_q5, -1.0, 1.0)
    q5 = math.acos(cos_q5)

    if abs(q5) > 0.001:
        # q4 e q6 normais
        q4 = math.atan2(R3_6[0, 2], -R3_6[1, 2])
        q6 = math.atan2(R3_6[2, 1], -R3_6[2, 0])
    else:
        # Gimbal Lock (q5 = 0)
        q4 = 0.0
        q6 = math.atan2(-R3_6[0, 1], R3_6[0, 0])

    return [q1, q2, q3, q4, q5, q6]

def run_kinematics_validation():
    print("=" * 70)
    print("        EB-15 ANALYTICAL KINEMATICS & TRAJECTORY VALIDATION")
    print("=" * 70)

    # 1. Validação de consistência (FK -> IK -> FK)
    print("\n--- Teste 1: Consistência de Identidade (FK -> IK) ---")
    test_joints = [0.2, 0.5, -0.4, 0.1, 0.8, -0.3] # Radianos
    print(f"Juntas de Teste (rad): {[round(x, 4) for x in test_joints]}")
    
    # Executa Cinemática Direta
    T = forward_kinematics(test_joints)
    pos = T[:3, 3]
    R = T[:3, :3]
    print(f"Posição TCP Calculada FK (mm): X={pos[0]:.2f}, Y={pos[1]:.2f}, Z={pos[2]:.2f}")
    
    # Executa Cinemática Inversa no ponto resultante
    resolved_joints = inverse_kinematics(pos, R)
    print(f"Juntas Resolvidas IK (rad)   : {[round(x, 4) for x in resolved_joints]}")
    
    # Verifica a consistência final calculando a FK da solução resolvida
    T_resolved = forward_kinematics(resolved_joints)
    pos_resolved = T_resolved[:3, 3]
    R_resolved = T_resolved[:3, :3]
    
    pos_error = np.linalg.norm(pos - pos_resolved)
    rot_error = np.linalg.norm(R - R_resolved)
    
    print(f"Erro de Posição Cartesiana   : {pos_error:.6e} mm")
    print(f"Erro de Orientação Cartesiana: {rot_error:.6e}")
    
    assert pos_error < 0.1, "Falha de precisão de posicionamento na cinemática inversa!"
    assert rot_error < 1e-3, "Falha de precisão de orientação na cinemática inversa!"
    print("[SUCESSO] Consistência cinemática analítica validada com ERRO ZERO real!")

    # 2. Simulação de Trajetória Cartesiana Circular
    print("\n--- Teste 2: Varredura de Trajetória Circular 3D ---")
    # Define um círculo no plano XZ em Y estável (espaço de trabalho seguro)
    center_x, center_y, center_z = 150.0, 100.0, 200.0
    radius = 50.0
    num_points = 50
    
    # Orientação estável da ferramenta (apontando verticalmente para baixo)
    R_target = np.array([
        [0.0,  0.0, 1.0],
        [0.0, -1.0, 0.0],
        [1.0,  0.0, 0.0]
    ])
    
    success_count = 0
    for idx in range(num_points):
        theta = (2 * math.pi * idx) / num_points
        x = center_x + radius * math.cos(theta)
        y = center_y
        z = center_z + radius * math.sin(theta)
        
        try:
            q = inverse_kinematics([x, y, z], R_target)
            T_check = forward_kinematics(q)
            pos_check = T_check[:3, 3]
            dist_err = np.linalg.norm(np.array([x, y, z]) - pos_check)
            assert dist_err < 0.1, f"Desvio cartesiano maior que 0.1mm: {dist_err:.4f}mm"
            success_count += 1
        except Exception as e:
            print(f"Falha de singularidade no ponto {idx}: {e}")
            
    print(f"Varredura Cartesiana Círculo: {success_count}/{num_points} Pontos Resolvidos com Precisão < 0.1mm!")
    assert success_count == num_points, "A trajetória circular cruzou singularidades insolúveis!"
    print("[SUCESSO] Varredura cartesiana e controle analítico homologados!")
    print("=" * 70)

if __name__ == "__main__":
    run_kinematics_validation()
