import urllib.request
import json

def get_url(url):
    try:
        with urllib.request.urlopen(url, timeout=2.0) as res:
            return json.loads(res.read().decode('utf-8'))
    except Exception as e:
        return {"error": str(e)}

print("Joints:")
print(json.dumps(get_url("http://localhost:8080/api/joints"), indent=2))
print("Pose:")
print(json.dumps(get_url("http://localhost:8080/api/pose"), indent=2))
print("Status:")
print(json.dumps(get_url("http://localhost:8080/api/status"), indent=2))
