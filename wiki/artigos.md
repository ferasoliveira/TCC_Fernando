# Fundamentação Teórica e Revisão Bibliográfica do TCC

Este documento reúne a catalogação estruturada, avaliação técnica e citações no formato BibTeX de todos os livros fundamentais e artigos científicos avaliados para o desenvolvimento do braço robótico de 6 graus de liberdade **EB15**.

---

### Título: Modern Robotics: Mechanics, Planning, and Control
- **Arquivo:** `raw/artigos/modern_robotics_mechanics_planning_and_control.pdf`
- **Relevância para o TCC:** `9.5/10`
- **Objetivo:** Introduzir a cinemática, dinâmica, planejamento de trajetória e controle de manipulações robóticas usando a Teoria de Parafusos (Screw Theory) em oposição à parametrização clássica de Denavit-Hartenberg.
- **Resumo:** O livro aborda de forma rigorosa a representação de rotações e translações no espaço de coordenadas tridimensionais (grupos de Lie SO(3) e SE(3)), a formulação geométrica baseada em vetores espaciais e parafusos mecânicos para cinemática direta e dinâmica (algoritmo de Newton-Euler recursivo), cinemática inversa analítica e numérica, planejamento de trajetórias com perfis de velocidade e controle dinâmico no espaço de juntas e operacional.
- **Insights:** Para o braço EB15 (6 DOFs), a modelagem geométrica via Screw Theory (fórmula do produto de exponenciais) simplifica o tratamento matemático das juntas e evita singularidades associadas aos parâmetros DH clássicos. Além disso, fornece o suporte conceitual para a divisão e sequenciamento de trajetórias em tempo real executadas pelo processador central ESP32 S3.
- **Citação:**
```bibtex
@book{lynch2017modern,
    author    = {Kevin M. Lynch and Frank C. Park},
    title     = {Modern Robotics: Mechanics, Planning, and Control},
    publisher = {Cambridge University Press},
    year      = {2017},
    isbn      = {9781107156302}
}
```

---

### Título: Robotics: Modelling, Planning and Control
- **Arquivo:** `raw/artigos/robotics_modelling_planning_and_control.pdf`
- **Relevância para o TCC:** `9.8/10`
- **Objetivo:** Fornecer um tratamento completo e rigoroso da cinemática, dinâmica, geração de trajetórias, atuadores/sensores e arquiteturas de controle de robôs manipuladores industriais.
- **Resumo:** Esta obra clássica de Bruno Siciliano detalha desde a modelagem matemática básica (convenções D-H, Jacobiano analítico e geométrico) até métodos avançados de controle dinâmico (torque computado, controle de força/impedância, controle adaptativo). Apresenta também um capítulo crucial sobre arquiteturas de controle industriais, diferenciando os laços de controle rápidos baseados em hardware de baixo nível das tarefas de supervisão e planejamento de trajetória de alto nível.
- **Insights:** O capítulo sobre arquiteturas de controle justifica diretamente a divisão distribuída do EB15: o ESP32 S3 atua na camada de alto nível (planejamento global, interface RTDE/web, cinemática J1-J3) enquanto o Arduino Uno executa o loop de baixo nível de movimentação física das juntas superiores J4-J6.
- **Citação:**
```bibtex
@book{siciliano2009robotics,
    author    = {Bruno Siciliano and Lorenzo Sciavicco and Luigi Villani and Giuseppe Oriolo},
    title     = {Robotics: Modelling, Planning and Control},
    publisher = {Springer Science \& Business Media},
    year      = {2009},
    isbn      = {9781848825413}
}
```

---

### Título: Introduction to Robotics: Mechanics and Control
- **Arquivo:** `raw/artigos/introduction_to_robotics_mechanics_and_control.pdf`
- **Relevância para o TCC:** `9.2/10`
- **Objetivo:** Apresentar os conceitos clássicos da mecânica e controle de robôs manipuladores, com forte ênfase na parametrização Denavit-Hartenberg (D-H) tradicional e na cinemática inversa analítica.
- **Resumo:** O livro de John J. Craig ensina a modelagem cinemática direta usando matrizes de transformação homogênea baseadas na convenção clássica de parâmetros D-H. Também explora a solução fechada (analítica) para a cinemática inversa de robôs industriais comuns (como manipuladores antropomórficos), Jacobianos de velocidade, formulação dinâmica de Lagrange e controle linear desacoplado por junta (PID clássico).
- **Insights:** A cinemática inversa analítica resolvida no livro é fundamental para que o ESP32 S3 calcule, em tempo real e de forma extremamente rápida, os ângulos de junta desejados sem sobrecarregar seu processador dual-core, transmitindo as coordenadas finais das juntas superiores J4-J6 para o Arduino Uno via serial UART.
- **Citação:**
```bibtex
@book{craig2005introduction,
    author    = {John J. Craig},
    title     = {Introduction to Robotics: Mechanics and Control},
    edition   = {3},
    publisher = {Pearson/Prentice Hall},
    year      = {2005},
    isbn      = {9780201543612}
}
```

---

### Título: Robot Manipulators: Modeling, Performance Analysis and Control
- **Arquivo:** `raw/artigos/robot_manipulators_modeling_performance_analysis_control.pdf`
- **Relevância para o TCC:** `8.8/10`
- **Objetivo:** Focar nos métodos de modelagem dinâmica para manipuladores e na análise quantitativa de seu desempenho mecânico, rigidez e estabilidade em malha fechada.
- **Resumo:** A obra de Dombre e Khalil apresenta métodos eficientes para a geração de modelos dinâmicos inversos (Newton-Euler recursivo) e diretos. Discute detalhadamente a calibração geométrica de robôs, a identificação dos parâmetros de atrito e inércia mecânica, e como os erros de modelagem e folgas nas transmissões afetam a precisão de posicionamento na ponta do robô.
- **Insights:** Dá suporte teórico para justificar as calibrações de zero nos encoders AS5600 no EB15. O monitoramento em tempo real via sensores magnéticos em malha fechada ajuda a mitigar e compensar erros de folga (backlash) comuns em transmissões impressas em 3D de baixo custo, conforme discutido pelos autores para mecanismos de baixa rigidez.
- **Citação:**
```bibtex
@book{dombre2007robot,
    author    = {Etienne Dombre and Wisama Khalil},
    title     = {Robot Manipulators: Modeling, Performance Analysis and Control},
    publisher = {ISTE Ltd / John Wiley \& Sons},
    year      = {2007},
    isbn      = {9781848210356}
}
```

