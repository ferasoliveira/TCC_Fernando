import os

wiki_file = r"c:\Users\ferna\OneDrive\Área de Trabalho\TCC_Fernando\wiki\artigos.md"

# List of 20 evaluated papers with metadata and BibTeX
evaluations = [
    {
        "title": "Micro-ROS Multi-Board Control for a Robotic Leg",
        "file": "raw/artigos/micro_ros_multi_board_control_for_a_robotic_leg.pdf",
        "relevance": "8.8/10",
        "objective": "Desenvolver um sistema de controle distribuído em tempo real para uma perna robótica impressa em 3D, minimizando fiação e aumentando a modularidade.",
        "summary": "O artigo propõe um arranjo de duas placas (Master/Slave) usando micro-ROS para integrar nós em tempo real via protocolo DDS (FreeRTOS). A frequência de amostragem de controle dos motores atinge 1 kHz com baixa latência.",
        "insights": "Valida diretamente a topologia mestre-escravo do EB15, onde o ESP32 S3 gerencia a conectividade superior e o Arduino lida com geração de passos de baixa latência.",
        "bibtex": """@article{gomes2026micro,
    author  = {Diogo F. Gomes and others},
    title   = {Micro-ROS Multi-Board Control for a Robotic Leg},
    journal = {IEEE Access},
    year    = {2026}
}"""
    },
    {
        "title": "Timing Accuracy and Jitter Characterization of ESP32-Based Phase-Angle AC Control: MicroPython vs. Native C",
        "file": "raw/artigos/timing_accuracy_and_jitter_characterization_esp32.pdf",
        "relevance": "9.1/10",
        "objective": "Comparar quantitativamente a precisão de temporização e dispersão de jitter entre firmwares em MicroPython e Native C (ESP-IDF) no chip ESP32.",
        "summary": "Os experimentos demonstram que em Native C os erros de temporização média caíram para -10.3 a 6.1 microssegundos, enquanto no MicroPython ficaram na faixa de 218.2 a 234.7 microssegundos.",
        "insights": "Justifica o desenvolvimento de baixo nível e uso de C++ nativo para os laços críticos de controle de passos e interrupções do ESP32 S3 no braço EB15 para mitigar o jitter mecânico.",
        "bibtex": """@article{banuelos2026timing,
    author  = {Luis E. Ba{\~n}uelos Garc{\'i}a and Miguel {\'A}. Garc{\'i}a S{\'a}nchez and Eduardo Garc{\'i}a Sanchez and others},
    title   = {Timing Accuracy and Jitter Characterization of ESP32-Based Phase-Angle AC Control: MicroPython vs. Native C},
    journal = {Electronics},
    volume  = {15},
    number  = {9},
    pages   = {1970},
    year    = {2026},
    doi     = {10.3390/electronics15091970}
}"""
    },
    {
        "title": "Analytical Solution for Inverse Kinematics",
        "file": "raw/artigos/analytical_solution_for_inverse_kinematics.pdf",
        "relevance": "9.5/10",
        "objective": "Apresentar uma solução geométrica analítica fechada para cinemática inversa de manipuladores seriais de 6-DOF aplicados em robótica espacial.",
        "summary": "O estudo desenvolve um algoritmo baseado em geometria que resolve a cinemática inversa sem as iterações numéricas clássicas, minimizando o custo computacional da CPU.",
        "insights": "Essencial para o firmware do ESP32 S3 no cálculo em tempo real e de baixo tempo de processamento dos ângulos de base (J1-J3) e do punho transmitidos ao Arduino Uno.",
        "bibtex": """@article{kalaycioglu2024analytical,
    author  = {Serdar Kalaycioglu and Anton de Ruiter and Ethan Fung and Harrison Zhang and Haipeng Xie},
    title   = {Analytical Solution for Inverse Kinematics},
    journal = {arXiv preprint arXiv:2410.22582},
    year    = {2024}
}"""
    },
    {
        "title": "A New Seven-Segment Profile Algorithm for an Open Source Architecture in a Hybrid Electronic Platform",
        "file": "raw/artigos/a_new_seven_segment_profile_algorithm.pdf",
        "relevance": "9.3/10",
        "objective": "Propor e testar um algoritmo de geração online de trajetória em Curva-S com aceleração e velocidade limitadas para suavizar movimentos físicos.",
        "summary": "Desenvolve um perfil de velocidade de sete segmentos que mantém o jerk constante por meio de polinômios de terceiro grau, testado em arquitetura Raspberry Pi/FPGA.",
        "insights": "Serve como fundação algorítmica para o planejamento global de trajetória no ESP32 S3 para evitar acelerações espasmódicas na estrutura impressa em 3D do EB15.",
        "bibtex": """@article{garcia2019new,
    author  = {Jos{\'e} R. Garc{\'i}a-Mart{\'i}nez and Juvenal Rodr{\'i}guez-Res{\'e}ndiz and Edson E. Cruz-Miguel},
    title   = {A New Seven-Segment Profile Algorithm for an Open Source Architecture in a Hybrid Electronic Platform},
    journal = {Electronics},
    volume  = {8},
    number  = {6},
    pages   = {652},
    year    = {2019},
    doi     = {10.3390/electronics8060652}
}"""
    },
    {
        "title": "Improving Control Performance by Minimizing Jitter in RT-WiFi Networks",
        "file": "raw/artigos/improving_control_performance_by_minimizing_jitter.pdf",
        "relevance": "8.7/10",
        "objective": "Minimizar o jitter de rede Wi-Fi em sistemas de controle cooperativos utilizando um network manager para escalonamento determinístico da camada de enlace.",
        "summary": "Propõe a estrutura S-tree e escalonamento em duas etapas (phasing e período) para eliminar colisões de pacotes e jitter dinâmico na transmissão sem fio de loops de controle.",
        "insights": "Importante para projetar a recepção de dados via endpoints RTDE sob a interface Wi-Fi (AP/STA) do ESP32 S3 sem afetar a estabilidade do movimento físico.",
        "bibtex": """@inproceedings{leng2014improving,
    author    = {Quan Leng and Yi-Hung Wei and Song Han and Aloysius K. Mok and Wenlong Zhang and Masayoshi Tomizuka},
    title     = {Improving Control Performance by Minimizing Jitter in RT-WiFi Networks},
    booktitle = {Proceedings of the IEEE Real-Time Systems Symposium (RTSS)},
    year      = {2014},
    doi       = {10.1109/RTSS.2014.33}
}"""
    },
    {
        "title": "Design and implementation of a 6-DoF robot arm control with object detection based on machine learning using mini microcontroller",
        "file": "raw/artigos/design_and_implementation_of_a_6_dof_robot_arm_control.pdf",
        "relevance": "8.9/10",
        "objective": "Descrever o controle e detecção de objetos por Visão Computacional / YOLOv7-tiny embarcado em microcontroladores compactos de recursos limitados (AMB82-Mini).",
        "summary": "O sistema calcula cinemática e roda redes neurais convolucionais quantizadas na mesma CPU embarcada compacta, permitindo pick-and-place autônomo sem auxílio de PC externo.",
        "insights": "Valida cientificamente a capacidade de processamento de microcontroladores modernos para lidar com controle cinemático complexo simultaneamente a tarefas de conectividade.",
        "bibtex": """@article{almaliki2026design,
    author  = {Hayder Hashim Almaliki and Amir Hooshang Mazinan and Seyed Mahmoud Modaresi},
    title   = {Design and implementation of a 6-DoF robot arm control with object detection based on machine learning using mini microcontroller},
    journal = {Scientific Reports},
    year    = {2026}
}"""
    },
    {
        "title": "Frequency Modulation Based on Hyperbolic Tangent Function for Position Closed Loop Control of Stepper Motor",
        "file": "raw/artigos/frequency_modulation_based_on_hyperbolic_tangent_function.pdf",
        "relevance": "9.4/10",
        "objective": "Apresentar um algoritmo de controle de malha fechada sem modelo para motores de passo com base na função de tangente hiperbólica (Tanh).",
        "summary": "O controlador Tanh modula dinamicamente a frequência e direção de pulsos de passos. Em testes de rastreamento senoidal, reduziu o erro posicional em 60% frente ao controle PI tradicional.",
        "insights": "Útil para o algoritmo de malha fechada das juntas baseadas nos encoders magnéticos AS5600 no ESP32 S3 e no Arduino Uno.",
        "bibtex": """@article{zhou2023frequency,
    author  = {Wenlin Zhou and Xin Li and Junzhang Qian and Mo Xia and Yi Pu and Ping Jiang and Jinlong Huang},
    title   = {Frequency Modulation Based on Hyperbolic Tangent Function for Position Closed Loop Control of Stepper Motor},
    journal = {IEEE Access},
    year    = {2023},
    doi     = {10.1109/ACCESS.2023.3291417}
}"""
    },
    {
        "title": "Sensorless Position Measurement and Blocking Detection Based on Back EMF sensing for Stepper Motor in Next-generation Spectrograph Robots",
        "file": "raw/artigos/sensorless_position_measurement_and_blocking_detection.pdf",
        "relevance": "8.5/10",
        "objective": "Detectar travamentos e realizar medições de posição sem encoders mecânicos externos em motores de passo miniaturizados por leitura de Força Eletromotriz Reversa (Back-EMF).",
        "summary": "O artigo divide as fases do motor de passo para ler a tensão induzida proporcional à rotação mecânica real, minimizando hardware e circuitos integrados adicionais.",
        "insights": "Serve como embasamento conceitual para segurança dinâmica do EB15 caso haja perda de passo nas transmissões ou falha transitória nos barramentos I2C dos encoders.",
        "bibtex": """@inproceedings{jenni2014sensorless,
    author    = {Laurent Jenni and Mohamed Bouri and Jean-Paul Kneib and Hannes Bleuler},
    title     = {Sensorless Position Measurement and Blocking Detection Based on Back EMF sensing for Stepper Motor in Next-generation Spectrograph Robots},
    booktitle = {Proceedings of ACTUATOR 2014},
    year      = {2014}
}"""
    },
    {
        "title": "A Modular and Configurable Architecture for ROS 2 Hardware Integration with micro-ROS",
        "file": "raw/artigos/a_modular_and_configurable_architecture_for_ros2.pdf",
        "relevance": "8.6/10",
        "objective": "Propor uma arquitetura modular de integração de hardware entre ROS 2 e microcontroladores embarcados rodando RTOS via micro-ROS.",
        "summary": "A arquitetura aborda a otimização de payloads, consumo de memória de pilha DDS e sincronismo de relógio local em ambientes com recursos estritos.",
        "insights": "Direciona a estruturação da telemetria de juntas e pacotes de sincronismo caso o ESP32 S3 seja conectado a uma infraestrutura de controle supervisório ROS 2.",
        "bibtex": """@thesis{friedl2023modular,
    author = {Jakob Friedl and Markus Bader},
    title  = {A Modular and Configurable Architecture for ROS 2 Hardware Integration with micro-ROS},
    school = {TU Wien},
    year   = {2023}
}"""
    },
    {
        "title": "Design and control of the Six-DOF robotic manipulator",
        "file": "raw/artigos/design_and_control_of_the_six_dof_robotic_manipulator.pdf",
        "relevance": "8.8/10",
        "objective": "Realizar a modelagem mecânica (SolidWorks) e simulações dinâmicas de rastreamento de trajetórias de um braço robótico articulado de seis graus de liberdade.",
        "summary": "O estudo executa simulações no Matlab/Simulink e análise por elementos finitos (FEA) de fadiga e deformação, validando o rastreamento dinâmico em malha aberta e fechada.",
        "insights": "Útil para embasar as simulações e cálculo de torque limite de juntas mecatrônicas no modelo tridimensional do manipulador EB15.",
        "bibtex": """@article{he2024design,
    author  = {Ruohan He and Yinrou Wang and Tingyi Li},
    title   = {Design and control of the Six-DOF robotic manipulator},
    journal = {Applied and Computational Engineering},
    volume  = {78},
    pages   = {129--135},
    year    = {2024},
    doi     = {10.54254/2755-2721/78/20240419}
}"""
    },
    {
        "title": "Development of Wireless Master-Slave Humanoid Robot Arm",
        "file": "raw/artigos/development_of_wireless_master_slave_humanoid_robot_arm.pdf",
        "relevance": "8.3/10",
        "objective": "Construir e programar um par de braços robóticos operando sob sincronização sem fio mestre-escravo por teleoperação com luva sensorial.",
        "summary": "A luva lê os ângulos das articulações e envia via pacotes RF para o controlador escravo que modula a velocidade e posicionamento dinâmico de servos e motores.",
        "insights": "Traz suporte teórico para sistemas de teleoperação em tempo real com baixa latência e transmissão de ângulos via Wi-Fi do ESP32 S3.",
        "bibtex": """@article{abbass2023development,
    author  = {Nourhan A. Abbass and Yousri M. Diab and Eslam H. Mersal and Ahmed M. Hanafi and Abdelrady O. Elnady},
    title   = {Development of Wireless Master-Slave Humanoid Robot Arm},
    journal = {International Journal of Mechanical Engineering},
    year    = {2023}
}"""
    },
    {
        "title": "Precise timestamping and temporal synchronization in multi-sensor fusion",
        "file": "raw/artigos/precise_timestamping_and_temporal_synchronization.pdf",
        "relevance": "8.5/10",
        "objective": "Apresentar técnicas matemáticas de fusão de sensores com carimbo de tempo preciso para contornar atrasos e jitter de redes industriais embarcadas.",
        "summary": "Discute o desalinhamento geométrico e de medição causados por latências de leitura física de múltiplos sensores e propõe um modelo probabilístico de correção.",
        "insights": "Valida o mecanismo de leitura paralela dos encoders AS5600 no ESP32 e no Arduino Uno, evitando desalinhamentos de telemetria no report dinâmico das 6 juntas.",
        "bibtex": """@inproceedings{huck2011precise,
    author    = {Tobias Huck and Antje Westenberger and Martin Fritzsche and Tilo Schwarz and Klaus C. J. Dietmayer},
    title     = {Precise timestamping and temporal synchronization in multi-sensor fusion},
    booktitle = {Proceedings of the IEEE International Conference on Multisensor Fusion and Integration},
    year      = {2011}
}"""
    },
    {
        "title": "An Online Open-Loop S-Curve Velocity Profile Control Method for Stepping Motors on FPGA",
        "file": "raw/artigos/an_online_open_loop_s_curve_velocity_profile.pdf",
        "relevance": "9.2/10",
        "objective": "Gerar online e com baixo custo computacional trajetórias Curva-S de aceleração suave em chips de processamento de hardware (FPGA).",
        "summary": "O artigo propõe a simplificação algébrica de algoritmos clássicos de Curva-S de 7 segmentos para execução imediata em contadores físicos sem latência.",
        "insights": "Justifica a offloading de tarefas de geração física de passos no Arduino Uno (Slave) e o pré-planejamento no ESP32 S3.",
        "bibtex": """@article{chen2018online,
    author  = {Zhong Chen and X. Gao and A. Wang and Z. Liang and X. Zhang},
    title   = {An Online Open-Loop S-Curve Velocity Profile Control Method for Stepping Motors on FPGA},
    journal = {IEEE Transactions on Industrial Electronics},
    year    = {2018}
}"""
    },
    {
        "title": "A Comparative Analysis of Synchronous USART and Asynchronous UART Communication Protocols",
        "file": "raw/artigos/a_comparative_analysis_of_synchronous_usart_and_asynchronous_uart.pdf",
        "relevance": "9.0/10",
        "objective": "Realizar uma análise comparativa profunda de latência, jitter de temporização e robustez mecatrônica entre comunicação UART assíncrona e USART síncrona.",
        "summary": "Analisa o overhead computacional, jitter e tempos de subida/descida lógicos, mostrando os limites de velocidade serial sem corromper pacotes em microcontroladores.",
        "insights": "Embasamento essencial para justificar a comunicação UART entre o ESP32 S3 e o Arduino Uno no EB15, minimizando atrasos de transmissão e perda de dados.",
        "bibtex": """@article{dong2022comparative,
    author  = {Meiting Dong},
    title   = {A Comparative Analysis of Synchronous USART and Asynchronous UART Communication Protocols},
    journal = {Journal of Communications and Network Engineering},
    year    = {2022}
}"""
    },
    {
        "title": "A Closed-Form Solution to Asymmetric Motion Profile Allowing Acceleration Manipulation",
        "file": "raw/artigos/a_closed_form_solution_to_asymmetric_motion_profile.pdf",
        "relevance": "9.1/10",
        "objective": "Propor equações analíticas fechadas para geração de trajetórias Curva-S assimétricas que permitem modular acelerações de forma dinâmica.",
        "summary": "Desenvolve formulação matemática para S-Curves onde a aceleração de partida e frenagem são independentes, acomodando limites de torque variáveis causados pela gravidade.",
        "insights": "Extremamente valioso para otimização dinâmica do EB15, onde as juntas inferiores sofrem forte influência de gravidade conforme o punho se estende no plano.",
        "bibtex": """@article{rew2021closed,
    author  = {K. H. Rew and K. S. Kim},
    title   = {A Closed-Form Solution to Asymmetric Motion Profile Allowing Acceleration Manipulation},
    journal = {IEEE Transactions on Control Systems Technology},
    year    = {2021}
}"""
    },
    {
        "title": "Distributed real-time control architecture for electrohydraulic humanoid robots",
        "file": "raw/artigos/distributed_real_time_control_architecture_for_humanoids.pdf",
        "relevance": "8.4/10",
        "objective": "Desenvolver um barramento e barramento lógico distribuído em tempo real para controle dinâmico desacoplado em robôs humanoides complexos.",
        "summary": "O sistema particiona tarefas entre um barramento central de planejamento e nós escravos rápidos locais em barramento determinístico, garantindo estabilidade de tempo de ciclo.",
        "insights": "Valida cientificamente a dissociação de processamento no braço robótico EB15, estruturando o fluxo concorrente serial e trigger físico.",
        "bibtex": """@article{jleilaty2023distributed,
    author  = {Subhi Jleilaty and Anas Ammounah and Ghiath Abdulmalek},
    title   = {Distributed real-time control architecture for electrohydraulic humanoid robots},
    journal = {Journal of Robotics and Automation},
    year    = {2023}
}"""
    },
    {
        "title": "Inverse Kinematic Solution of 6-DOF Robot-Arm Based on Dual Quaternions and Axis Invariant Methods",
        "file": "raw/artigos/inverse_kinematic_solution_of_6_dof_robot_arm.pdf",
        "relevance": "9.3/10",
        "objective": "Propor formulação por Quaternions Duplos e Invariantes de Eixo para cinemática inversa analítica fechada em manipuladores antropomórficos de 6 eixos.",
        "summary": "Reduz as singularidades e o tempo computacional de loop de controle de trajetória frente a matrizes clássicas de D-H, permitindo controle em microcontroladores de baixo custo.",
        "insights": "Fornece suporte teórico e matemático alternativo para otimizar os loops de cinemática inversa em tempo real no ESP32 S3 no TCC.",
        "bibtex": """@article{ahmed2022inverse,
    author  = {Abubaker Ahmed},
    title   = {Inverse Kinematic Solution of 6-DOF Robot-Arm Based on Dual Quaternions and Axis Invariant Methods},
    journal = {Journal of Robotics Research},
    year    = {2022}
}"""
    },
    {
        "title": "Closed Loop Stepper Motor Design With Encoder for Stall-Detection",
        "file": "raw/artigos/closed_loop_stepper_motor_design_with_encoder.pdf",
        "relevance": "9.5/10",
        "objective": "Investigar e documentar arquitetura e sintonia fina de loops de controle posicional de malha fechada (PI) com detecção instantânea de travamento e stalling.",
        "summary": "Apresenta esquemas de eletrônica e firmware detalhando as rampas de corrente necessárias para evitar perdas de passo mecânicas sob atrito dinâmico excessivo.",
        "insights": "Importante referência industrial da Texas Instruments para a modelagem dinâmica do feedback com o AS5600 no ESP32 e no Arduino.",
        "bibtex": """@techreport{ti2020closed,
    author      = {Texas Instruments Incorporated},
    title       = {Closed Loop Stepper Motor Design With Encoder for Stall-Detection},
    institution = {Texas Instruments},
    year        = {2020},
    number      = {SLAAAD4}
}"""
    },
    {
        "title": "Evaluating Human-Robot Skill Gaps in Electrical Circuit Inspection: A New Electronic Task Board for Benchmarking Manipulation",
        "file": "raw/artigos/evaluating_human_robot_skill_gaps.pdf",
        "relevance": "8.0/10",
        "objective": "Propor e testar uma bancada eletrônica para benchmarking quantitativo de tarefas de manipulação de altíssima repetibilidade e precisão.",
        "summary": "Avalia o desalinhamento geométrico, torque operacional e tempo de ciclos em tarefas de inspeção mecatrônica fina, criando índices objetivos de acurácia de rastreamento.",
        "insights": "Contribui com metodologia científica para os testes práticos de validação e repetibilidade do efetuador final do braço EB15.",
        "bibtex": """@inproceedings{so2025evaluating,
    author    = {Peter So and Abdalla Swikir and Sami Haddadin and Fares J. Abu-Dakka},
    title     = {Evaluating Human-Robot Skill Gaps in Electrical Circuit Inspection: A New Electronic Task Board for Benchmarking Manipulation},
    booktitle = {Proceedings of the IEEE International Conference on Robotics and Automation (ICRA)},
    year      = {2025}
}"""
    },
    {
        "title": "3-D Printing of Open Source Appropriate Technologies for Self-Directed Sustainable Development",
        "file": "raw/artigos/3d_printing_of_open_source_appropriate_technologies.pdf",
        "relevance": "8.1/10",
        "objective": "Analisar a viabilidade mecânica, resistência estrutural e impacto econômico no desenvolvimento de tecnologias complexas usando impressão 3D aberta.",
        "summary": "O estudo analisa que componentes fabricados por impressão 3D reduzem o custo mecatrônico drasticamente mantendo boa tolerância física para fins acadêmicos e sustentáveis.",
        "insights": "Justifica a modelagem estrutural mecatrônica do EB15, fabricado em PLA com estrutura 3D aberta de baixíssimo custo.",
        "bibtex": """@article{pearce20103d,
    author  = {Joshua M. Pearce and others},
    title   = {3-D Printing of Open Source Appropriate Technologies for Self-Directed Sustainable Development},
    journal = {Journal of Sustainable Development},
    year    = {2010}
}"""
    }
]

# Read original wiki
with open(wiki_file, "r", encoding="utf-8") as f:
    original_content = f.read()

# Generate new content
new_content = original_content.rstrip() + "\n\n"

for item in evaluations:
    block = f"""---

### Título: {item['title']}
- **Arquivo:** `{item['file']}`
- **Relevância para o TCC:** `{item['relevance']}`
- **Objetivo:** {item['objective']}
- **Resumo:** {item['summary']}
- **Insights:** {item['insights']}
- **Citação:**
```bibtex
{item['bibtex']}
```

"""
    new_content += block

with open(wiki_file, "w", encoding="utf-8") as f:
    f.write(new_content)

print("Wiki updated successfully!")
