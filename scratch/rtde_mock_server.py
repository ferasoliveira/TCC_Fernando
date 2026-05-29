#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
=============================================================================
         EB-15 Robotic Arm — RTDE TCP Mock Server (ESP32 Emulator)
=============================================================================
Este script emula o comportamento do servidor RTDE TCP do ESP32 S3. Ele roda
localmente na porta 5000 e transmite uma trajetória contínua e suave (onda)
a 50 Hz para que você veja o robô se mover no Webots sem precisar de hardware.
=============================================================================
"""

import socket
import struct
import time
import math

HOST = "127.0.0.1"   # Localhost para simulação interna
PORT = 5000          # Porta RTDE
RTDE_FORMAT = "<ffffff ffffff f"

def main():
    print("=" * 70)
    print("      EB-15 RTDE TCP MOCK SERVER (ESP32 EMULATOR)")
    print("=" * 70)
    print(f"Iniciando Servidor TCP em {HOST}:{PORT}...")
    
    server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_sock.bind((HOST, PORT))
    server_sock.listen(1)
    
    print("Aguardando conexão do controlador do Webots...")
    
    try:
        conn, addr = server_sock.accept()
        print(f"Controlador conectado com sucesso a partir de: {addr}")
        
        start_time = time.time()
        
        while True:
            t = time.time() - start_time
            
            # Gera uma trajetória contínua e suave (movimento senoidal sincronizado)
            j1 = 45.0 * math.sin(t * 1.0)          # J1: Rotação da base de -45° a 45°
            j2 = 30.0 * math.cos(t * 1.2) + 20.0   # J2: Ombro
            j3 = -40.0 * math.sin(t * 0.8) - 10.0  # J3: Cotovelo
            j4 = 15.0 * math.sin(t * 1.5)          # J4: Giro do punho
            j5 = 25.0 * math.cos(t * 1.0)          # J5: Flexão do punho
            j6 = 90.0 * math.sin(t * 2.0)          # J6: Giro final
            
            joints = [j1, j2, j3, j4, j5, j6]
            
            # Erros fictícios de malha fechada (ruído dinâmico sub-milimétrico)
            errors = [0.005 * math.sin(t * 5.0) for _ in range(6)]
            
            # Temperatura fictícia oscilando levemente
            temp = 38.5 + 0.5 * math.sin(t * 0.1)
            
            # Empacota a telemetria (52 bytes)
            payload = struct.pack(RTDE_FORMAT, *joints, *errors, temp)
            
            # Transmite para o cliente do Webots
            conn.sendall(payload)
            
            # Mantém a taxa estável de 50 Hz (20 ms por ciclo)
            time.sleep(0.02)
            
    except ConnectionResetError:
        print("\n[INFO] Conexão fechada pelo Webots.")
    except KeyboardInterrupt:
        print("\n[INFO] Servidor finalizado pelo usuário.")
    finally:
        server_sock.close()
        print("Servidor TCP encerrado.")

if __name__ == "__main__":
    main()
