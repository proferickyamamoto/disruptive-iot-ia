## 📘 Aula 07.1 - Sensores e Atuadores com ESP32: HTTP Server e MQTT

Nesta aula, vamos integrar sensores e atuadores com o ESP32, utilizando duas abordagens para comunicação: um **servidor HTTP** embutido e o **protocolo MQTT**. Você verá como controlar dispositivos e obter dados de sensores pela web e pela nuvem.

---

### 🎯 Objetivo
Aprender a criar um servidor HTTP com ESP32 e utilizar o protocolo MQTT para integrar sensores e atuadores com comunicação em tempo real.

### 🔥 Motivação
Controlar e monitorar dispositivos à distância é um dos principais objetivos da IoT. O ESP32 permite implementar servidores web locais e sistemas conectados com MQTT, facilitando essa integração.

---

### 🔧 Materiais Necessários
- ESP32
- Sensor de temperatura (LM35, DHT11 ou LDR)
- LED, buzzer ou relé como atuador
- Protoboard e jumpers

---

## 🌐 Servidor HTTP com ESP32

### ✅ Passo a Passo (HTTP)
1. Configure o Wi-Fi no ESP32 com seu SSID e senha.
2. Use a biblioteca `WebServer` para criar páginas HTTP.
3. Programe rotas para acionar dispositivos.
4. Acesse o IP local pelo navegador para controlar o ESP32.

### 💻 Exemplo: Controle de LED via navegador
```cpp
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";
WebServer server(80);

int ledPin = 2;

void handleRoot() {
  server.send(200, "text/html", "<h1>ESP32 HTTP Server</h1><a href='/on'>Ligar LED</a><br><a href='/off'>Desligar LED</a>");
}

void handleOn() {
  digitalWrite(ledPin, HIGH);
  server.send(200, "text/plain", "LED Ligado");
}

void handleOff() {
  digitalWrite(ledPin, LOW);
  server.send(200, "text/plain", "LED Desligado");
}

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(1000);

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.begin();
}

void loop() {
  server.handleClient();
}
```

📌 **Explicação:**
- O ESP32 hospeda uma página web.
- Os links executam comandos que controlam o LED.
- IP exibido no Serial Monitor deve ser acessado via navegador.

---

## 📡 Publicando Sensor via MQTT

### ✅ Passo a Passo (MQTT)
1. Use `WiFi.h` para conexão à rede.
2. Use a biblioteca `PubSubClient` para comunicação com MQTT.
3. Configure um broker como `broker.hivemq.com`.
4. Envie leituras do sensor para um tópico MQTT.

### 💻 Exemplo: Envio de dados de sensor para MQTT
```cpp
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

int sensorPin = 34;

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      client.subscribe("topico/comando");
    } else {
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
  int leitura = analogRead(sensorPin);
  char msg[10];
  sprintf(msg, "%d", leitura);
  client.publish("topico/sensor", msg);
  delay(3000);
}
```

📌 **Explicação:**
- O ESP32 publica dados de um sensor em um broker MQTT.
- O tópico pode ser visualizado em apps como MQTT Dash ou MQTT Explorer.

---

### 🎯 Atividade Individual
1. Crie um servidor HTTP que:
   - Mostre a leitura de um sensor (LDR ou temperatura)
   - Permita acionar um LED ou buzzer via botão no navegador
2. Publique a mesma leitura em um broker MQTT.
3. Use um app MQTT para visualizar os dados em tempo real.

---

### 🤝 Projeto em Dupla
**Objetivo:** Desenvolver um sistema híbrido local/remoto com ESP32

**Requisitos:**
- HTTP: Página web que exibe sensor e controla LED
- MQTT: Publicação de dados e recepção de comandos externos

📌 **Dica:** Sincronize LED local (HTTP) e remoto (MQTT) no mesmo código.

---

### 🧠 Desafio Avançado
Monte um **dashboard completo** com:
- 3 sensores (ex: luminosidade, temperatura, vibração)
- 3 atuadores (ex: LED, relé, buzzer)
- Interface web local (HTTP Server)
- Painel remoto (MQTT Dashboard)

📌 Separe os tópicos MQTT por tipo: `/sensor/luz`, `/comando/led`, etc.

---
Esta aula une controle local e remoto com ESP32 — ideal para construir interfaces inteligentes e acessíveis via navegador e nuvem! 🌐📶
