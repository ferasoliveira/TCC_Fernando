import os
import requests

# Create target directory
target_dir = r"c:\Users\ferna\OneDrive\Área de Trabalho\TCC_Fernando\raw\artigos"
os.makedirs(target_dir, exist_ok=True)

# List of papers/books to download
downloads = [
    {
        "name": "Modern Robotics: Mechanics, Planning, and Control",
        "url": "http://hades.mech.northwestern.edu/images/7/7f/MR.pdf",
        "filename": "modern_robotics_mechanics_planning_and_control.pdf"
    },
    {
        "name": "Introduction to Robotics: Mechanics and Control",
        "url": "https://marsuniversity.github.io/ece387/Introduction-to-Robotics-Craig.pdf",
        "filename": "introduction_to_robotics_mechanics_and_control.pdf"
    },
    {
        "name": "A Comprehensive Investigation of ESP32 in Enhancing Wi-Fi Range and Traffic Control",
        "url": "https://publicationsdrdo.in/index.php/dsj/article/download/20284/8518",
        "filename": "comprehensive_investigation_esp32_wifi.pdf"
    },
    {
        "name": "Research on Closed-loop Control of Step Motor Based on Magnetic Encoder",
        "url": "https://pdfs.semanticscholar.org/8678/bea830634c44e3c6b549724ef0508158dddd.pdf",
        "filename": "research_closed_loop_control_step_motor_magnetic_encoder.pdf"
    },
    {
        "name": "Developing and Analyzing a Novel Multimodal Stroke Rehabilitation System for Stroke Patients",
        "url": "https://www.scienceopen.com/document_file/77726fb2-eb15-488c-8b98-3a1448a2539b/ScienceOpen/jdr20250670.pdf",
        "filename": "developing_and_analyzing_novel_multimodal_stroke_rehabilitation.pdf"
    },
    {
        "name": "Simultaneous Triggering and Synchronization of Sensors and Onboard Computers",
        "url": "https://arxiv.org/pdf/2507.05717v1.pdf", # Wait, if it fails, we will try alternative
        "filename": "simultaneous_triggering_and_synchronization_sensors.pdf"
    }
]

headers = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36',
    'Accept-Encoding': 'gzip, deflate',
    'Connection': 'keep-alive'
}

for item in downloads:
    path = os.path.join(target_dir, item["filename"])
    print(f"Downloading {item['name']}...")
    try:
        r = requests.get(item["url"], headers=headers, timeout=30, stream=True)
        if r.status_code == 200:
            with open(path, 'wb') as f:
                for chunk in r.iter_content(chunk_size=8192):
                    f.write(chunk)
            print(f"SUCCESS: {item['name']}")
        else:
            print(f"FAILED: {item['name']} with status {r.status_code}")
    except Exception as e:
        print(f"ERROR: {item['name']}: {e}")