---

### Título: Springer Handbook of Robotics
- **Arquivo:** `raw/artigos/springer_handbook_of_robotics.pdf`
- **Relevância para o TCC:** `9.0/10`
- **Objetivo:** Reunir de forma enciclopédica o estado da arte de todas as subáreas da robótica moderna, desde fundamentos matemáticos até aplicações industriais, colaborativas e arquiteturas embarcadas distribuídas.
- **Resumo:** Organizado por Siciliano e Khatib, este handbook conta com contribuições dos maiores pesquisadores do mundo. As partes de maior interesse para o TCC são os capítulos sobre Redes de Sensores/Atuadores, Arquiteturas de Sistemas de Controle Embarcados e Robótica Cooperativa/Rede. Discute a latência inerente de barramentos de comunicação, jitter e determinismo temporal na execução paralela de loops de controle.
- **Insights:** Consolida a robustez científica de se adotar uma arquitetura de hardware mestre-escravo de baixo custo. Auxilia na elaboração do referencial teórico sobre a sincronização temporal na troca de pacotes via UART e o impacto dos atrasos (latency/jitter) nas juntas J1-J3 e J4-J6.
- **Citação:**
```bibtex
@book{siciliano2016springer,
    author    = {Bruno Siciliano and Oussama Khatib},
    title     = {Springer Handbook of Robotics},
    edition   = {2},
    publisher = {Springer-Verlag},
    year      = {2016},
    isbn      = {9783319325521}
}
```

---

### Título: Distributed controller architecture for advanced robot control
- **Arquivo:** `raw/artigos/distributed_controller_architecture_advanced_robot.pdf`
- **Relevância para o TCC:** `9.3/10`
- **Objetivo:** Analisar e demonstrar a viabilidade do uso de uma arquitetura de controle distribuída em microcontroladores de baixo custo para controle robótico avançado, reduzindo a carga de processamento individual.
- **Resumo:** O artigo descreve um sistema de controle onde as tarefas matemáticas pesadas (como planejamento de trajetória cartesiana e cinemática inversa) são desacopladas da geração imediata de pulsos de trigger e monitoramento de segurança dos motores. Mostra que ao particionar tarefas entre nós de processamento dedicados em rede local (como barramentos seriais industriais), obtém-se maior estabilidade temporal, evitando-se o jitter de controle decorrente de multitarefas concorrentes no processador principal.
- **Insights:** Esse artigo serve como principal justificativa teórica para o design de hardware distribuído do EB15. O ESP32 S3, ao gerenciar tarefas de rede (servidor web, RTDE), ficaria propenso a falhas de tempo real crítico na geração de pulsos para os motores de passo. O particionamento de tarefas delegando o controle de movimentação J4-J6 para o Arduino Uno atua diretamente na eliminação desse gargalo.
- **Citação:**
```bibtex
@article{marton2009distributed,
    author  = {L{\H{o}}rinc M{\'a}rton and Katalin M. Hangos and Attila Magyar},
    title   = {Distributed controller architecture for advanced robot control},
    journal = {Proceedings of the IEEE International Conference on Robotics and Biomimetics},
    pages   = {789-794},
    year    = {2009},
    doi     = {10.1109/ROBIO.2009.4913076}
}
```

---

### Título: Master-Slave Synchronization of Robotic Arm using PID Controller
- **Arquivo:** `raw/artigos/master_slave_synchronization_robotic_arm_pid.pdf`
- **Relevância para o TCC:** `9.7/10`
- **Objetivo:** Demonstrar o controle de posição e sincronização em tempo real de um sistema robótico mestre-escravo operando sob controladores PID convencionais em hardware Arduino.
- **Resumo:** O artigo apresenta experimentos práticos e simulações em MATLAB/Simulink de um robô operando em arquitetura mestre-escravo. Os autores modelam matematicamente os motores DC com caixas de redução e testam o comportamento da resposta ao degrau em juntas sob controle PID. Utilizam o protocolo de comunicação serial I2C para conectar o nó mestre ao nó escravo, obtendo atrasos de transmissão da ordem de 3 milissegundos, com alta repetibilidade e erros de regime estacionário mínimos após a sintonização fina dos ganhos do controlador (Kp=1.3, Ki=0.003, Kd=0.005 em simulação; Kp=1, Ki=0.18, Kd=0.05 em hardware).
- **Insights:** Validado experimentalmente que atrasos curtos na comunicação inter-processador (abaixo de 5 ms) viabilizam a sincronização mestre-escravo estável. Para o EB15, que utiliza comunicação UART entre o ESP32 S3 e o Arduino Uno, os dados do artigo ajudam a balizar a banda de sintonia do controle de posição das juntas e a modelagem cinemática distribuída.
- **Citação:**
```bibtex
@article{htun2023master,
    author  = {Yin Hnin Thet Htun and May Su Hlaing and Tin Tin Hla},
    title   = {Master-Slave Synchronization of Robotic Arm using PID Controller},
    journal = {Indonesian Journal of Electrical Engineering and Informatics (IJEEI)},
    volume  = {11},
    number  = {1},
    pages   = {77--87},
    year    = {2023},
    doi     = {10.52549/ijeei.v11i1.4171}
}
```

---

