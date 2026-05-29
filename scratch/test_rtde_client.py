#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
=============================================================================
             EB-15 Robotic Arm — RTDE TCP Client Mock Validation
=============================================================================
Este script simula e valida o parsing dos frames binários compactos de 
telemetria enviados via barramento TCP RTDE (Real-Time Data Exchange) 
a 50 Hz pelo ESP32 S3.
=============================================================================
"""

import struct
import socket
import time

# Estrutura do frame RTDE:
# 6 floats = juntas J1-J6 reais (graus) -> 24 bytes
# 6 floats = erros de rastreamento J1-J6 (graus) -> 24 bytes
# 1 float  = temperatura interna do chip (°C) -> 4 bytes
# Total: 52 bytes compactos empacotados em Little Endian '<'
RTDE_FORMAT = "<ffffff ffffff f"
RTDE_SIZE = 52

def parse_rtde_payload(payload: bytes) -> dict:
    """Realiza o parsing de alta velocidade e descompactação do frame binário."""
    if len(payload) != RTDE_SIZE:
        raise ValueError(f"Tamanho do payload inválido! Esperado: {RTDE_SIZE} bytes, Recebido: {len(payload)} bytes.")
        
    data = struct.unpack(RTDE_FORMAT, payload)
    
    return {
        "joints": list(data[0:6]),
        "errors": list(data[6:12]),
        "temperature": data[12]
    }

def run_rtde_client_validation():
    print("=" * 70)
    print("        EB-15 RTDE TCP TELEMETRY PARSING & PROTOCOL VALIDATION")
    print("=" * 70)
    
    # Gera um frame binário simulado (dados representativos de regime dinâmico)
    mock_joints = [12.5, -45.0, 85.3, 0.1, -12.4, 180.0]     # Juntas reais (graus)
    mock_errors = [0.012, -0.051, 0.082, 0.001, -0.002, 0.045] # Erros de controle (graus)
    mock_temp = 41.5                                           # Temperatura do chip (°C)
    
    # Serialização do frame compactado
    payload = struct.pack(RTDE_FORMAT, *mock_joints, *mock_errors, mock_temp)
    
    print(f"Frame Binário Gerado: {len(payload)} bytes compactos.")
    hex_dump = " ".join(f"{b:02X}" for b in payload)
    print(f"Dump Hexadecimal    : {hex_dump[:70]}...")
    
    # Executa o parsing
    start_time = time.perf_counter()
    parsed_data = parse_rtde_payload(payload)
    latency = (time.perf_counter() - start_time) * 1000.0
    
    print(f"\nTelemetria Desempacotada em {latency:.4f} ms (Alta Performance):")
    print("-" * 55)
    print("  Junta   | Angulo Real (deg) | Erro de Rastreamento (deg)")
    print("-" * 55)
    for i in range(6):
        print(f"   J{i+1}     |     {parsed_data['joints'][i]:8.2f}        |       {parsed_data['errors'][i]:8.3f}")
    print("-" * 55)
    print(f"  Temperatura Embarcada do ESP32 S3: {parsed_data['temperature']:.1f} °C")
    print("-" * 55)
    
    # Confirmação de integridade absoluta do parser
    assert abs(parsed_data["joints"][0] - 12.5) < 1e-4, "Erro no desempacotamento de J1!"
    assert abs(parsed_data["temperature"] - 41.5) < 1e-4, "Erro no desempacotamento da Temperatura!"
    print("\n[SUCESSO] Protocolo binário RTDE TCP homologado com sucesso!")
    print("=" * 70)

if __name__ == "__main__":
    run_rtde_client_validation()
