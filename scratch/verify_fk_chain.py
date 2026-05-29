"""
Verify the FK chain and compare Webots kinematic chain vs DH parameters.
Traces the cumulative translations through the .wbt file anchors/endPoints.
"""
import math

L1 = 150.0  # mm
L2 = 200.0  # mm
L3 = 200.0  # mm
L6 = 80.0   # mm

# ===========================================================================
# 1. DH Forward Kinematics (from controller)
# ===========================================================================
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
        [L1,   0.0,  math.pi/2, 0.0        ],  # J1: d, a, alpha, theta_offset
        [0.0,  L2,   0.0,      -math.pi/2  ],  # J2
        [0.0,  L3,   0.0,       0.0         ],  # J3
        [0.0,  0.0, -math.pi/2, -math.pi/2  ],  # J4
        [0.0,  0.0,  math.pi/2, 0.0         ],  # J5
        [L6,   0.0,  0.0,       0.0         ]   # J6
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

# ===========================================================================
# 2. Test Various Joint Configurations
# ===========================================================================
print("=" * 70)
print("FK Verification for EB15 DH Parameters")
print("=" * 70)

# Test 1: Home position in DH space: [0, 90, 0, 0, 0, 0] deg
# (Webots J2 = 0, DH J2 = 90 due to +90 offset)
q_home_dh = [0.0, 90.0, 0.0, 0.0, 0.0, 0.0]
q_rad = [math.radians(x) for x in q_home_dh]
T = forward_kinematics(q_rad)
print(f"\n[Test 1] DH Home [0, 90, 0, 0, 0, 0] deg:")
print(f"  TCP = X:{T[0][3]:.2f}, Y:{T[1][3]:.2f}, Z:{T[2][3]:.2f} mm")

# Test 2: All zeros in DH space
q_zero = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
q_rad = [math.radians(x) for x in q_zero]
T = forward_kinematics(q_rad)
print(f"\n[Test 2] DH All-Zero [0, 0, 0, 0, 0, 0] deg:")
print(f"  TCP = X:{T[0][3]:.2f}, Y:{T[1][3]:.2f}, Z:{T[2][3]:.2f} mm")

# Test 3: Target [200, 0, 150] — what should the IK solution be?
print(f"\n[Test 3] Expected target: [200.0, 0.0, 150.0] mm")
print(f"  L1={L1}, L2={L2}, L3={L3}, L6={L6}")

# Webots chain trace
print("\n" + "=" * 70)
print("Webots Chain Trace (from .wbt file)")
print("=" * 70)
print("""
Robot origin:           (0, 0, 0.2) m from world = (0, 0, 200) mm
  |
  J1 anchor:            (0, 0, 0.15) m = (0, 0, 150) mm from robot
  J1 endPoint:          (0, 0, 0.15) m = (0, 0, 150) mm from robot
  => J1 center at world Z = 200 + 150 = 350 mm
  |
  J2 anchor (local):    (0, 0, 0.05) m = (0, 0, 50) mm from shoulder_link
  J2 endPoint:          (0, 0, 0.05) m = (0, 0, 50) mm from shoulder_link
  => J2 center at Z = 350 + 50 = 400 mm from world
  |
  J3 anchor (local):    (0.2, 0, 0) m = (200, 0, 0) mm from arm_link origin
  J3 endPoint:          (0.2, 0, 0) m from arm_link
  |
  J4 anchor (local):    (0.2, 0, 0) m = (200, 0, 0) mm from forearm_link origin
  J4 endPoint:          (0.2, 0, 0) m from forearm_link
  |
  J5 anchor (local):    (0, 0, 0) m  = WCP colocated with J4 origin
  J5 endPoint:          no translation
  |
  J6 anchor (local):    (0.03, 0, 0) m = 30 mm from wrist_pitch_link
  J6 endPoint:          (0.03, 0, 0) m
  |
  TCP ref sphere:       (0.012, 0, 0) m from gripper_link
  => Total from WCP to TCP = 30 + 12 = 42 mm
""")

print("CRITICAL ISSUE:")
print(f"  DH expects L6 (WCP to TCP) = {L6} mm")
print(f"  Webots chain gives WCP to TCP = 42 mm")
print(f"  MISMATCH = {L6 - 42} mm")
print()

# Also check L1 mismatch
# DH: L1 = 150 mm (d parameter of J1 = height along Z from base to shoulder)
# Webots: Robot at Z=0.2, J1 anchor at Z=0.15 from robot, J2 anchor at Z=0.05 from shoulder
# So total height from robot base to J2 pivot = 0.15 + 0.05 = 0.20 m = 200 mm
# But DH L1 = 150 mm
print("L1 CHECK:")
print(f"  DH L1 = {L1} mm (base to shoulder, d-parameter of J1)")
print(f"  Webots: J1 at 150mm + J2 offset 50mm = 200mm total from robot base to J2 pivot")
print(f"  The DH d-parameter L1 represents the Z offset to the J2 rotation center")
print(f"  In Webots: J1 anchor = 150mm, then J2 is 50mm higher = 200mm total")
print(f"  But DH convention: L1 is the d1 parameter (J1 frame Z displacement)")
print(f"  So effectively the shoulder is at Z=L1=150mm in DH, but in Webots it's at Z=200mm")
print(f"  POTENTIAL MISMATCH of 50 mm in Z height")
