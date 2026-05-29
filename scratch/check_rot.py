import math
import numpy as np

# Definimos get_dh_matrix
def get_dh_matrix(theta, d, a, alpha):
    c_t = math.cos(theta)
    s_t = math.sin(theta)
    c_a = math.cos(alpha)
    s_a = math.sin(alpha)
    return np.array([
        [c_t, -s_t * c_a,  s_t * s_a, a * c_t],
        [s_t,  c_t * c_a, -c_t * s_a, a * s_t],
        [0,    s_a,        c_a,       d],
        [0,    0,          0,         1]
    ])

# Teste com valores específicos
q4, q5, q6 = 0.1, 0.8, -0.3

# Multiplica as 3 matrizes de rotação do punho
T3_4 = get_dh_matrix(q4 - math.pi/2, 0, 0, -math.pi/2)
T4_5 = get_dh_matrix(q5, 0, 0, math.pi/2)
T5_6 = get_dh_matrix(q6, 80.0, 0, 0)

R3_6 = (T3_4 @ T4_5 @ T5_6)[:3, :3]

print("R3_6 Matrix:")
print(R3_6)

# Vamos descobrir quais elementos de R3_6 nos dão q4, q5, q6!
print("\nTentativa de deduzir q5:")
print(f"R3_6[2,2]       = {R3_6[2,2]:.5f} vs cos(q5) = {math.cos(q5):.5f}")
print(f"R3_6[2,0]       = {R3_6[2,0]:.5f} vs sin(q5)*cos(q6) = {math.sin(q5)*math.cos(q6):.5f}")
print(f"R3_6[2,1]       = {R3_6[2,1]:.5f} vs sin(q5)*sin(q6) = {math.sin(q5)*math.sin(q6):.5f}")
print(f"R3_6[0,2]       = {R3_6[0,2]:.5f} vs -sin(q4)*sin(q5) = {-math.sin(q4)*math.sin(q5):.5f}")
print(f"R3_6[1,2]       = {R3_6[1,2]:.5f} vs cos(q4)*sin(q5) = {math.cos(q4)*math.sin(q5):.5f}")
