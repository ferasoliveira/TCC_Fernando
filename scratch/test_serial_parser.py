#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
=============================================================================
             EB-15 Robotic Arm — Serial Parser Validation Script
=============================================================================
Este script valida numericamente a serialização do frame binário de 10 bytes,
a robustez do cálculo de checksum XOR modular, e o comportamento do parser sob
condições severas de ruído de rede e corrupção de bits.
=============================================================================
"""

import struct
import random

# Estrutura do frame no formato de empacotamento 'struct' do Python:
# B = uint8_t (preamble)
# h = int16_t (steps_j4, little-endian '<')
# h = int16_t (steps_j5)
# h = int16_t (steps_j6)
# B = uint8_t (velocity)
# B = uint8_t (acceleration)
# B = uint8_t (checksum)
FRAME_FORMAT = "<BhhhBBB"
FRAME_SIZE = 10

def compute_checksum(payload_bytes: bytes) -> int:
    """Calcula a soma modular XOR acumulada sobre os bytes fornecidos."""
    xor_sum = 0
    for b in payload_bytes:
        xor_sum ^= b
    return xor_sum

def pack_frame(s4: int, s5: int, s6: int, vel: int, accel: int) -> bytes:
    """Serializa os parâmetros mecânicos no frame estruturado de 10 bytes."""
    # Prepara o payload temporário para calcular o checksum (primeiros 9 bytes)
    temp_bytes = struct.pack("<BhhhBB", 0xAA, s4, s5, s6, vel, accel)
    checksum = compute_checksum(temp_bytes)
    # Retorna o frame binário final completo com 10 bytes
    return temp_bytes + bytes([checksum])

def unpack_and_validate(frame_bytes: bytes) -> dict:
    """Simula a máquina de estados do Arduino Uno para parsear e validar o frame."""
    result = {"valid": False, "error": None, "data": None}
    
    if len(frame_bytes) != FRAME_SIZE:
        result["error"] = f"Tamanho incorreto do frame: {len(frame_bytes)} bytes."
        return result
        
    preamble = frame_bytes[0]
    if preamble != 0xAA:
        result["error"] = f"Preâmbulo de sincronismo inválido: 0x{preamble:02X}."
        return result
        
    received_checksum = frame_bytes[-1]
    computed_checksum = compute_checksum(frame_bytes[:-1])
    
    if received_checksum != computed_checksum:
        result["error"] = f"Checksum corrompido! Recebido = 0x{received_checksum:02X}, Esperado = 0x{computed_checksum:02X}."
        return result
        
    # Extrai os dados mecânicos estruturados
    unpacked = struct.unpack(FRAME_FORMAT, frame_bytes)
    result["valid"] = True
    result["data"] = {
        "j4": unpacked[1],
        "j5": unpacked[2],
        "j6": unpacked[3],
        "velocity": unpacked[4],
        "acceleration": unpacked[5],
        "checksum": unpacked[6]
    }
    return result

def inject_noise(frame: bytes, noise_rate: float = 0.1) -> bytes:
    """Injeta alterações aleatórias de bits (noise) no frame binário."""
    frame_list = list(frame)
    num_errors = 0
    for i in range(len(frame_list)):
        if random.random() < noise_rate:
            # Corrompe o byte aplicando uma máscara XOR aleatória (de 1 a 255)
            frame_list[i] ^= random.randint(1, 255)
            num_errors += 1
    return bytes(frame_list), num_errors

def run_validation_suite():
    print("=" * 70)
    print("      EB-15 SERIAL PROTOCOL PARSER & INTEGRITY VALIDATION SUITE")
    print("=" * 70)
    
    # -----------------------------------------------------------------------
    # TESTE 1: Geração de Frames Íntegros e Validação de Limites
    # -----------------------------------------------------------------------
    print("\n--- Teste 1: Geração e Parsing de Frames Nominais ---")
    nominal_cases = [
        (0, 0, 0, 50, 20),
        (500, -320, 100, 100, 50),
        (-32768, 32767, -1, 0, 0), # Limites extremos de int16
    ]
    
    for idx, (s4, s5, s6, vel, accel) in enumerate(nominal_cases):
        frame = pack_frame(s4, s5, s6, vel, accel)
        parse_result = unpack_and_validate(frame)
        
        hex_dump = " ".join(f"0x{b:02X}" for b in frame)
        print(f"\n[Caso #{idx + 1}] Parâmetros: J4={s4}, J5={s5}, J6={s6}, Vel={vel}, Acc={accel}")
        print(f"  Frame Binário (10 Bytes): [ {hex_dump} ]")
        
        if parse_result["valid"]:
            data = parse_result["data"]
            print(f"  Resultado: SUCESSO")
            print(f"  Parsing Confirmado: J4={data['j4']}, J5={data['j5']}, J6={data['j6']}, Checksum=0x{data['checksum']:02X}")
            # Validação lógica de identidade dos dados
            assert data["j4"] == s4 and data["j5"] == s5 and data["j6"] == s6, "Erro de alinhamento estrutural de variáveis!"
        else:
            print(f"  Resultado: FALHA - {parse_result['error']}")
            raise AssertionError("Falha inesperada no parsing de dados íntegros.")
            
    # -----------------------------------------------------------------------
    # TESTE 2: Simulação de Robustez sob Injeção de Ruído Binário
    # -----------------------------------------------------------------------
    print("\n--- Teste 2: Injeção Estocástica de Ruído e Taxa de Detecção ---")
    num_simulations = 10000
    undetected_errors = 0
    detected_errors = 0
    correct_frames = 0
    
    random.seed(42) # Estabilidade para reprodução do teste
    
    for _ in range(num_simulations):
        # Gera valores de trajetórias mecânicas aleatórias
        s4 = random.randint(-1000, 1000)
        s5 = random.randint(-1000, 1000)
        s6 = random.randint(-1000, 1000)
        vel = random.randint(1, 100)
        accel = random.randint(1, 50)
        
        original_frame = pack_frame(s4, s5, s6, vel, accel)
        
        # Injeta ruído elétrico com probabilidade de 15% por byte
        noisy_frame, errors_injected = inject_noise(original_frame, noise_rate=0.15)
        
        parse_result = unpack_and_validate(noisy_frame)
        
        if errors_injected == 0:
            assert parse_result["valid"] == True
            correct_frames += 1
        else:
            if parse_result["valid"]:
                # ERRO DE SEGUNDA ESPÉCIE: O frame corrompido foi aceito como válido (falha catastrófica de integridade)
                undetected_errors += 1
            else:
                # ERRO DETECTADO COM SUCESSO
                detected_errors += 1
                
    detection_rate = (detected_errors / (detected_errors + undetected_errors)) * 100.0 if (detected_errors + undetected_errors) > 0 else 100.0
    print(f"Simulações Executadas       : {num_simulations}")
    print(f"Frames Íntegros (Sem Ruído) : {correct_frames}")
    print(f"Erros Injetados e Detectados: {detected_errors}")
    print(f"Erros Não Detectados        : {undetected_errors}")
    print(f"Eficiência do Checksum XOR  : {detection_rate:.4f}%")
    
    # Exige uma eficiência mínima de detecção do XOR de 99.0% para homologar o firmware
    assert detection_rate >= 99.0, f"Taxa de detecção do XOR abaixo do mínimo aceitável! Eficiência: {detection_rate:.2f}%"
    print("\n[SUCESSO] Protocolo homologado com robustez matemática superior!")
    print("=" * 70)

if __name__ == "__main__":
    run_validation_suite()
