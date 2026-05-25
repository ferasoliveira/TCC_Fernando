import os
import requests

target_dir = r"c:\Users\ferna\OneDrive\Área de Trabalho\TCC_Fernando\raw\artigos"
os.makedirs(target_dir, exist_ok=True)

url = "https://github.com/yangmingustb/planning_books_1/raw/master/Siciliano%20-%202009%20-%20Robotics%20modelling%2C%20planning%20and%20control.pdf"
filename = "robotics_modelling_planning_and_control.pdf"
path = os.path.join(target_dir, filename)

headers = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36',
    'Accept-Encoding': 'gzip, deflate',
    'Connection': 'keep-alive'
}

print(f"Downloading Robotics: Modelling, Planning and Control...")
try:
    r = requests.get(url, headers=headers, timeout=60, stream=True)
    if r.status_code == 200:
        with open(path, 'wb') as f:
            for chunk in r.iter_content(chunk_size=8192):
                f.write(chunk)
        print("SUCCESS: Robotics: Modelling, Planning and Control")
    else:
        print(f"FAILED: status {r.status_code}")
except Exception as e:
    print(f"ERROR: {e}")
