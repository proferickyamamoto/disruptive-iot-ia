# 📘 Aula 02 - Arduino - Entradas e Saídas Digitais

Nesta aula, exploraremos a utilização de entradas e saídas digitais no Arduino. O objetivo é entender como acionar LEDs e ler o estado de um botão, aplicando os conceitos de eletrônica e programação.

## 🔧 Materiais Necessários

- **Placa Arduino UNO**
- **LEDs (Vermelho, Azul, Amarelo)**
- **Resistores de 1kΩ**
- **Protoboard e Jumpers**
- **Botão (Push Button)**

## 🔌 Esquemático do Circuito
O circuito consiste em um botão de entrada e três LEDs conectados ao Arduino. Veja a configuração abaixo:

![Esquemático](Aulas/Aula%2002%20-%20Arduino%20-%20Entradas%20e%20Sa%C3%ADdas%20Digitais/Esquema%20El%C3%A9trico/Circuito_BT_LED.png)

### 📍 Ligações Principais:

- **LED Vermelho:** Pino 3
- **LED Azul:** Pino 8
- **LED Amarelo:** Pino 11
- **Botão:** Pino 9 (com **pull-up interno ativado**)
- **GND comum**

## 💻 Código Fonte

```cpp
// Mapeamento de I/Os
#define LED_YELLOW 11
#define LED_BLUE 8
#define LED_RED 3
#define BT 9

void setup() {
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BT, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(BT) == LOW) {
    piscaLED();
  } else {
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_RED, LOW);
  }
}

void piscaLED() {
  digitalWrite(LED_YELLOW, HIGH);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_RED, HIGH);
  delay(2000);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_BLUE, HIGH);
  digitalWrite(LED_RED, LOW);
  delay(1000);
}