### Título: Optimizing Master-Slave Broadcast Communication in Multi-Node Network Using RS485 Standard
- **Arquivo:** `raw/artigos/optimizing_master_slave_broadcast_communication_rs485.pdf`
- **Relevância para o TCC:** `8.7/10`
- **Objetivo:** Otimizar a eficiência e a latência de sistemas de comunicação broadcast do tipo mestre-escravo sob a especificação física RS485 para aplicações de controle multi-nó de baixa latência.
- **Resumo:** O artigo estuda o gargalo de comunicação em redes distribuídas que operam com pacotes pequenos e frequentes, comuns na telemetria de sensores robóticos. Os autores propõem estruturas de pacotes de dados compactos e um algoritmo dinâmico de priorização de broadcast para minimizar colisões e retardos em redes industriais com restrições severas de tempo real, demonstrando redução drástica na latência e maior confiabilidade de transmissão em ambientes ruidosos.
- **Insights:** Auxilia na fundamentação do protocolo serial customizado do EB15. O ESP32 S3 precisa se comunicar de forma rápida com o Arduino Uno via serial UART. A adoção de pacotes leves, estruturados sem redundâncias desnecessárias e protegidos por algoritmos simples de integridade (como checksum ou CRC simples), é validada por este trabalho para garantir estabilidade e tempos de varredura previsíveis.
- **Citação:**
```bibtex
@article{tanvir2024optimizing,
    author  = {Md Shakil Tanvir and Fiaz Ahmed and Md Monirul Islam and Mainul Islam and Raihan Ur Rashid},
    title   = {Optimizing Master-Slave Broadcast Communication in Multi-Node Network Using RS485 Standard},
    journal = {IEEE International Conference on Computer and Information Technology (ICCIT)},
    year    = {2024},
    doi     = {10.1109/ICCIT64611.2024.11022430}
}
```

---

### Título: A Comprehensive Investigation of ESP32 in Enhancing Wi-Fi Range and Traffic Control
- **Arquivo:** `raw/artigos/comprehensive_investigation_esp32_wifi.pdf`
- **Relevância para o TCC:** `8.9/10`
- **Objetivo:** Analisar o desempenho prático e a robustez do microcontrolador ESP32 em aplicações de rede sem fio, com foco em modos de operação STA (Station) e AP (Access Point), latência de tráfego, jitter e perda de pacotes.
- **Resumo:** Este artigo científico publicado na Defence Science Journal em 2025 avalia quantitativamente o ESP32 sob algoritmos estáticos e dinâmicos de balanceamento de tráfego Wi-Fi. São medidos parâmetros cruciais como taxas de transferência (throughput), tempo de resposta, latência de rede, perda de pacotes e jitter (que caiu de 1.01 ms para 0.80 ms sob controle estático). O trabalho comprova que o hardware dual-core do ESP32 possui excelente estabilidade de conexão de rede, viabilizando transmissões em tempo real confiáveis mesmo sob carregamento de dados.
- **Insights:** Justifica cientificamente o uso do ESP32 S3 como central de conectividade Wi-Fi e servidor web do EB15. Como o sistema operará tanto em modo AP quanto STA para comandos via navegador e integração RTDE, as análises de latência e jitter de Wi-Fi deste artigo respaldam a confiabilidade da nossa arquitetura embarcada.
- **Citação:**
```bibtex
@article{kowsalyadevi2025comprehensive,
    author  = {A. K. Kowsalyadevi and G. Umamaheswari},
    title   = {A Comprehensive Investigation of ESP32 in Enhancing Wi-Fi Range and Traffic Control for Defence Networks},
    journal = {Defence Science Journal},
    volume  = {75},
    number  = {1},
    pages   = {100--110},
    year    = {2025},
    doi     = {10.14429/dsj.75.20284}
}
```

---

### Título: Simultaneous Triggering and Synchronization of Sensors and Onboard Computers
- **Arquivo:** `raw/artigos/simultaneous_triggering_and_synchronization_sensors.pdf`
- **Relevância para o TCC:** `9.1/10`
- **Objetivo:** Propor e validar um sistema versátil e de baixo custo para sincronização temporal de alta precisão entre microcontroladores de tempo real e computadores de bordo, resolvendo a latência e o desalinhamento de dados de sensores.
- **Resumo:** O artigo da NTNU (publicado no arXiv em 2025) utiliza um microcontrolador Teensy 4.1 acoplado a um chip RTC DS3231 para gerar pulsos físicos de trigger síncronos e alinhar relógios internos de computadores de bordo (Khadas Vim4) via protocolo de tempo de alta precisão IEEE 1588 PTP. O sistema mantém desvios inferiores a 100 nanossegundos entre o trigger físico e o carimbo de tempo (timestamp) de software Unix, mitigando completamente o atraso de leitura de múltiplos sensores operando em taxas distintas.
- **Insights:** Valida a lógica de sincronização por trigger de hardware/sinal serial aplicada ao EB15. O ESP32 S3, ao planejar a trajetória cartesiana, calcula as posições futuras e envia as parciais J4-J6 ao Arduino Uno. O sinal de "início" (trigger de sincronismo) para partida simultânea dos motores, conforme concebido em nosso TCC, segue a mesma linha metodológica de paralelismo determinístico recomendada por Nissov et al.
- **Citação:**
```bibtex
@article{nissov2025simultaneous,
    author  = {Morten Nissov and Nikhil Khedekar and Kostas Alexis},
    title   = {Simultaneous Triggering and Synchronization of Sensors and Onboard Computers},
    journal = {arXiv preprint arXiv:2507.05717},
    year    = {2025}
}
```

---

