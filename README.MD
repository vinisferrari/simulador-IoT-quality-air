# Monitor IoT de Qualidade do Ar com ESP32 + MQTT + Node-RED

Este projeto consiste em um sistema IoT para monitoramento de **temperatura**, **umidade** e **nível de gases** em ambiente fechado, utilizando um **ESP32**, comunicação via **MQTT**, servidor **Broker MQTT**, processamento lógico no **Node-RED**, e acompanhamento em tempo real via **dashboard web**.

O projeto foi desenvolvido como parte de uma atividade acadêmica envolvendo IoT, computação ubíqua e comunicação por protocolos de internet.

---

## 📡 Visão Geral do Sistema

O sistema é composto por:

- **ESP32** enviando dados de sensores ao broker MQTT  
- **Node-RED** recebendo, processando e tomando decisões  
- **Broker MQTT** (HiveMQ Public Broker)  
- **Dashboard Web** para visualização em tempo real  
- **Atuador (LED)** acionado via comandos MQTT definidos pela lógica no Node-RED  

---

## 🔧 Hardware Utilizado

Descrição detalhada em [`hardware/descricao_hardware.md`](hardware/descricao_hardware.md)

- ESP32 DevKit v1  
- Sensor DHT22  
- Potenciômetro (simulando MQ-135)  
- LED + resistor de 220Ω  
- Protoboard  
- Cabos jumper  

Foto da montagem está em:  
📄 `/hardware/fotos_montagem.png`

---

## 💻 Firmware (ESP32)

Código disponível em:

📁 `/firmware/esp32-air-monitor.ino`

Documentação do código:  
📄 `/firmware/documentacao_codigo.md`

Funções principais:

- Conexão Wi-Fi
- Comunicação MQTT  
- Leitura dos sensores
- Publicação no tópico: `vinicius/monitorar/ar`
- Recebimento de comandos no tópico: `vinicius/monitorar/led`
- Acionamento do LED

---

## 🌐 Node-RED

Fluxo completo em:  
📁 `/nodered/flow.json`

Instruções de uso:  
📄 `/nodered/instrucoes_nodered.md`

Principais funcionalidades:

- Recebe dados do ESP32  
- Processa a lógica:
  - Se gases > 60% **ou** umidade < 30% → LED = **ON**
  - Caso contrário → LED = **OFF**
- Publica no tópico `vinicius/monitorar/led`
- Exibe gráfico em dashboard

---

## 🛰️ Protocolo de Comunicação

O projeto utiliza:

- **TCP/IP**
- **MQTT v3.1.1**
- **QoS 0**
- Broker: *broker.hivemq.com*
- Tópicos:
  - `vinicius/monitorar/ar`
  - `vinicius/monitorar/led`

Documentação detalhada em:  
📄 `/docs/comunicacao_mqtt.md`

---

## 🧪 Simulação Wokwi

Todos os arquivos estão em:  
📁 `/wokwi/`

Simulação executável no navegador:  
🔗 https://wokwi.com/projects/443020517383682049

---

## 📊 Resultados

As imagens da simulaçãoee dashboard estão disponíveis em:  
📁 `/docs/resultados/`

---

## 🎥 Vídeo Demonstrativo

Demonstração completa no YouTube (não listado):  
🔗 https://www.youtube.com/SEU_VIDEO_AQUI

---
## 📃 Repositório

Todo o código-fonte está disponível em:  
🔗 https://www.youtube.com/SEU_VIDEO_AQUI

---

## 📌 Como Reproduzir

1. Clone o repositório  
2. Importe o fluxo no Node-RED  
3. Compile o firmware no ESP32  
4. Configure o Wi-Fi no código  
5. Abra o dashboard em:  
   `http://localhost:1880/ui`  
6. Pronto: você verá a leitura dos sensores e o acionamento automático do LED.

---

## 📄 Licença

Este projeto está licenciado sob a MIT License.

