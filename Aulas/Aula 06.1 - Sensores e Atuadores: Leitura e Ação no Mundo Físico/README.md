## 📘 Aula 06.1 - Sensores e Atuadores: Leitura e Ação no Mundo Físico

Nesta aula, exploraremos sensores que permitem **a leitura do ambiente físico**, como **ultrassom**, **piezoelétrico**, **sensor de presença (PIR)** e **RFID**, utilizando tanto o **Arduino UNO** quanto o **ESP32**. Também abordaremos como atuar com LEDs, buzzers ou relés em resposta às leituras.

---

### 🎯 Objetivo
Compreender como sensores capturam dados do ambiente e como atuadores podem responder a esses estímulos, utilizando tanto o Arduino quanto o ESP32.

### 🔥 Motivação
Sensores são os "olhos" e "ouvidos" dos sistemas embarcados, e atuadores representam suas "ações" no mundo físico. Com Arduino e ESP32, conseguimos desenvolver soluções versáteis de automação e monitoramento.

---

### 🔧 Materiais Necessários
- Sensor Ultrassônico HC-SR04
- Sensor Piezoelétrico
- Sensor de Presença PIR
- Módulo RFID RC522 + tags
- LED, Buzzer, Relé
- Arduino UNO e/ou ESP32
- Protoboard e jumpers

---

## 🧪 Sensor Ultrassônico HC-SR04
### ➤ Arduino UNO
```cpp
#define TRIG 9
#define ECHO 10

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

void loop() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duracao = pulseIn(ECHO, HIGH);
  float distancia = duracao * 0.034 / 2;

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
  delay(1000);
}
```

### ➤ ESP32
Use os pinos GPIO compatíveis, como 5 e 18.
```cpp
#define TRIG 5
#define ECHO 18

void setup() {
  Serial.begin(115200);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

void loop() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duracao = pulseIn(ECHO, HIGH);
  float distancia = duracao * 0.034 / 2;

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
  delay(1000);
}
```

---

## 🥁 Sensor Piezoelétrico
### ➤ Arduino UNO / ESP32 (sem alteração)
```cpp
int piezoPin = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int leitura = analogRead(piezoPin);
  if (leitura > 100) {
    Serial.println("Vibracao detectada!");
  }
  delay(200);
}
```
*Para o ESP32, use uma entrada analógica como GPIO 34, 35 ou 36.*

---

## 👁️ Sensor PIR (Presença)
### ➤ Arduino UNO / ESP32
```cpp
int pirPin = 2; // Pode ser outro pino digital para ESP32

void setup() {
  pinMode(pirPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  int estado = digitalRead(pirPin);
  if (estado == HIGH) {
    Serial.println("Movimento detectado!");
  } else {
    Serial.println("Sem movimento");
  }
  delay(1000);
}
```

---

## 🆔 Leitor RFID RC522
### ➤ Arduino UNO
Conexões SPI padrão:
- SDA → 10  | SCK → 13
- MOSI → 11 | MISO → 12
- RST → 9

```cpp
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

  Serial.print("UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();
  delay(1000);
}
```

### ➤ ESP32
Para o ESP32, use:
- SDA → 21, SCK → 18, MOSI → 23, MISO → 19, RST → 22
E adapte no código os pinos no `MFRC522`.

```cpp
#define SS_PIN 21
#define RST_PIN 22
MFRC522 rfid(SS_PIN, RST_PIN);
```

---

### 🤖 Atividade Prática
1. Meça a distância com o sensor ultrassônico e acione um LED se for menor que 10 cm.
2. Detecte uma batida com o piezo e acione um buzzer.
3. Acenda um LED por 5 segundos com o PIR.
4. Mostre o UID de uma tag RFID no Serial Monitor.

---

### 🚀 Desafio em Grupo
**Objetivo:** Criar um sistema de segurança residencial:
- PIR detecta presença
- Ultrassom mede aproximação
- RFID valida entrada
- Piezo detecta impacto
- LED e buzzer indicam alarme

**Extra:** Só desligar o alarme com tag autorizada no leitor.

---

Essa aula permite explorar sensores e atuadores em dois ambientes distintos — ideal para aprender portabilidade e adaptar soluções de automação em diferentes plataformas! 🛠️📶

