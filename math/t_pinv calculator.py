import numpy as np

# Thruster Positions Relative to the Center of Gravity
pos = np.array([
    [ 0.156,  0.111, -0.05], # Thruster 1
    [ 0.156, -0.111, -0.05], # Thruster 2
    [-0.156,  0.111, -0.05], # Thruster 3
    [-0.156, -0.111, -0.05], # Thruster 4
    [ 0.120,  0.218,  0.00], # Thruster 5
    [ 0.120, -0.218,  0.00], # Thruster 6
    [-0.120,  0.218,  0.00], # Thruster 7
    [-0.120, -0.218,  0.00]  # Thruster 8
])

# Normalized Thrust Vectors
vec = np.array([
    [-0.707,  0.707,  0.0], # Thruster 1
    [-0.707, -0.707,  0.0], # Thruster 2
    [ 0.707,  0.707,  0.0], # Thruster 3
    [ 0.707, -0.707,  0.0], # Thruster 4
    [ 0.0,    0.0,   -1.0], # Thruster 5
    [ 0.0,    0.0,   -1.0], # Thruster 6
    [ 0.0,    0.0,   -1.0], # Thruster 7
    [ 0.0,    0.0,   -1.0]  # Thruster 8
])

T = np.zeros((6, 8))
for i in range(8):
    T[0:3, i] = vec[i]
    T[3:6, i] = np.cross(pos[i], vec[i])

T_pinv = np.linalg.pinv(T)

print("const float T_pinv[48] = {")
for row in T_pinv:
    print("    " + ", ".join([f"{val:8.4f}f" for val in row]) + ",")
print("};")

input()