### Título: Research on Closed-loop Control of Step Motor Based on Magnetic Encoder
- **Arquivo:** `raw/artigos/research_closed_loop_control_step_motor_magnetic_encoder.pdf`
- **Relevância para o TCC:** `9.6/10`
- **Objetivo:** Investigar e propor um sistema de controle de malha fechada de baixo custo para motores de passo com base em leituras angulares de alta velocidade fornecidas por encoders magnéticos absolutos.
- **Resumo:** A pesquisa descreve detalhadamente o hardware e software necessários para converter um motor de passo de malha aberta clássico em um sistema servocontrolado de alta precisão usando o sensor magnético absoluto AS5600 (interface I2C/Analógica). O encoder lê a posição real do rotor continuamente, permitindo ao algoritmo do microcontrolador monitorar perda de passo (stalling), corrigir instantaneamente o desvio angular via controle proporcional-integral (PI) de corrente e ajustar dinamicamente a frequência dos pulsos enviados ao driver do motor. Os resultados de ensaios práticos provam uma mitigação total de oscilações mecânicas em baixas velocidades e uma estabilização de torque superior.
- **Insights:** Justifica diretamente a escolha e topologia de hardware do EB15. Em robôs de baixo custo impressos em 3D, a perda de passos devido a sobrecargas e folgas mecânicas é um risco crítico. O artigo valida que a leitura magnética com encoders AS5600 colocados nas juntas do robô é o método mais eficiente e econômico para implementar um sistema robusto de malha fechada em tempo real.
- **Citação:**
```bibtex
@article{tang2022research,
    author  = {Sai Tang and Yuanhua Yu},
    title   = {Research on Closed-loop Control of Step Motor Based on Magnetic Encoder},
    journal = {Journal of Physics: Conference Series},
    volume  = {2200},
    pages   = {012015},
    year    = {2022},
    doi     = {10.1088/1742-6596/2200/1/012015}
}
```

---

### Título: A ROS-Based Framework for Low-Cost Real-Time Haptic Feedback in Human-Robot Teaming
- **Arquivo:** `raw/artigos/a_ros_based_framework_low_cost_haptic.pdf`
- **Relevância para o TCC:** `8.6/10`
- **Objetivo:** Desenvolver uma arquitetura baseada no Robot Operating System (ROS) e microcontroladores Arduino de baixo custo para implementar retorno tátil e teleoperação em tempo real com baixa latência.
- **Resumo:** Os autores propõem um framework modular que integra motores e encoders magnéticos (através do ecossistema Arduino/ROS) para fornecer feedback de força a um operador remoto. Descrevem a arquitetura de software contendo drivers de motores no Arduino Uno comunicando-se via serial com um PC rodando ROS. Utilizam algoritmos simplificados para processar a cinemática direta/inversa em tempo real, atingindo taxas de atualização robustas de controle no barramento de comunicação e provando que hardware de baixíssimo custo é capaz de realizar manipulações cooperativas.
- **Insights:** Fornece as diretrizes teóricas e práticas de comunicação e teleoperação para o EB15. Valida que a arquitetura embarcada baseada em microcontroladores acessíveis é capaz de sustentar loops de controle em tempo real determinísticos quando o software é modularizado adequadamente e o protocolo de serial é otimizado para pequenos payloads de telemetria.
- **Citação:**
```bibtex
@article{okter2025ros,
    author  = {Martin {\O}kter and Filippo Sanfilippo},
    title   = {A ROS-Based Framework for Low-Cost Real-Time Haptic Feedback in Human-Robot Teaming},
    journal = {Proceedings of the IEEE International Conference on Human-Robot Interaction (HRI)},
    year    = {2025},
    doi     = {10.3939/393955603}
}
```

---

### Título: Developing and Analyzing a Novel Multimodal Stroke Rehabilitation System for Stroke Patients
- **Arquivo:** `raw/artigos/developing_analyzing_novel_multimodal_stroke_rehabilitation.pdf`
- **Relevância para o TCC:** `8.0/10`
- **Objetivo:** Desenvolver e analisar clinicamente um sistema de reabilitação robótica multimodal de baixo custo, focado no suporte ao movimento de membros superiores em pacientes que sofreram AVC.
- **Resumo:** Este artigo de 2025 aborda o desenvolvimento de uma órtese/braço robótico articulado que opera de forma assistiva, auxiliando no trajeto mecânico repetitivo de fisioterapia. O sistema utiliza sensores de força e posição angular em tempo real para monitorar os movimentos pretendidos pelo paciente. Seus testes de usabilidade e interface em tempo real demonstram melhoria significativa na recuperação funcional motora quando o robô responde de forma suave, sem solavancos nas juntas (perfil de aceleração controlado).
- **Insights:** Embora o foco seja reabilitação clínica, a usabilidade e segurança mecânica de braços robóticos industriais de baixo custo dependem da suavidade dos movimentos. Este artigo serve como justificativa acadêmica para a aplicação do planejamento de trajetórias com perfis de velocidade contínuos e sem saltos de aceleração no EB15, minimizando estresse mecânico e melhorando a segurança de operação física em robótica colaborativa.
- **Citação:**
```bibtex
@article{alsheikhy2025developing,
    author  = {A. A. Alsheikhy and others},
    title   = {Developing and Analyzing a Novel Multimodal Stroke Rehabilitation System for Stroke Patients},
    journal = {Journal of Diagnostic Research},
    volume  = {14},
    number  = {2},
    pages   = {jdr20250670},
    year    = {2025},
    doi     = {10.7772/jdr20250670}
}
```

---

### Título: S-Curve Trajectory Profiles Deep Dive
- **Arquivo:** `raw/artigos/s_curve_trajectory_profiles_deep_dive.pdf`
- **Relevância para o TCC:** `9.4/10`
- **Objetivo:** Fornecer um guia analítico profundo sobre as equações matemáticas, implementação prática e benefícios de perfis de trajetória em Curva-S em comparação aos perfis trapezoidais tradicionais no controle de movimento industrial.
- **Resumo:** O artigo técnico escrito pela corporação Performance Motion Devices (PMD) explica que, diferentemente do perfil trapezoidal que gera saltos infinitos na aceleração instantânea (causando o fenômeno de solavanco ou jerk), o perfil em Curva-S utiliza uma rampa de aceleração suave com derivada de aceleração constante e finita. Isso reduz significativamente a vibração mecânica residual nas juntas robóticas, preserva as engrenagens e motores contra fadiga e proporciona um movimento notavelmente mais preciso e suave no efetuador final.
- **Insights:** O planejamento de trajetória cartesiana a ser programado no ESP32 S3 para o braço EB15 deve idealmente incorporar a matemática de perfis em Curva-S descrita neste trabalho. Isto evitará que a estrutura impressa em 3D de baixo custo do robô sofra vibrações excessivas e ressonâncias nas juntas J1-J6, garantindo altíssima repetibilidade posicional mesmo em alta velocidade.
- **Citação:**
```bibtex
@article{lewin2023scurve,
    author  = {Chuck Lewin},
    title   = {S-Curve Trajectory Profiles Deep Dive},
    journal = {Performance Motion Devices Tech Library},
    year    = {2023},
    url     = {https://www.pmdcorp.com/resources/type/articles/get/s-curve-profiles-deep-dive-article}
}
```

