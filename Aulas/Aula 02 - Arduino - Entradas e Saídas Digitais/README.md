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

![Esquemático](LED_BT.txt)

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
```

## 🚀 Como Testar
1. **Monte o circuito** conforme o esquemático.
2. **Carregue o código no Arduino** usando a IDE Arduino.
3. **Pressione o botão** e observe os LEDs piscando de acordo com a lógica implementada.

## 📢 Dicas
- Utilize o monitor serial para depuração se necessário.
- Caso os LEDs não acendam corretamente, verifique a ligação dos pinos.

## 📩 Contato
Dúvidas ou sugestões? Entre em contato com o professor!


