#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
=============================================================================
             EB-15 Robotic Arm — I2C AS5600 Encoder Validation
=============================================================================
Este script valida numericamente a conversão de ticks brutos do AS5600 (12 bits)
para graus e radianos, além do algoritmo de rastreamento multi-voltas (multi-turn)
que evita instabilidade na transição de wrap-around de 360° para 0°.
=============================================================================
"""

import math

class AS5600EncoderTracker:
    def __init__(self):
        self.last_raw = None
        self.turns = 0
        self.initialized = False

    def update(self, raw_val: int) -> int:
        """
        Atualiza o rastreador com um novo valor bruto de 12 bits (0-4095).
        Retorna a contagem total acumulada de ticks (multi-turn).
        """
        assert 0 <= raw_val < 4096, "Valor bruto fora da faixa de 12 bits!"
        
        if not self.initialized:
            self.last_raw = raw_val
            self.turns = 0
            self.initialized = True
            return raw_val
            
        # Calcula a diferença com sinal entre a leitura atual e a última
        diff = raw_val - self.last_raw
        
        # Detecção de wrap-around baseada na metade da resolução (2048 ticks)
        if diff < -2048:
            # Cruzamento de 4095 -> 0 (Sentido Horário / Avanço)
            self.turns += 1
        elif diff > 2048:
            # Cruzamento de 0 -> 4095 (Sentido Anti-horário / Retorno)
            self.turns -= 1
            
        self.last_raw = raw_val
        accumulated_ticks = (self.turns * 4096) + raw_val
        return accumulated_ticks

def ticks_to_degrees(ticks: int) -> float:
    """Converte ticks acumulados para graus decimais."""
    return ticks * (360.0 / 4096.0)

def ticks_to_radians(ticks: int) -> float:
    """Converte ticks acumulados para radianos."""
    return ticks * (2 * math.pi / 4096.0)

def run_encoder_test_suite():
    print("=" * 70)
    print("      EB-15 ENCODER WRAP-AROUND & MULTI-TURN VALIDATION SUITE")
    print("=" * 70)
    
    tracker = AS5600EncoderTracker()
    
    # Sequência de leituras brutas simulando rotação contínua avançando
    # 1. Inicia em 4000
    # 2. Gira no sentido horário passando pelo 4095 -> 0 -> 100
    # 3. Gira no sentido anti-horário retornando de 100 -> 0 -> 4095 -> 3900
    simulated_reads = [
        4000,
        4050,
        4090,
        10,    # Wrap-around forward (+1 volta)
        50,
        100,
        50,
        5,
        4090,  # Wrap-around backward (-1 volta)
        4000,
        3900
    ]
    
    print("\n--- Rastreamento de Movimentação e Detecção de Transições ---")
    expected_accumulated = [
        4000,
        4050,
        4090,
        4106,  # 4096 + 10
        4146,  # 4096 + 50
        4196,  # 4096 + 100
        4146,
        4101,
        4090,  # Retornou para 0 voltas: 0*4096 + 4090
        4000,
        3900
    ]
    
    for idx, raw in enumerate(simulated_reads):
        accum = tracker.update(raw)
        deg = ticks_to_degrees(accum)
        rad = ticks_to_radians(accum)
        
        print(f"Leitura #{idx:02d}: Bruto={raw:4d} | Acumulado={accum:5d} | Graus={deg:7.2f}° | Radianos={rad:6.3f} rad")
        
        # Validação do acúmulo correto
        assert accum == expected_accumulated[idx], f"Falha! Acumulado lido {accum}, esperado {expected_accumulated[idx]}!"
        
    print("\n[SUCESSO] Rastreamento multi-voltas e prevenção de wrap-around validados!")
    print("=" * 70)

if __name__ == "__main__":
    run_encoder_test_suite()
