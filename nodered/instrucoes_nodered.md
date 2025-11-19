# Instruções de Uso — Node-RED
## Projeto: Monitor IoT de Qualidade do Ar via MQTT

Este documento detalha a configuração, importação e funcionamento do fluxo Node-RED utilizado no projeto de Monitoramento IoT da Qualidade do Ar. O Node-RED atua como o **cérebro lógico do sistema**, recebendo dados do ESP32, processando regras e enviando comandos ao atuador (LED) via MQTT.

---

# 1. Pré-requisitos

Antes de iniciar, é necessário possuir:

### • Node.js instalado (versão LTS)
Download: https://nodejs.org

### • Node-RED instalado
Instalação via npm:
```bash
npm install -g node-red
```
- Dependências do projeto

O painel foi construído utilizando o módulo:

```bash
node-red-dashboard
```
Para instalar manualmente:

```bash
cd ~/.node-red
npm install node-red-dashboard
```
# 2. Executando o Node-RED

Após instalado, o Node-RED pode ser iniciado com:

```bash
node-red
```

O painel administrativo estará disponível em:

```bash
http://localhost:1880
```


O dashboard estará disponível em:

```bash
http://localhost:1880/ui
```

# 3. Importando o Fluxo Completo do Projeto

1. No Node-RED, clique no menu (≡) no canto superior direito.

2. Selecione Import → Clipboard.

3. Cole o conteúdo do fluxo JSON fornecido no repositório:

```bash
/nodered/flow.json
```

4. Clique em Import.

5. Posicione o fluxo onde desejar e clique em Deploy.

O fluxo estará pronto para uso imediatamente após o deploy.

# 4. Arquitetura do Fluxo

O fluxo é composto pelas seguintes etapas principais:

```java
ESP32 → Broker MQTT (HiveMQ) → Node-RED → Dashboard / Controle de LED
```

# 5. Descrição dos Nós Utilizados
#### 5.1. Nó MQTT IN (sensores)

**Tópico assinado:**

```bash
vinicius/monitorar/ar
```


Este nó recebe o JSON enviado pelo ESP32 contendo:

- temperatura

- umidade

- gases

Formato recebido:
```json
{
  "temperatura": 24.8,
  "umidade": 55.1,
  "gases": 12.0
}
```
#### 5.2. Nó JSON

Converte a string recebida em objeto JSON utilizável nos fluxos seguintes.

#### 5.3. Nós de Gauge (Dashboard)

Três gauges exibem valores em tempo real:

- Temperatura (°C)

- Umidade (%)

- Gases (%)

Esses elementos são renderizados automaticamente no painel /ui.

#### 5.4. Nó Function (lógica de decisão)

Código utilizado:

```bash
let temperatura = msg.payload.temperatura;
let umidade = msg.payload.umidade;
let gases = msg.payload.gases;

if (gases > 60 || umidade < 30) {
    msg.payload = "ON";
} else {
    msg.payload = "OFF";
}

return msg;
```

Esse nó:

- Analisa os valores recebidos

- Aplica regras de controle (gases > 60% OU umidade < 30%)

- Decide se o LED deve ser ligado ou desligado

#### 5.5. Nó MQTT OUT (comando para ESP32)

**Tópico publicado:**

```bash
vinicius/monitorar/led
```

O ESP32 está inscrito neste tópico e usa esse comando para acionar o LED.

**Valores enviados:**

- ```"ON"```

- ```"OFF"```

#### 5.6. Nó MQTT IN (feedback do LED)

Escuta novamente o tópico:

```bash
vinicius/monitorar/led
```

Mostra no dashboard o estado atual do atuador.

#### 5.7. Nó UI Text (Status do LED)

Exibe:

```bash
LED: ON
```
ou

```bash
LED: OFF
```

# 6. Dashboard do Projeto

O dashboard inclui:

- Painel de monitoramento com três gauges (temperatura, umidade, gases)

- Indicador textual do status do LED

- Comunicação em tempo real com o ESP32

- Atualizações automáticas a cada 3 segundos

URL:

```bash
http://localhost:1880/ui
```

# 7. Comunicação MQTT — Estrutura Lógica
***Publicação (ESP32 → Node-RED)***

Tópico:

```bash
vinicius/monitorar/ar
```


Conteúdo:

```bash
{
  "temperatura": 23.5,
  "umidade": 42.1,
  "gases": 37.0
}
```

***Assinatura (Node-RED → ESP32)***

Tópico:

```bash
vinicius/monitorar/led
```

Conteúdo:

- ```"ON"```

- ```"OFF"```

# 8. Dependências do Fluxo

O arquivo ```package.json``` do Node-RED incluirá:

```json
{
  "dependencies": {
    "node-red-dashboard": "latest"
  }
}
```

# 9. Possíveis Erros e Soluções
**🔧 MQTT não conecta**

- Verifique se está usando broker.hivemq.com porta 1883

- Certifique-se que não está usando TLS

**🔧 Dashboard não aparece**

- Verifique se há pelo menos 1 UI node ativo

- Confirme se o módulo node-red-dashboard está instalado

**🔧 Gauges não atualizam**

- Certifique-se que o ESP32 está enviando JSON válido

- Teste o broker usando um cliente MQTT (ex: MQTT Explorer)

# 10. Conclusão

O Node-RED desempenha papel fundamental neste projeto:

- Centraliza o processamento das regras de controle

- Interfaceia o ESP32 com o dashboard

- Fornece visualização em tempo real dos sensores

- Atua como controlador final do LED via MQTT