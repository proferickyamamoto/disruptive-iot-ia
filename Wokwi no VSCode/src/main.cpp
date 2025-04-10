/////////--------IOT--------FIAP------------///////////

#include <Arduino.h>

#include <WiFi.h>
#include <ArduinoJson.h>
#include <DHTesp.h>
#include <PubSubClient.h>


// Configurações de WiFi
const char *SSID = "Wokwi-GUEST";
const char *PASSWORD = "";  // Substitua pelo sua senha

// Configurações de MQTT
const char *BROKER_MQTT = "broker.hivemq.com";
const int BROKER_PORT = 1883;
const char *ID_MQTT = "esp32_mqtt";
const char *TOPIC_SUBSCRIBE_LED = "fiap/iot/led";
const char *TOPIC_PUBLISH_TEMP_HUMI = "fiap/iot/temphumi";

// Configurações de Hardware
#define PIN_DHT 12
#define PIN_LED 15
#define PUBLISH_DELAY 2000


// Variáveis globais
WiFiClient espClient;
PubSubClient MQTT(espClient);
DHTesp dht;
unsigned long publishUpdate = 0;
TempAndHumidity sensorValues;
const int TAMANHO = 200;


// Protótipos de funções
void updateSensorValues();
void initWiFi();
void initMQTT();
void callbackMQTT(char *topic, byte *payload, unsigned int length);
void reconnectMQTT();
void reconnectWiFi();
void checkWiFIAndMQTT();

void updateSensorValues() {
  sensorValues = dht.getTempAndHumidity();
}

void initWiFi() {
  Serial.print("Conectando com a rede: ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso: ");
  Serial.println(SSID);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(callbackMQTT);
}

void callbackMQTT(char *topic, byte *payload, unsigned int length) {
  String msg = String((char*)payload).substring(0, length);
  
  //Serial.printf("Mensagem recebida via MQTT: %s do tópico: %s\n", msg.c_str(), topic);

  StaticJsonDocument<TAMANHO> json;
  DeserializationError error = deserializeJson(json, msg);
  
  if (error) {
    Serial.println("Falha na deserialização do JSON.");
    return;
  }

  if (json.containsKey("led")) {
    int valor = json["led"];
    if (valor == 1) {
      digitalWrite(PIN_LED, HIGH);
      Serial.println("LED ligado via comando MQTT");
    } else if (valor == 0) {
      digitalWrite(PIN_LED, LOW);
      Serial.println("LED desligado via comando MQTT");
    }
  }
}

void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Tentando conectar com o Broker MQTT: ");
    Serial.println(BROKER_MQTT);

    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado ao broker MQTT!");
      MQTT.subscribe(TOPIC_SUBSCRIBE_LED);
    } else {
      Serial.println("Falha na conexão com MQTT. Tentando novamente em 2 segundos.");
      delay(2000);
    }
  }
}

void checkWiFIAndMQTT() {
  if (WiFi.status() != WL_CONNECTED) reconnectWiFi();
  if (!MQTT.connected()) reconnectMQTT();
}

void reconnectWiFi(void)
{
  if (WiFi.status() == WL_CONNECTED)
    return;

  WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Wifi conectado com sucesso");
  Serial.print(SSID);
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  dht.setup(PIN_DHT, DHTesp::DHT22);
  initWiFi();
  initMQTT();
}

void loop() {
  checkWiFIAndMQTT();
  MQTT.loop();

  if ((millis() - publishUpdate) >= PUBLISH_DELAY) {
    publishUpdate = millis();
    updateSensorValues();

    if (!isnan(sensorValues.temperature) && !isnan(sensorValues.humidity)) {
      StaticJsonDocument<TAMANHO> doc;
      doc["temperatura"] = sensorValues.temperature;
      doc["umidade"] = sensorValues.humidity;

      char buffer[TAMANHO];
      serializeJson(doc, buffer);
      MQTT.publish(TOPIC_PUBLISH_TEMP_HUMI, buffer);
      Serial.println(buffer);
    }
  }
}









///// --------variação para publicar MQTT apenas se houver mudança do sensor

// // Variáveis globais para armazenar os últimos valores de temperatura e umidade
// float lastTemperature = NAN;
// float lastHumidity = NAN;
      // // Verificar se os novos valores de temperatura e umidade são diferentes dos últimos valores armazenados
      // if (sensorValues.temperature != lastTemperature || sensorValues.humidity != lastHumidity) {

        // // Atualizar os valores armazenados com os novos valores
        // lastTemperature = sensorValues.temperature;
        // lastHumidity = sensorValues.humidity;
