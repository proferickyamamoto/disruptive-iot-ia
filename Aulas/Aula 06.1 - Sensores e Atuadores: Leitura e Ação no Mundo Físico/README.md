## 📘 Aula 06 - Sensores e Atuadores: Leitura e Ação no Mundo Físico

Nesta aula, vamos explorar sensores que permitem **a leitura do ambiente físico**, como **ultrassom**, **piezoelétrico**, **sensor de presença (PIR)** e **RFID**. Também abordaremos a atuação em resposta às leituras, como acionamento de LEDs, buzzers ou motores.

---

### 🎯 Objetivo
Compreender como sensores capturam dados do ambiente e como atuadores podem responder a esses estímulos, utilizando o Arduino ou ESP32 para integrar essa interação.

### 🔥 Motivação
Sensores são os "olhos" e "ouvidos" do sistema embarcado, enquanto atuadores são as "mãos". Saber utilizá-los de forma eficiente permite a construção de sistemas inteligentes e reativos.

---

### 🔧 Materiais Necessários
- Sensor Ultrassônico HC-SR04
- Sensor Piezoelétrico
- Sensor de Presença PIR
- Módulo RFID RC522 + tags
- LEDs, Buzzer, Servo motor
- Arduino UNO ou ESP32
- Protoboard e jumpers

---

### 📏 Sensor Ultrassônico HC-SR04
Usado para medir distância com base em ondas sonoras.

#### ✅ Passo a Passo
1. Conecte os pinos TRIG e ECHO aos pinos digitais do Arduino.
2. Alimente o sensor com 5V e GND.
3. Carregue o código abaixo.
4. Abra o Serial Monitor para verificar as distâncias medidas.

```cpp
#define TRIG 9 // GPIO 5 para ESP32
#define ECHO 10 // GPIO 18 para ESP32

void setup() {
  Serial.begin(9600); //Não esquecer de alterar para 115200 para ESP32
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

void loop() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);
  float distance = duration * 0.034 / 2;

  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(1000);
}
```

📌 **Explicação:** A função `pulseIn()` mede o tempo de resposta do sinal refletido. A fórmula converte isso em centímetros.

---

### 🎵 Sensor Piezoelétrico
Detecta vibração ou batidas. Ideal para toque, segurança ou percussão.

#### ✅ Passo a Passo
1. Conecte o piezo ao pino analógico A0.
2. Fixe o sensor em uma superfície sólida.
3. Carregue o código e observe o Serial Monitor.

```cpp
int piezoPin = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int value = analogRead(piezoPin);
  if (value > 100) {
    Serial.println("Vibracao detectada!");
  }
  delay(200);
}
```

📌 **Explicação:** A leitura analógica varia com base na vibração. Acima de um limite, considera-se que houve um impacto.

---

### 👁️ Sensor de Presença PIR
Detecta movimento de pessoas baseado em calor (infravermelho passivo).

#### ✅ Passo a Passo
1. Conecte o PIR ao pino digital 2.
2. Alimente com 5V e GND.
3. Carregue o código e movimente-se em frente ao sensor.

```cpp
int pirPin = 2;

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

📌 **Explicação:** O sensor retorna `HIGH` quando detecta calor em movimento (pessoas).

---

### 🆔 Leitor RFID RC522
Permite identificação por radiofrequência com tags ou cartões.

#### ✅ Passo a Passo
1. Conecte o módulo RFID aos pinos SPI do Arduino (10, 11, 12, 13).
2. Carregue a biblioteca `MFRC522` na IDE Arduino.
3. Carregue o código abaixo e aproxime a tag do leitor.

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

  Serial.print("UID da tag: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();
  delay(1000);
}
```

📌 **Explicação:** A leitura UID permite identificar cada tag individualmente. Pode-se armazenar UIDs autorizados.

---

### 🤖 Atividade Prática
1. Meça a distância com o sensor ultrassônico e acione um LED se a distância for menor que 10 cm.
2. Detecte uma vibração com o piezo e acione um buzzer.
3. Com o PIR, acenda um LED por 5 segundos ao detectar movimento.
4. Leia uma tag RFID e exiba seu UID no Serial Monitor.

---

### 🚀 Desafio em Grupo
**Objetivo:** Criar um sistema interativo com sensores e atuadores que simule um **sistema de segurança residencial**:
- Sensor PIR para detectar movimento.
- Sensor ultrassônico para verificar distância de intrusos.
- Leitor RFID para autenticar entrada.
- Piezo para detectar tentativas de violação (batidas ou quedas).
- LED vermelho e buzzer como sinal de alerta.

**Extra:** Armazene os UIDs autorizados e só desative o alarme se o cartão correto for aproximado.

---
Compreender sensores e atuadores amplia as possibilidades de interagir com o mundo físico — a base para automação, segurança e sistemas inteligentes! 🌍📟