---

### Título: Micro-ROS Multi-Board Control for a Robotic Leg
- **Arquivo:** `raw/artigos/micro_ros_multi_board_control_for_a_robotic_leg.pdf`
- **Relevância para o TCC:** `8.8/10`
- **Objetivo:** Desenvolver um sistema de controle distribuído em tempo real para uma perna robótica impressa em 3D, minimizando fiação e aumentando a modularidade.
- **Resumo:** O artigo propõe um arranjo de duas placas (Master/Slave) usando micro-ROS para integrar nós em tempo real via protocolo DDS (FreeRTOS). A frequência de amostragem de controle dos motores atinge 1 kHz com baixa latência.
- **Insights:** Valida diretamente a topologia mestre-escravo do EB15, onde o ESP32 S3 gerencia a conectividade superior e o Arduino lida com geração de passos de baixa latência.
- **Citação:**
```bibtex
@article{gomes2026micro,
    author  = {Diogo F. Gomes and others},
    title   = {Micro-ROS Multi-Board Control for a Robotic Leg},
    journal = {IEEE Access},
    year    = {2026}
}
```

---

### Título: Timing Accuracy and Jitter Characterization of ESP32-Based Phase-Angle AC Control: MicroPython vs. Native C
- **Arquivo:** `raw/artigos/timing_accuracy_and_jitter_characterization_esp32.pdf`
- **Relevância para o TCC:** `9.1/10`
- **Objetivo:** Comparar quantitativamente a precisão de temporização e dispersão de jitter entre firmwares em MicroPython e Native C (ESP-IDF) no chip ESP32.
- **Resumo:** Os experimentos demonstram que em Native C os erros de temporização média caíram para -10.3 a 6.1 microssegundos, enquanto no MicroPython ficaram na faixa de 218.2 a 234.7 microssegundos.
- **Insights:** Justifica o desenvolvimento de baixo nível e uso de C++ nativo para os laços críticos de controle de passos e interrupções do ESP32 S3 no braço EB15 para mitigar o jitter mecânico.
- **Citação:**
```bibtex
@article{banuelos2026timing,
    author  = {Luis E. Ba{\~n}uelos Garc{'i}a and Miguel {'A}. Garc{'i}a S{'a}nchez and Eduardo Garc{'i}a Sanchez and others},
    title   = {Timing Accuracy and Jitter Characterization of ESP32-Based Phase-Angle AC Control: MicroPython vs. Native C},
    journal = {Electronics},
    volume  = {15},
    number  = {9},
    pages   = {1970},
    year    = {2026},
    doi     = {10.3390/electronics15091970}
}
```

---

### Título: Analytical Solution for Inverse Kinematics
- **Arquivo:** `raw/artigos/analytical_solution_for_inverse_kinematics.pdf`
- **Relevância para o TCC:** `9.5/10`
- **Objetivo:** Apresentar uma solução geométrica analítica fechada para cinemática inversa de manipuladores seriais de 6-DOF aplicados em robótica espacial.
- **Resumo:** O estudo desenvolve um algoritmo baseado em geometria que resolve a cinemática inversa sem as iterações numéricas clássicas, minimizando o custo computacional da CPU.
- **Insights:** Essencial para o firmware do ESP32 S3 no cálculo em tempo real e de baixo tempo de processamento dos ângulos de base (J1-J3) e do punho transmitidos ao Arduino Uno.
- **Citação:**
```bibtex
@article{kalaycioglu2024analytical,
    author  = {Serdar Kalaycioglu and Anton de Ruiter and Ethan Fung and Harrison Zhang and Haipeng Xie},
    title   = {Analytical Solution for Inverse Kinematics},
    journal = {arXiv preprint arXiv:2410.22582},
    year    = {2024}
}
```

---

### Título: A New Seven-Segment Profile Algorithm for an Open Source Architecture in a Hybrid Electronic Platform
- **Arquivo:** `raw/artigos/a_new_seven_segment_profile_algorithm.pdf`
- **Relevância para o TCC:** `9.3/10`
- **Objetivo:** Propor e testar um algoritmo de geração online de trajetória em Curva-S com aceleração e velocidade limitadas para suavizar movimentos físicos.
- **Resumo:** Desenvolve um perfil de velocidade de sete segmentos que mantém o jerk constante por meio de polinômios de terceiro grau, testado em arquitetura Raspberry Pi/FPGA.
- **Insights:** Serve como fundação algorítmica para o planejamento global de trajetória no ESP32 S3 para evitar acelerações espasmódicas na estrutura impressa em 3D do EB15.
- **Citação:**
```bibtex
@article{garcia2019new,
    author  = {Jos{'e} R. Garc{'i}a-Mart{'i}nez and Juvenal Rodr{'i}guez-Res{'e}ndiz and Edson E. Cruz-Miguel},
    title   = {A New Seven-Segment Profile Algorithm for an Open Source Architecture in a Hybrid Electronic Platform},
    journal = {Electronics},
    volume  = {8},
    number  = {6},
    pages   = {652},
    year    = {2019},
    doi     = {10.3390/electronics8060652}
}
```

---

