/*******************************************************
 *  PROJETO: Monitor de Qualidade do Ar com ESP32
 *  SENSORES: DHT22 (temperatura/umidade)
 *            Potenciômetro simulando MQ-135 (gases)
 *  MQTT: Publica dados em "vinicius/monitorar/ar"
 *        Recebe comandos ON/OFF em "vinicius/monitorar/led"
 *******************************************************/

#include "DHT.h"
#include <WiFi.h>
#include <PubSubClient.h>

// -------------------------
// DEFINIÇÃO DE PINOS
// -------------------------
#define DHTPIN 15
#define DHTTYPE DHT22
#define POTPIN 34
#define LEDPIN 2

// -------------------------
// CONFIGURAÇÃO DO WI-FI
// -------------------------
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// -------------------------
// CONFIGURAÇÃO DO BROKER MQTT (HiveMQ Público)
// -------------------------
const char* mqtt_server = "broker.hivemq.com";

// Objetos MQTT e Wi-Fi
WiFiClient espClient;
PubSubClient client(espClient);

// Objeto do sensor DHT
DHT dht(DHTPIN, DHTTYPE);


// ======================================================
// FUNÇÃO: Conectar ao Wi-Fi
// ======================================================
void setup_wifi() {
  delay(10);
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi conectado!");
  Serial.print("IP obtido: ");
  Serial.println(WiFi.localIP());
}


// ======================================================
// CALLBACK - Executa quando chega mensagem MQTT
// ======================================================
void callback(char* topic, byte* message, unsigned int length) {
  String msg;

  for (int i = 0; i < length; i++) {
    msg += (char)message[i];
  }

  Serial.print("Mensagem recebida em [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(msg);

  // Controle do LED por comando MQTT
  if (String(topic) == "vinicius/monitorar/led") {
    if (msg == "ON") {
      digitalWrite(LEDPIN, HIGH);
      Serial.println("LED ligado!");
    } else {
      digitalWrite(LEDPIN, LOW);
      Serial.println("LED desligado!");
    }
  }
}


// ======================================================
// FUNÇÃO: Reconexão ao MQTT
// ======================================================
void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conectar ao broker MQTT... ");

    if (client.connect("ESP32MonitorQualidadeAr")) {
      Serial.println("Conectado!");

      // Assina tópico do LED
      client.subscribe("vinicius/monitorar/led");
      Serial.println("Inscrito no tópico de controle do LED.");
    }
    else {
      Serial.print("Falhou, rc= ");
      Serial.print(client.state());
      Serial.println("... tentando novamente em 5s.");
      delay(5000);
    }
  }
}


// ======================================================
// SETUP PRINCIPAL
// ======================================================
void setup() {
  Serial.begin(115200);

  pinMode(LEDPIN, OUTPUT);
  dht.begin();

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  Serial.println("Monitor de Qualidade do Ar iniciado!");
}


// ======================================================
// LOOP PRINCIPAL
// ======================================================
void loop() {

  // Mantém conexão MQTT ativa
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Leituras do sensor
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();
  int potValue = analogRead(POTPIN);
  float gases = map(potValue, 0, 4095, 0, 100);

  // Impressão no monitor serial
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" °C | Umidade: ");
  Serial.print(umidade);
  Serial.print(" % | Gases: ");
  Serial.print(gases);
  Serial.println(" %");


  // Envio MQTT
  String payload = "{\"temperatura\": " + String(temperatura, 1) +
                   ", \"umidade\": " + String(umidade, 1) +
                   ", \"gases\": " + String(gases, 1) + "}";

  client.publish("vinicius/monitorar/ar", payload.c_str());

  delay(3000);
}
