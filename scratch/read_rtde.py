import socket
import struct

def read_rtde():
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect(("127.0.0.1", 5000))
        data = sock.recv(52)
        sock.close()
        
        if len(data) == 52:
            unpacked = struct.unpack("<ffffff ffffff f", data)
            joints = unpacked[0:6]
            errors = unpacked[6:12]
            temp = unpacked[12]
            print("RTDE Telemetry Successfully Read:")
            print(f"Joints (deg): {[round(x, 4) for x in joints]}")
            print(f"Errors (deg): {[round(x, 4) for x in errors]}")
            print(f"Sim Temperature: {temp:.2f} C")
        else:
            print(f"Unexpected packet size: {len(data)} bytes")
    except Exception as e:
        print(f"Error reading RTDE: {e}")

if __name__ == "__main__":
    read_rtde()