### Título: Improving Control Performance by Minimizing Jitter in RT-WiFi Networks
- **Arquivo:** `raw/artigos/improving_control_performance_by_minimizing_jitter.pdf`
- **Relevância para o TCC:** `8.7/10`
- **Objetivo:** Minimizar o jitter de rede Wi-Fi em sistemas de controle cooperativos utilizando um network manager para escalonamento determinístico da camada de enlace.
- **Resumo:** Propõe a estrutura S-tree e escalonamento em duas etapas (phasing e período) para eliminar colisões de pacotes e jitter dinâmico na transmissão sem fio de loops de controle.
- **Insights:** Importante para projetar a recepção de dados via endpoints RTDE sob a interface Wi-Fi (AP/STA) do ESP32 S3 sem afetar a estabilidade do movimento físico.
- **Citação:**
```bibtex
@inproceedings{leng2014improving,
    author    = {Quan Leng and Yi-Hung Wei and Song Han and Aloysius K. Mok and Wenlong Zhang and Masayoshi Tomizuka},
    title     = {Improving Control Performance by Minimizing Jitter in RT-WiFi Networks},
    booktitle = {Proceedings of the IEEE Real-Time Systems Symposium (RTSS)},
    year      = {2014},
    doi       = {10.1109/RTSS.2014.33}
}
```

---

### Título: Design and implementation of a 6-DoF robot arm control with object detection based on machine learning using mini microcontroller
- **Arquivo:** `raw/artigos/design_and_implementation_of_a_6_dof_robot_arm_control.pdf`
- **Relevância para o TCC:** `8.9/10`
- **Objetivo:** Descrever o controle e detecção de objetos por Visão Computacional / YOLOv7-tiny embarcado em microcontroladores compactos de recursos limitados (AMB82-Mini).
- **Resumo:** O sistema calcula cinemática e roda redes neurais convolucionais quantizadas na mesma CPU embarcada compacta, permitindo pick-and-place autônomo sem auxílio de PC externo.
- **Insights:** Valida cientificamente a capacidade de processamento de microcontroladores modernos para lidar com controle cinemático complexo simultaneamente a tarefas de conectividade.
- **Citação:**
```bibtex
@article{almaliki2026design,
    author  = {Hayder Hashim Almaliki and Amir Hooshang Mazinan and Seyed Mahmoud Modaresi},
    title   = {Design and implementation of a 6-DoF robot arm control with object detection based on machine learning using mini microcontroller},
    journal = {Scientific Reports},
    year    = {2026}
}
```

---

### Título: Frequency Modulation Based on Hyperbolic Tangent Function for Position Closed Loop Control of Stepper Motor
- **Arquivo:** `raw/artigos/frequency_modulation_based_on_hyperbolic_tangent_function.pdf`
- **Relevância para o TCC:** `9.4/10`
- **Objetivo:** Apresentar um algoritmo de controle de malha fechada sem modelo para motores de passo com base na função de tangente hiperbólica (Tanh).
- **Resumo:** O controlador Tanh modula dinamicamente a frequência e direção de pulsos de passos. Em testes de rastreamento senoidal, reduziu o erro posicional em 60% frente ao controle PI tradicional.
- **Insights:** Útil para o algoritmo de malha fechada das juntas baseadas nos encoders magnéticos AS5600 no ESP32 S3 e no Arduino Uno.
- **Citação:**
```bibtex
@article{zhou2023frequency,
    author  = {Wenlin Zhou and Xin Li and Junzhang Qian and Mo Xia and Yi Pu and Ping Jiang and Jinlong Huang},
    title   = {Frequency Modulation Based on Hyperbolic Tangent Function for Position Closed Loop Control of Stepper Motor},
    journal = {IEEE Access},
    year    = {2023},
    doi     = {10.1109/ACCESS.2023.3291417}
}
```

---

### Título: Sensorless Position Measurement and Blocking Detection Based on Back EMF sensing for Stepper Motor in Next-generation Spectrograph Robots
- **Arquivo:** `raw/artigos/sensorless_position_measurement_and_blocking_detection.pdf`
- **Relevância para o TCC:** `8.5/10`
- **Objetivo:** Detectar travamentos e realizar medições de posição sem encoders mecânicos externos em motores de passo miniaturizados por leitura de Força Eletromotriz Reversa (Back-EMF).
- **Resumo:** O artigo divide as fases do motor de passo para ler a tensão induzida proporcional à rotação mecânica real, minimizando hardware e circuitos integrados adicionais.
- **Insights:** Serve como embasamento conceitual para segurança dinâmica do EB15 caso haja perda de passo nas transmissões ou falha transitória nos barramentos I2C dos encoders.
- **Citação:**
```bibtex
@inproceedings{jenni2014sensorless,
    author    = {Laurent Jenni and Mohamed Bouri and Jean-Paul Kneib and Hannes Bleuler},
    title     = {Sensorless Position Measurement and Blocking Detection Based on Back EMF sensing for Stepper Motor in Next-generation Spectrograph Robots},
    booktitle = {Proceedings of ACTUATOR 2014},
    year      = {2014}
}
```

---

### Título: A Modular and Configurable Architecture for ROS 2 Hardware Integration with micro-ROS
- **Arquivo:** `raw/artigos/a_modular_and_configurable_architecture_for_ros2.pdf`
- **Relevância para o TCC:** `8.6/10`
- **Objetivo:** Propor uma arquitetura modular de integração de hardware entre ROS 2 e microcontroladores embarcados rodando RTOS via micro-ROS.
- **Resumo:** A arquitetura aborda a otimização de payloads, consumo de memória de pilha DDS e sincronismo de relógio local em ambientes com recursos estritos.
- **Insights:** Direciona a estruturação da telemetria de juntas e pacotes de sincronismo caso o ESP32 S3 seja conectado a uma infraestrutura de controle supervisório ROS 2.
- **Citação:**
```bibtex
@thesis{friedl2023modular,
    author = {Jakob Friedl and Markus Bader},
    title  = {A Modular and Configurable Architecture for ROS 2 Hardware Integration with micro-ROS},
    school = {TU Wien},
    year   = {2023}
}
```

---

