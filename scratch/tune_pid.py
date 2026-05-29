import math

# exact PID+Tanh step control law from eb15_controller.py
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
        
        tanh_factor = math.tanh(self.gamma * abs(error))
        u_speed_deg_s = self.kp * error * tanh_factor + self.kd * err_diff
        u_speed_deg_s = max(-self.limit_speed, min(self.limit_speed, u_speed_deg_s))
        return u_speed_deg_s, error

def simulate_step(kp, kd, gamma, limit_speed, dt, sim_time, target_deg):
    pid = JointPID(kp, kd, gamma, limit_speed)
    real_deg = 0.0
    steps = int(sim_time / dt)
    
    itae = 0.0
    overshoot = 0.0
    settling_time = sim_time
    settled = False
    
    history = []
    
    for step in range(steps):
        t = step * dt
        u_speed, error = pid.step(target_deg, real_deg, dt)
        
        # Plant: Kinematic integrator
        real_deg += u_speed * dt
        
        # Calculate overshoot
        if real_deg > target_deg:
            os = (real_deg - target_deg) / target_deg
            if os > overshoot:
                overshoot = os
        
        # ITAE calculation
        itae += t * abs(error) * dt
        
        # Settling time (within 2% of target)
        if abs(error) <= 0.02 * target_deg:
            if not settled:
                settling_time = t
                settled = True
        else:
            settled = False
            settling_time = sim_time
            
        history.append((t, real_deg, u_speed))
        
    return itae, overshoot, settling_time, history

# Run optimization grid search
dt = 0.016 # Webots basic time step (16ms)
sim_time = 5.0 # 5 seconds simulation
target_deg = 90.0 # 90 degree step input
gamma = 8.0
limit_speed = 45.0 # degrees/sec (for J1-J3)

best_kp = 0.0
best_kd = 0.0
min_itae = float('inf')
best_overshoot = 0.0
best_settling_time = sim_time

print("Grid search in progress...")
# Search over stable region
for kp_idx in range(1, 51):
    kp = kp_idx * 0.1 # 0.1 to 5.0
    for kd_idx in range(0, 31):
        kd = kd_idx * 0.02 # 0.0 to 0.6 (Derivative gains larger than 1.0 are mathematically unstable)
        
        itae, overshoot, settling_time, _ = simulate_step(kp, kd, gamma, limit_speed, dt, sim_time, target_deg)
        
        # We require strictly no overshoot (< 0.1%) to prevent flexural vibrations in 3D printed arm
        if overshoot < 0.001:
            if itae < min_itae:
                min_itae = itae
                best_kp = kp
                best_kd = kd
                best_overshoot = overshoot
                best_settling_time = settling_time

print("="*60)
print("             OPTIMIZATION RESULTS (ITAE MINIMIZATION)")
print("="*60)
print(f"Optimal Proportional Gain (Kp): {best_kp:.2f}")
print(f"Optimal Derivative Gain (Kd)  : {best_kd:.4f}")
print(f"Minimum ITAE Cost             : {min_itae:.4f}")
print(f"Overshoot                     : {best_overshoot * 100.0:.4f}%")
print(f"Settling Time (2% criterion)  : {best_settling_time:.3f} s")
print("="*60)
