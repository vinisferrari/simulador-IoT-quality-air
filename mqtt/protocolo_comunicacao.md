# Protocolo de Comunicação MQTT
## Projeto: Monitor IoT de Qualidade do Ar

Este documento descreve a comunicação entre o ESP32, o broker MQTT e o Node-RED, incluindo os tópicos utilizados, o formato das mensagens, a lógica de controle e detalhes sobre QoS, assinaturas e publicações.

---

# 1. Visão Geral do Protocolo MQTT

O protocolo **MQTT (Message Queuing Telemetry Transport)** foi utilizado neste projeto por ser:

- Leve e eficiente (ideal para IoT)
- Baseado em publicação/assinatura (pub/sub)
- Funcionar sobre TCP/IP
- Suportar comunicação bidirecional entre dispositivo e servidor
- Ter ampla compatibilidade com plataformas como Node-RED, HiveMQ e ESP32

O broker utilizado foi o **HiveMQ Public Broker**, disponível gratuitamente em:
```yaml
broker.hivemq.com
Porta: 1883
```

O ESP32 atua como um **client MQTT**, enviando dados e recebendo comandos do Node-RED.

---

# 2. Tópicos Utilizados no Projeto

O sistema utiliza dois tópicos principais:

| Tópico | Direção | Função |
|--------|---------|--------|
| **vinicius/monitorar/ar** | ESP32 → Broker → Node-RED | Envio das leituras de temperatura, umidade e gases |
| **vinicius/monitorar/led** | Node-RED → Broker → ESP32 | Envio de comando ("ON" / "OFF") para acionar o LED atuador |

---

# 3. Fluxo da Comunicação

O comportamento geral do sistema pode ser representado assim:
```yaml
[Sensores no ESP32]
↓
Publica dados em vinicius/monitorar/ar
↓
[Node-RED recebe e processa]
↓
Decide ON/OFF conforme regras
↓
Publica comando em vinicius/monitorar/led
↓
ESP32 recebe o comando e aciona o LED

```

A lógica de controle **não está no firmware**, mas sim no Node-RED, seguindo boas práticas IoT (edge simplificado + backend inteligente).

---

# 4. Formatos de Mensagens

## 4.1. Publicação do ESP32 (Sensores)
Topic:
```
vinicius/monitorar/ar
```

Formato JSON enviado a cada 3 segundos:

```json
{
  "temperatura": 24.5,
  "umidade": 41.2,
  "gases": 37.0
}
```
Valores:

- temperatura — float (°C)

- umidade — float (%)

- gases — float (%) mapeado de 0–100

**4.2. Publicação do Node-RED (Atuador)**

Topic:

```yaml
vinicius/monitorar/led
```
Valores possíveis:

```vbnet
ON
OFF`
```
O ESP32 interpreta esses comandos na função callback().

# 5. Lógica de Processamento (executada no Node-RED)
O Node-RED recebe os valores dos sensores e avalia:

```js
if (gases > 60 || umidade < 30) {
    msg.payload = "ON";
} else {
    msg.payload = "OFF";
}
return msg;
```

Essa lógica é enviada para o tópico do LED.

# 6. QoS Utilizado
Todos os nós MQTT foram configurados com:

```ini
QoS = 0
```
Justificativa:

- Menor latência

- Adequado para monitoramento em tempo real

- Perder um pacote não compromete o funcionamento geral

- HiveMQ aceita QoS 0 sem restrições

# 7. Assinaturas do ESP32
O firmware assina apenas 1 tópico:

```cpp
client.subscribe("vinicius/monitorar/led");
```
A função callback() recebe e interpreta o comando:

```cpp
if (msg == "ON") digitalWrite(LEDPIN, HIGH);
else digitalWrite(LEDPIN, LOW);
```
# 8. Reconexão Automática
O protocolo MQTT implementado no ESP32 possui reconexão:

- Caso Wi-Fi caia → reconecta ao Wi-Fi

- Caso MQTT caia → tenta reconectar a cada 5 segundos

- Após reconectar → reenvia subscribe necessário

Trecho responsável:

```cpp
while (!client.connected()) {
    if (client.connect("ESP32MonitorQualidadeAr")) {
        client.subscribe("vinicius/monitorar/led");
    } else {
        delay(5000);
    }
}
```

# 9. Segurança
Como o broker usado é público, não foi habilitado TLS.
Configurações de segurança incluem:

- ClientID único

- Sem credenciais (broker público)

- Sem retenção de mensagens

- Topics não persistentes

# 10. Ferramentas Utilizadas
- ESP32 + PubSubClient — implementação MQTT no firmware

- HiveMQ Cloud / Public Broker — gerenciamento dos tópicos

- Node-RED + Dashboard — processamento dos dados e interface visual

# 12. Conclusão
O protocolo MQTT implementado atende a todos os requisitos do projeto:

- Comunicação bidirecional confiável

- Baixa latência

- Integração simples com Node-RED

- Estrutura escalável de tópicos

- Organização de mensagens em JSON

- Controle remoto eficiente de atuadores