### Título: Design and control of the Six-DOF robotic manipulator
- **Arquivo:** `raw/artigos/design_and_control_of_the_six_dof_robotic_manipulator.pdf`
- **Relevância para o TCC:** `8.8/10`
- **Objetivo:** Realizar a modelagem mecânica (SolidWorks) e simulações dinâmicas de rastreamento de trajetórias de um braço robótico articulado de seis graus de liberdade.
- **Resumo:** O estudo executa simulações no Matlab/Simulink e análise por elementos finitos (FEA) de fadiga e deformação, validando o rastreamento dinâmico em malha aberta e fechada.
- **Insights:** Útil para embasar as simulações e cálculo de torque limite de juntas mecatrônicas no modelo tridimensional do manipulador EB15.
- **Citação:**
```bibtex
@article{he2024design,
    author  = {Ruohan He and Yinrou Wang and Tingyi Li},
    title   = {Design and control of the Six-DOF robotic manipulator},
    journal = {Applied and Computational Engineering},
    volume  = {78},
    pages   = {129--135},
    year    = {2024},
    doi     = {10.54254/2755-2721/78/20240419}
}
```

---

### Título: Development of Wireless Master-Slave Humanoid Robot Arm
- **Arquivo:** `raw/artigos/development_of_wireless_master_slave_humanoid_robot_arm.pdf`
- **Relevância para o TCC:** `8.3/10`
- **Objetivo:** Construir e programar um par de braços robóticos operando sob sincronização sem fio mestre-escravo por teleoperação com luva sensorial.
- **Resumo:** A luva lê os ângulos das articulações e envia via pacotes RF para o controlador escravo que modula a velocidade e posicionamento dinâmico de servos e motores.
- **Insights:** Traz suporte teórico para sistemas de teleoperação em tempo real com baixa latência e transmissão de ângulos via Wi-Fi do ESP32 S3.
- **Citação:**
```bibtex
@article{abbass2023development,
    author  = {Nourhan A. Abbass and Yousri M. Diab and Eslam H. Mersal and Ahmed M. Hanafi and Abdelrady O. Elnady},
    title   = {Development of Wireless Master-Slave Humanoid Robot Arm},
    journal = {International Journal of Mechanical Engineering},
    year    = {2023}
}
```

---

### Título: Precise timestamping and temporal synchronization in multi-sensor fusion
- **Arquivo:** `raw/artigos/precise_timestamping_and_temporal_synchronization.pdf`
- **Relevância para o TCC:** `8.5/10`
- **Objetivo:** Apresentar técnicas matemáticas de fusão de sensores com carimbo de tempo preciso para contornar atrasos e jitter de redes industriais embarcadas.
- **Resumo:** Discute o desalinhamento geométrico e de medição causados por latências de leitura física de múltiplos sensores e propõe um modelo probabilístico de correção.
- **Insights:** Valida o mecanismo de leitura paralela dos encoders AS5600 no ESP32 e no Arduino Uno, evitando desalinhamentos de telemetria no report dinâmico das 6 juntas.
- **Citação:**
```bibtex
@inproceedings{huck2011precise,
    author    = {Tobias Huck and Antje Westenberger and Martin Fritzsche and Tilo Schwarz and Klaus C. J. Dietmayer},
    title     = {Precise timestamping and temporal synchronization in multi-sensor fusion},
    booktitle = {Proceedings of the IEEE International Conference on Multisensor Fusion and Integration},
    year      = {2011}
}
```

---

### Título: An Online Open-Loop S-Curve Velocity Profile Control Method for Stepping Motors on FPGA
- **Arquivo:** `raw/artigos/an_online_open_loop_s_curve_velocity_profile.pdf`
- **Relevância para o TCC:** `9.2/10`
- **Objetivo:** Gerar online e com baixo custo computacional trajetórias Curva-S de aceleração suave em chips de processamento de hardware (FPGA).
- **Resumo:** O artigo propõe a simplificação algébrica de algoritmos clássicos de Curva-S de 7 segmentos para execução imediata em contadores físicos sem latência.
- **Insights:** Justifica a offloading de tarefas de geração física de passos no Arduino Uno (Slave) e o pré-planejamento no ESP32 S3.
- **Citação:**
```bibtex
@article{chen2018online,
    author  = {Zhong Chen and X. Gao and A. Wang and Z. Liang and X. Zhang},
    title   = {An Online Open-Loop S-Curve Velocity Profile Control Method for Stepping Motors on FPGA},
    journal = {IEEE Transactions on Industrial Electronics},
    year    = {2018}
}
```

---

### Título: A Comparative Analysis of Synchronous USART and Asynchronous UART Communication Protocols
- **Arquivo:** `raw/artigos/a_comparative_analysis_of_synchronous_usart_and_asynchronous_uart.pdf`
- **Relevância para o TCC:** `9.0/10`
- **Objetivo:** Realizar uma análise comparativa profunda de latência, jitter de temporização e robustez mecatrônica entre comunicação UART assíncrona e USART síncrona.
- **Resumo:** Analisa o overhead computacional, jitter e tempos de subida/descida lógicos, mostrando os limites de velocidade serial sem corromper pacotes em microcontroladores.
- **Insights:** Embasamento essencial para justificar a comunicação UART entre o ESP32 S3 e o Arduino Uno no EB15, minimizando atrasos de transmissão e perda de dados.
- **Citação:**
```bibtex
@article{dong2022comparative,
    author  = {Meiting Dong},
    title   = {A Comparative Analysis of Synchronous USART and Asynchronous UART Communication Protocols},
    journal = {Journal of Communications and Network Engineering},
    year    = {2022}
}
```

---

### Título: A Closed-Form Solution to Asymmetric Motion Profile Allowing Acceleration Manipulation
- **Arquivo:** `raw/artigos/a_closed_form_solution_to_asymmetric_motion_profile.pdf`
- **Relevância para o TCC:** `9.1/10`
- **Objetivo:** Propor equações analíticas fechadas para geração de trajetórias Curva-S assimétricas que permitem modular acelerações de forma dinâmica.
- **Resumo:** Desenvolve formulação matemática para S-Curves onde a aceleração de partida e frenagem são independentes, acomodando limites de torque variáveis causados pela gravidade.
- **Insights:** Extremamente valioso para otimização dinâmica do EB15, onde as juntas inferiores sofrem forte influência de gravidade conforme o punho se estende no plano.
- **Citação:**
```bibtex
@article{rew2021closed,
    author  = {K. H. Rew and K. S. Kim},
    title   = {A Closed-Form Solution to Asymmetric Motion Profile Allowing Acceleration Manipulation},
    journal = {IEEE Transactions on Control Systems Technology},
    year    = {2021}
}
```

