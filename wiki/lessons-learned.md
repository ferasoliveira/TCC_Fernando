# Lessons Learned — EB15 Robotic Arm Project

This document catalogs technical challenges, debugging breakthroughs, and best practices discovered during the development of the EB15.

---

### HTTP Thread Starvation & DNS Timeout (Data: 2026-05-29)
- **Problema:** The 200 Hz simulation loop was acquiring the global interpreter state lock 8 times per cycle, causing high lock contention and starving the background HTTP thread. Concurrently, Python's `BaseHTTPRequestHandler` was performing reverse DNS lookups on every request, creating a 20-second connection hang in offline sandboxed environments.
- **Solução:** Overrode `address_string()` in `RobotStateHandler` to return raw IP, bypassing reverse DNS lookup completely. Refactored `run_simulation` to combine lock acquisitions into a single telemetry update block at the end of each cycle, and moved trajectory calculations completely outside of the HTTP POST lock context.
- **Regra Futura (Boa Prática):** In multithreaded simulation environments, always bypass reverse DNS lookups by overriding `address_string()` to return the raw IP, and keep lock critical sections extremely short to prevent thread starvation.

### Silent Cartesian Trajectory Truncation (Data: 2026-05-29)
- **Problema:** When a linear trajectory slice failed the analytical Inverse Kinematics (due to temporary boundary singular configs or out-of-reach path segments), the planner silently bypassed the failure using `except Exception: continue`. This truncated the path slice queue, causing the arm to stop midway and settle at incorrect intermediate coordinates.
- **Solução:** Refactored the `plan_move_l` routine to implement a fail-fast design. If any intermediate trajectory slice fails the IK solver, a descriptive `ValueError` is raised detailing the exact step and coordinate, preventing partial execution of corrupt paths.
- **Regra Futura (Boa Prática):** Never swallow kinematic planning errors with silent catch blocks; always raise descriptive exceptions to expose path failures immediately.
