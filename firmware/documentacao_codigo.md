📄 documentacao_codigo.md
# Documentação do Código — Firmware ESP32
## Projeto: Monitor IoT de Qualidade do Ar via MQTT

Este documento descreve detalhadamente o funcionamento do código desenvolvido para o ESP32 utilizado no projeto de monitoramento de qualidade do ar, incluindo a organização das funções, responsabilidades de cada módulo, explicação das bibliotecas utilizadas e o fluxo de comunicação MQTT.

---

# 1. Introdução

O firmware desenvolvido tem como objetivo:

- Capturar dados ambientais através do sensor **DHT22** (temperatura e umidade).
- Simular o nível de gases utilizando um **potenciômetro ligado ao ADC** do ESP32.
- **Publicar periodicamente** os dados no broker MQTT.
- **Receber comandos MQTT** para ligar ou desligar um LED atuador.
- Integrar-se ao Node-RED, onde a lógica de controle é processada.

A comunicação ocorre via **Wi-Fi + MQTT**, atendendo aos requisitos do projeto de IoT e computação ubíqua.

---

# 2. Bibliotecas Utilizadas

### **WiFi.h**
Responsável pela conexão do ESP32 à rede sem fio (protocolo TCP/IP).

### **PubSubClient.h**
Gerencia a comunicação MQTT (conexão, subscribe, publish e callback).

### **DHT.h**
Leitura de temperatura e umidade do sensor DHT22.

---

# 3. Declaração de Pinos e Constantes

```cpp
#define DHTPIN 15
#define DHTTYPE DHT22
#define POTPIN 34
#define LEDPIN 2
```

DHTPIN 15 → pino digital onde o DHT22 está conectado

POTPIN 34 → entrada analógica (ADC1) para leitura do potenciômetro

LEDPIN 2 → pino onde está conectado o LED atuador

Para o MQTT e Wi-Fi:

```cpp
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";
```

# 4. Inicialização dos Objetos

```cpp
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);
```


WiFiClient → camada TCP/IP base usada pelo MQTT

PubSubClient → cliente MQTT

DHT → objeto para leitura do sensor DHT22

# 5. Função setup_wifi()

Responsável pela conexão do ESP32 à rede sem fio.

Funções internas:

- WiFi.begin(ssid, password);

- Laço while aguardando WL_CONNECTED

- Impressão do endereço IP obtido via DHCP

Essa função garante que o ESP32 só continue após estar conectado à rede.

# 6. Função callback()

Executa automaticamente quando uma mensagem MQTT chega em um dos tópicos inscritos.

```cpp

if (String(topic) == "vinicius/monitorar/led") {
    if (msg == "ON") digitalWrite(LEDPIN, HIGH);
    else digitalWrite(LEDPIN, LOW);
}

```

Responsabilidades:

- Interpretar a mensagem recebida no tópico de controle

- Atualizar o estado do LED

- Registrar no monitor serial o comando recebido

Importante: O firmware não decide sozinho se o LED liga ou não; a decisão é tomada no Node-RED.

# 7. Função reconnect()

Garante que o ESP32 reconecte ao broker MQTT sempre que a conexão cair.

A rotina:

1. Tenta conectar ao broker usando um clientID.

2. Se falhar, espera 5 segundos.

3. Após conectar, assina o tópico de controle do LED:

```cpp
client.subscribe("vinicius/monitorar/led");
```

# 8. Função setup()

Configura o sistema ao iniciar:

- Inicializa monitor serial

- Define LEDPIN como saída
 
- Inicia sensor DHT22
 
- Conecta ao Wi-Fi
 
- Configura broker MQTT
 
- Define função de callback
 
- Também imprime mensagem informando que o monitor foi iniciado.

# 9. Função loop()

Executa continuamente e contém o fluxo principal do firmware:

1. Mantém MQTT conectado
```cpp
if (!client.connected()) {
    reconnect();
}
client.loop();
```

2. Captura leituras dos sensores
```cpp
float temperatura = dht.readTemperature();
float umidade = dht.readHumidity();
int potValue = analogRead(POTPIN);
float gases = map(potValue, 0, 4095, 0, 100);
```


- O potenciômetro é convertido para uma escala de 0 a 100%, simulando o MQ-135.

3. Publica os dados no MQTT

```cpp
String payload = "{\"temperatura\": ..., \"umidade\": ..., \"gases\": ...}";
client.publish("vinicius/monitorar/ar", payload.c_str());
```

O envio ocorre a cada 3 segundos.

4. O LED não é acionado localmente no ESP32

Toda lógica de decisão ocorre no Node-RED:

- Node-RED processa os sensores

- Decide o estado "ON" ou "OFF"

- Publica no tópico vinicius/monitorar/led

ESP32 recebe e executa

# 10. Fluxo Geral do Sistema
```cpp
+---------------+        +------------------+        +----------------+
|   ESP32       | -----> |   Broker MQTT    | -----> |   Node-RED     |
| (Sensores)    |        | (HiveMQ)         |        | (Processamento)|
+---------------+        +------------------+        +----------------+
        ^                                                         |
        |                                                         |
        +---------------- MQTT (led topic) -----------------------+

```

# 11. Tópicos Utilizados
Publicação (ESP32 → MQTT → Node-RED)

```cpp
vinicius/monitorar/ar
```


Formato:

```json
{
  "temperatura": 25.1,
  "umidade": 48.2,
  "gases": 31.0
}

```

Assinatura (Node-RED → MQTT → ESP32)

```cpp
vinicius/monitorar/led
```


Valores possíveis:

- ```"ON"```

- ```"OFF"```

# 12. Considerações Finais

O firmware foi construído seguindo boas práticas para sistemas IoT:

- Lógica de decisão descentralizada (processamento externo)

- Comunicação assíncrona via MQTT

- Reconexão automática e robusta

- Modularidade entre firmware, broker e plataforma

- Formato JSON estruturado e compatível com múltiplas plataformas

- Este código é totalmente funcional tanto no Wokwi quanto em hardware real.