---

### Título: Distributed real-time control architecture for electrohydraulic humanoid robots
- **Arquivo:** `raw/artigos/distributed_real_time_control_architecture_for_humanoids.pdf`
- **Relevância para o TCC:** `8.4/10`
- **Objetivo:** Desenvolver um barramento e barramento lógico distribuído em tempo real para controle dinâmico desacoplado em robôs humanoides complexos.
- **Resumo:** O sistema particiona tarefas entre um barramento central de planejamento e nós escravos rápidos locais em barramento determinístico, garantindo estabilidade de tempo de ciclo.
- **Insights:** Valida cientificamente a dissociação de processamento no braço robótico EB15, estruturando o fluxo concorrente serial e trigger físico.
- **Citação:**
```bibtex
@article{jleilaty2023distributed,
    author  = {Subhi Jleilaty and Anas Ammounah and Ghiath Abdulmalek},
    title   = {Distributed real-time control architecture for electrohydraulic humanoid robots},
    journal = {Journal of Robotics and Automation},
    year    = {2023}
}
```

---

### Título: Inverse Kinematic Solution of 6-DOF Robot-Arm Based on Dual Quaternions and Axis Invariant Methods
- **Arquivo:** `raw/artigos/inverse_kinematic_solution_of_6_dof_robot_arm.pdf`
- **Relevância para o TCC:** `9.3/10`
- **Objetivo:** Propor formulação por Quaternions Duplos e Invariantes de Eixo para cinemática inversa analítica fechada em manipuladores antropomórficos de 6 eixos.
- **Resumo:** Reduz as singularidades e o tempo computacional de loop de controle de trajetória frente a matrizes clássicas de D-H, permitindo controle em microcontroladores de baixo custo.
- **Insights:** Fornece suporte teórico e matemático alternativo para otimizar os loops de cinemática inversa em tempo real no ESP32 S3 no TCC.
- **Citação:**
```bibtex
@article{ahmed2022inverse,
    author  = {Abubaker Ahmed},
    title   = {Inverse Kinematic Solution of 6-DOF Robot-Arm Based on Dual Quaternions and Axis Invariant Methods},
    journal = {Journal of Robotics Research},
    year    = {2022}
}
```

---

### Título: Closed Loop Stepper Motor Design With Encoder for Stall-Detection
- **Arquivo:** `raw/artigos/closed_loop_stepper_motor_design_with_encoder.pdf`
- **Relevância para o TCC:** `9.5/10`
- **Objetivo:** Investigar e documentar arquitetura e sintonia fina de loops de controle posicional de malha fechada (PI) com detecção instantânea de travamento e stalling.
- **Resumo:** Apresenta esquemas de eletrônica e firmware detalhando as rampas de corrente necessárias para evitar perdas de passo mecânicas sob atrito dinâmico excessivo.
- **Insights:** Importante referência industrial da Texas Instruments para a modelagem dinâmica do feedback com o AS5600 no ESP32 e no Arduino.
- **Citação:**
```bibtex
@techreport{ti2020closed,
    author      = {Texas Instruments Incorporated},
    title       = {Closed Loop Stepper Motor Design With Encoder for Stall-Detection},
    institution = {Texas Instruments},
    year        = {2020},
    number      = {SLAAAD4}
}
```

---

### Título: Evaluating Human-Robot Skill Gaps in Electrical Circuit Inspection: A New Electronic Task Board for Benchmarking Manipulation
- **Arquivo:** `raw/artigos/evaluating_human_robot_skill_gaps.pdf`
- **Relevância para o TCC:** `8.0/10`
- **Objetivo:** Propor e testar uma bancada eletrônica para benchmarking quantitativo de tarefas de manipulação de altíssima repetibilidade e precisão.
- **Resumo:** Avalia o desalinhamento geométrico, torque operacional e tempo de ciclos em tarefas de inspeção mecatrônica fina, criando índices objetivos de acurácia de rastreamento.
- **Insights:** Contribui com metodologia científica para os testes práticos de validação e repetibilidade do efetuador final do braço EB15.
- **Citação:**
```bibtex
@inproceedings{so2025evaluating,
    author    = {Peter So and Abdalla Swikir and Sami Haddadin and Fares J. Abu-Dakka},
    title     = {Evaluating Human-Robot Skill Gaps in Electrical Circuit Inspection: A New Electronic Task Board for Benchmarking Manipulation},
    booktitle = {Proceedings of the IEEE International Conference on Robotics and Automation (ICRA)},
    year      = {2025}
}
```

---

### Título: 3-D Printing of Open Source Appropriate Technologies for Self-Directed Sustainable Development
- **Arquivo:** `raw/artigos/3d_printing_of_open_source_appropriate_technologies.pdf`
- **Relevância para o TCC:** `8.1/10`
- **Objetivo:** Analisar a viabilidade mecânica, resistência estrutural e impacto econômico no desenvolvimento de tecnologias complexas usando impressão 3D aberta.
- **Resumo:** O estudo analisa que componentes fabricados por impressão 3D reduzem o custo mecatrônico drasticamente mantendo boa tolerância física para fins acadêmicos e sustentáveis.
- **Insights:** Justifica a modelagem estrutural mecatrônica do EB15, fabricado em PLA com estrutura 3D aberta de baixíssimo custo.
- **Citação:**
```bibtex
@article{pearce20103d,
    author  = {Joshua M. Pearce and others},
    title   = {3-D Printing of Open Source Appropriate Technologies for Self-Directed Sustainable Development},
    journal = {Journal of Sustainable Development},
    year    = {2010}
}
```

