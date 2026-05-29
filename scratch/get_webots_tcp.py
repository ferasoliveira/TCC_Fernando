from controller import Supervisor
import math

def get_positions():
    super = Supervisor()
    time_step = int(super.getBasicTimeStep())
    super.step(time_step)
    
    # Get the gripper_link node
    gripper = super.getFromDef("gripper_link")
    if gripper is None:
        # Try to find the node by name
        gripper = super.getDevice("gripper_link")
        
    print("Gripper Node:", gripper)
    if gripper:
        pos = gripper.getPosition()
        ori = gripper.getOrientation()
        print(f"Physical Position: X={pos[0]*1000.0:.2f}, Y={pos[1]*1000.0:.2f}, Z={pos[2]*1000.0:.2f} mm")
        print("Physical Orientation Matrix:", ori)
        
    # Let's also print all joint positions from sensors
    sensors = []
    for i in range(6):
        sensor = super.getDevice(f"sensor_j{i+1}")
        sensor.enable(time_step)
        sensors.append(sensor)
        
    super.step(time_step)
    
    print("Joint angles from sensors (deg):")
    for i in range(6):
        val = math.degrees(sensors[i].getValue())
        print(f"J{i+1}: {val:.4f}")

if __name__ == "__main__":
    get_positions()
