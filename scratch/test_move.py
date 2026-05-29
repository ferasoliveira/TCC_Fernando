import urllib.request
import json
import time

def send_post(url, data):
    req = urllib.request.Request(
        url,
        data=json.dumps(data).encode('utf-8'),
        headers={'Content-Type': 'application/json'},
        method='POST'
    )
    with urllib.request.urlopen(req) as res:
        return json.loads(res.read().decode('utf-8'))

def send_get(url):
    with urllib.request.urlopen(url) as res:
        return json.loads(res.read().decode('utf-8'))

print("Sending move command...")
cmd = {
    "target": {
        "x": 200.0, "y": 0.0, "z": 150.0,
        "rx": 0.0, "ry": 0.0, "rz": 0.0
    },
    "speed_pct": 30.0
}
try:
    res = send_post("http://localhost:8080/api/move/linear", cmd)
    print("Move response:", res)
except Exception as e:
    print("Failed to send move command:", e)
    exit(1)

print("Waiting 5 seconds for movement to settle...")
time.sleep(5)

joints = send_get("http://localhost:8080/api/joints")
pose = send_get("http://localhost:8080/api/pose")
print("Settled Joints (DH space):")
print(json.dumps(joints, indent=2))
print("Settled Pose (FK):")
print(json.dumps(pose, indent=2))
