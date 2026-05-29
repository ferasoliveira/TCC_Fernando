import math

L1 = 150.0  # mm
L2 = 200.0  # mm
L3 = 200.0  # mm
L6 = 80.0   # mm

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

target_pos = [200.0, 0.0, 150.0]
R_target = euler_to_matrix(0.0, 0.0, 0.0)

q = inverse_kinematics(target_pos, R_target)
print("IK Solved (deg):")
for i, val in enumerate(q):
    print(f"J{i+1}: {math.degrees(val)}")
