---
name: robotics-fundamentals
description: Robotics fundamentals relevant to the EB-15 project. DH parameters, FK, IK, velocity profiles, joint control, degrees of freedom. Use when working on kinematics or motion planning.
allowed-tools: Read, Glob, Grep
version: 1.0
---

# Robotics Fundamentals — EB-15 Context

> Core robotics concepts needed for developing and documenting the EB-15 arm.

---

## Degrees of Freedom (DOF)

The EB-15 is a **6-DOF serial manipulator**:

| Joint | Type | Axis | Controller |
|---|---|---|---|
| J1 | Revolute | Base rotation | ESP32-S3 |
| J2 | Revolute | Shoulder | ESP32-S3 |
| J3 | Revolute | Elbow | ESP32-S3 |
| J4 | Revolute | Wrist 1 | Arduino Uno |
| J5 | Revolute | Wrist 2 | Arduino Uno |
| J6 | Revolute | Wrist 3 (tool) | Arduino Uno |

---

## Denavit-Hartenberg (DH) Parameters

Standard DH convention: each joint described by 4 parameters.

| Parameter | Symbol | Meaning |
|---|---|---|
| Link length | a | Distance along x-axis between joint axes |
| Link twist | α | Angle between joint axes (around x) |
| Link offset | d | Distance along z-axis |
| Joint angle | θ | Rotation around z-axis (variable for revolute) |

> **EB-15 Note:** DH parameters require physical measurement of the actual robot. Placeholder values should be clearly marked as such until real measurements are taken.

---

## Forward Kinematics (FK)

**Input:** Joint angles (θ1...θ6) → **Output:** End-effector pose (x, y, z, Rx, Ry, Rz)

Computed by chaining transformation matrices:

```
T_0^6 = A_1 · A_2 · A_3 · A_4 · A_5 · A_6
```

Where each A_i is the 4×4 homogeneous transformation from DH parameters.

---

## Inverse Kinematics (IK)

**Input:** Desired pose → **Output:** Joint angles

| Approach | Pros | Cons | EB-15 Status |
|---|---|---|---|
| **Analytical** | Fast, exact, all solutions | Specific to geometry | Future work |
| **Numerical (iterative)** | Generic | Slower, may not converge | Current stub |
| **Geometric** | Intuitive for simple arms | Hard for 6-DOF | Not applicable |

---

## Velocity Profiles

### Trapezoidal Profile (Current EB-15)

```
Velocity
    ▲
    │    ┌────────────┐
    │   /              \
    │  /                \
    │ /                  \
    └──────────────────── ► Time
    Accel   Cruise   Decel
```

| Phase | Description |
|---|---|
| Acceleration | Linear ramp from 0 to max velocity |
| Cruise | Constant max velocity |
| Deceleration | Linear ramp from max velocity to 0 |

### S-Curve (Future Enhancement)

Smoother jerk profile — reduces mechanical stress. More complex to implement.

---

## Motion Types

| Type | Description | Space | EB-15 Status |
|---|---|---|---|
| **MoveJ** | Joint interpolation | Joint-space | Implemented |
| **MoveL** | Linear in Cartesian | Task-space | Stub (needs IK) |
| **MoveC** | Circular arc | Task-space | Structure only |

---

## Stepper Motor Control

| Concept | Detail |
|---|---|
| Steps per revolution | Depends on motor (NEMA 17: 200 steps/rev) |
| Microstepping | TB6600 supports 1/1 to 1/32 |
| Steps per degree | `(steps_per_rev × microstepping × gear_ratio) / 360` |
| Maximum speed | Limited by driver and motor torque curve |

---

## Key References

| Topic | Standard Reference |
|---|---|
| DH Convention | Denavit & Hartenberg, 1955 |
| Robot Modeling | Siciliano et al., "Robotics: Modelling, Planning and Control" |
| Industrial Robots | ISO 9283 (Performance criteria and test methods) |
| UR-10 Specs | Universal Robots technical datasheet |
