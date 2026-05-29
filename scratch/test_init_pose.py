import math

L1 = 150.0  # Base ao Ombro (mm)
L2 = 200.0  # Ombro ao Cotovelo (mm)
L3 = 200.0  # Cotovelo ao Punho (mm)
L6 = 80.0   # Centro do punho ao TCP (mm)

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

q_deg = [0.0, 90.0, 0.0, 0.0, 0.0, 0.0]
q_rad = [math.radians(ang) for ang in q_deg]
T = forward_kinematics(q_rad)
R = [row[:3] for row in T[:3]]
rx, ry, rz = matrix_to_euler(R)

print(f"TCP translation: X={T[0][3]}, Y={T[1][3]}, Z={T[2][3]}")
print(f"TCP orientation: rx={rx}, ry={ry}, rz={rz}")
