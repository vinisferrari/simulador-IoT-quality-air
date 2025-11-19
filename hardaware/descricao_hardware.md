# Descrição do Hardware — Projeto Monitor IoT de Qualidade do Ar
## Monitoramento de Temperatura, Umidade e Gases com ESP32 + MQTT

Este documento apresenta a descrição completa do hardware utilizado no projeto, incluindo sensores, atuadores, plataforma de desenvolvimento, diagrama de conexão e especificações de cada componente utilizado na simulação do sistema IoT.

---

# 1. Visão Geral do Hardware

O sistema desenvolvido utiliza uma arquitetura baseada no microcontrolador **ESP32**, que coleta dados ambientais e os envia para a internet utilizando MQTT. O hardware foi projetado para:

- Medir **temperatura e umidade** utilizando o sensor **DHT22**  
- Simular **nível de gases** utilizando um **potenciômetro ligado ao ADC**
- Controlar um **LED atuador** via comando MQTT
- Comunicar-se via **Wi-Fi (TCP/IP)**
- Alimentar-se diretamente pela porta USB ou alimentação 5V

A arquitetura permite fácil expansão para novos sensores e atuadores, mantendo compatibilidade com ESP32 e protocolos IoT.

---

# 2. Componentes Utilizados

## 2.1. ESP32 DevKit V1
Placa principal do projeto.

### **Funções:**
- Processamento do firmware
- Leitura de sensores
- Acesso à internet via Wi-Fi
- Comunicação MQTT
- Acionamento do LED

### **Características Técnicas:**
- CPU Tensilica LX6 Dual Core 240 MHz
- 520 KB RAM
- Wi-Fi 2.4 GHz (802.11 b/g/n)
- 34 GPIOs (3.3V)
- ADC de 12 bits

---

## 2.2. Sensor DHT22 — Temperatura e Umidade

### Função:
Captura dados ambientais para monitoramento climático.

### Especificações:
- Temperatura: -40 a 80°C (±0,5°C)
- Umidade: 0–100% (±2–5%)
- Comunicação digital (1 fio)
- Tensão: 3.3V ou 5V

### Conexões:
- VCC → 3.3V do ESP32  
- GND → GND  
- DATA → GPIO 15  

---

## 2.3. Potenciômetro (Simulação do Sensor MQ-135)

Como o Wokwi não possui MQ-135 nativamente, utilizou-se um **potenciômetro** para gerar níveis analógicos equivalentes.

### Função:
Simular a concentração de gases (0–100%).

### Especificações:
- Resistência: 10kΩ
- Leitura via ADC do ESP32 (0–4095)
- Conversão para % pelo firmware usando map()

### Conexão:
- Saída do potenciômetro → GPIO 34 (ADC1)

---

## 2.4. LED Atuador

### Função:
Exibir sinalização visual de alarme.

O LED é acionado **somente via comando MQTT**, de acordo com a lógica do Node-RED.

### Conexão:
- Ânodo → GPIO 2
- Catodo → GND

---

# 3. Diagrama de Conexões (Wokwi)

ESP32 DevKit V1
│
├── DHT22 (Temperatura/Umidade)
│ ├─ VCC → 3.3V
│ ├─ GND → GND
│ └─ DATA → GPIO 15
│
├── Potenciômetro (Simulação MQ-135)
│ ├─ VCC → 3.3V
│ ├─ GND → GND
│ └─ Sinal → GPIO 34
│
└── LED Atuador
├─ Ânodo → GPIO 2
└─ Catodo → GND

yaml
Copiar código

---

# 4. Tabela Resumo dos Componentes

| Componente     | Quantidade | Finalidade                                | Alimentação |
|----------------|------------|---------------------------------------------|-------------|
| ESP32 DevKit V1 | 1          | Processamento, Wi-Fi, MQTT e GPIO          | 5V USB / 3.3V |
| DHT22          | 1          | Sensor de temperatura e umidade             | 3.3V |
| Potenciômetro 10kΩ | 1      | Simulação do sensor MQ-135 (gases)         | 3.3V |
| LED            | 1          | Atuador controlado via MQTT                | 3.3V (via GPIO) |

---

# 5. Arquitetura Física do Sistema

O conjunto foi montado virtualmente na ferramenta **Wokwi**, garantindo precisão elétrica e compatibilidade com a plataforma ESP32.

Características da montagem:

- Cabeamento simples e organizado para fácil reprodução
- Todos os sensores conectados diretamente aos pinos GPIO
- Alimentação única fornecida pela USB (5V → regulador interno)
- Circuito totalmente compatível com prototipagem física

---

# 6. Alimentação e Consumo

### Fonte utilizada:
- Alimentação via USB 5V do computador.

### Consumo aproximado:
- ESP32 ativo com Wi-Fi: 80–240 mA
- Sensor DHT22: ~2.5 mA
- LED: 5–20 mA
- Potenciômetro: desprezível

Total estimado: **< 300 mA**

---

# 7. Compatibilidade com Hardware Real

Embora o projeto tenha sido simulado, todo hardware é compatível com montagem real:

✔ DHT22 funciona na mesma pinagem real  
✔ Potenciômetro funciona em qualquer ADC1 do ESP32  
 ✔ LED funciona no GPIO 2 sem restrições  
✔ MQTT e Wi-Fi operam da mesma forma no dispositivo físico  

---

# 8. Possíveis Expansões

O hardware foi planejado para permitir expansão futura:

- Substituição do potenciômetro pelo **sensor MQ-135 real**
- Inclusão de:
  - Buzzer de alerta
  - Display LCD ou OLED
  - Múltiplos sensores ambientais
- Caixa 3D impressa para encapsulamento
- Alimentação por fonte externa + bateria Li-ion

---

# 9. Conclusão

A configuração de hardware utilizada neste projeto atende completamente aos requisitos de monitoramento IoT:

- Simples de montar  
- Totalmente funcional em simulação e hardware real  
- Compatível com protocolos de internet (Wi-Fi + MQTT)  
- Modular e expansível  

O conjunto proporciona uma base sólida para aplicações de monitoramento ambiental e automação residencial.