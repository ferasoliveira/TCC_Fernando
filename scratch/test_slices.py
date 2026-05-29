import math
from test_fk import inverse_kinematics, euler_to_matrix

start_pos = [400.0, -80.0, 150.0]
start_rot = [90.0, 90.0, 0.0]

target_pos = [200.0, 0.0, 150.0]
target_rot = [0.0, 0.0, 0.0]

slices = 10
for s in range(1, slices + 1):
    tau = s / slices
    factor = 10 * (tau**3) - 15 * (tau**4) + 6 * (tau**5)
    
    slice_pos = [
        start_pos[i] + (target_pos[i] - start_pos[i]) * factor
        for i in range(3)
    ]
    slice_rot = [
        start_rot[i] + (target_rot[i] - start_rot[i]) * factor
        for i in range(3)
    ]
    
    try:
        R_slice = euler_to_matrix(slice_rot[0], slice_rot[1], slice_rot[2])
        slice_joints_rad = inverse_kinematics(slice_pos, R_slice)
        slice_joints_deg = [math.degrees(q) for q in slice_joints_rad]
        print(f"Slice {s}/{slices}: Reachable! Pos={slice_pos}, Rot={slice_rot}")
    except Exception as e:
        print(f"Slice {s}/{slices}: FAILED! Pos={slice_pos}, Rot={slice_rot}. Error: {e}")
