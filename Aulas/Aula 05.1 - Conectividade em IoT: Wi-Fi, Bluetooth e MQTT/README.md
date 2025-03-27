## 📘 Aula 05 - Conectividade em IoT: Wi-Fi, Bluetooth e MQTT

Nesta aula, exploraremos os principais métodos de **comunicação sem fio** para projetos de **Internet das Coisas (IoT)** usando **ESP32** e **Arduino**. Veremos como utilizar **Wi-Fi**, **Bluetooth** e uma introdução prática ao protocolo **MQTT**, essencial para sistemas IoT modernos.

---

### 🎯 Objetivo
Compreender como conectar dispositivos embarcados à internet ou entre si utilizando Wi-Fi, Bluetooth e o protocolo MQTT para comunicação em IoT.

### 🔥 Motivação
A conectividade é o coração dos sistemas IoT. Com Wi-Fi e Bluetooth, é possível enviar dados para a nuvem, interagir com aplicativos ou trocar informações entre dispositivos. O protocolo MQTT permite uma comunicação leve, confiável e escalável entre dispositivos IoT.

---

### 🔧 Materiais Necessários
- ESP32 (com Wi-Fi e Bluetooth integrados)
- Arduino UNO com módulo ESP8266 (opcional)
- Sensor de temperatura, LDR ou potenciômetro
- Protoboard e jumpers

---

### 📡 Conectividade Wi-Fi com ESP32

#### ✅ Passo a Passo
1. Substitua "SEU_SSID" e "SUA_SENHA" com o nome e senha da sua rede Wi-Fi.
2. Carregue o código no ESP32.
3. Abra o Serial Monitor para verificar se a conexão foi bem-sucedida.
4. Anote o endereço IP exibido para possíveis integrações futuras.

#### 💻 Exemplo: Conectando o ESP32 à rede Wi-Fi
```cpp
#include <WiFi.h>

const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado!");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Seu código aqui
}
```

📌 **Explicação:**
- `WiFi.begin()` inicia a conexão com a rede.
- O laço `while` aguarda até que a conexão seja estabelecida.
- `WiFi.localIP()` retorna o IP atribuído ao ESP32 na rede.

---

### 🔷 Comunicação Bluetooth com ESP32

#### ✅ Passo a Passo
1. Substitua o nome do dispositivo Bluetooth se desejar personalizar.
2. Carregue o código no ESP32.
3. Utilize o celular ou outro dispositivo Bluetooth para parear.
4. Envie caracteres via terminal Bluetooth (como o aplicativo Serial Bluetooth Terminal).

#### 💻 Exemplo: Enviando mensagens via Bluetooth
```cpp
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT"); // Nome do dispositivo Bluetooth
  Serial.println("Bluetooth pronto para pareamento");
}

void loop() {
  if (SerialBT.available()) {
    char c = SerialBT.read();
    Serial.write(c);
  }
  delay(20);
}
```

📌 **Explicação:**
- `BluetoothSerial SerialBT` instancia a comunicação Bluetooth.
- `SerialBT.begin()` define o nome visível ao parear.
- `SerialBT.read()` lê o caractere recebido.

---

### 📡 Introdução ao Protocolo MQTT

#### ✅ Passo a Passo
1. Conecte o ESP32 ao Wi-Fi normalmente.
2. Instale a biblioteca `PubSubClient` pela IDE Arduino.
3. Use um broker público como `broker.hivemq.com` ou `test.mosquitto.org`.
4. Publique dados e teste com ferramentas como MQTT Dash ou MQTT Explorer.

#### 💻 Exemplo com ESP32 (usando biblioteca PubSubClient)
```cpp
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      client.subscribe("topico/teste");
    } else {
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  client.publish("topico/teste", "Ola MQTT");
  delay(2000);
}
```

📌 **Explicação
