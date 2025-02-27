# 📘 Aula 03 - Arduino - Monitoramento Serial

Nesta aula, aprenderemos a utilizar a comunicação serial do Arduino para monitorar sensores e depurar códigos. O monitor serial é uma ferramenta essencial para visualizar dados enviados pelo Arduino ao computador.

## 🔧 Materiais Necessários

- **Placa Arduino UNO**
- **Sensor de Temperatura (NTC ou LM35)**
- **Resistor de 10kΩ (se necessário)**
- **Protoboard e Jumpers**

## 🔌 Esquemático do Circuito
O circuito consiste na leitura de um sensor de temperatura e exibição dos valores no monitor serial da IDE Arduino.

### 📍 Ligações Principais:

- **Sensor LM35:**
  - VCC → 5V (Arduino)
  - GND → GND (Arduino)
  - OUT → A0 (Arduino)

## 💻 Código Fonte

```cpp
// Definição do pino do sensor
#define SENSOR_TEMP A0

void setup() {
  Serial.begin(9600); // Inicia a comunicação serial com baudrate de 9600
  Serial.println("Monitoramento Serial Iniciado");
}

void loop() {
  int leituraAnalogica = analogRead(SENSOR_TEMP);
  float temperatura = (leituraAnalogica * 5.0 / 1023.0) * 100.0;
  
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");
  
  delay(1000); // Aguarda 1 segundo antes de próxima leitura
}
```

## 🚀 Como Testar
1. **Monte o circuito** conforme o esquemático.
2. **Carregue o código no Arduino** usando a IDE Arduino.
3. **Abra o Monitor Serial** (Ferramentas > Monitor Serial).
4. **Acompanhe os valores de temperatura** em tempo real.

## 📢 O que é Baud Rate?
O **baud rate** define a velocidade de transmissão dos dados na comunicação serial. No código acima, o baud rate é definido como `9600`, que significa que a taxa de transmissão é de 9600 bits por segundo (bps). Esse valor é um dos mais comuns para comunicação serial, pois garante uma boa estabilidade na transmissão de dados entre o Arduino e o computador. Outras taxas, como 115200 bps, podem ser utilizadas dependendo da necessidade do projeto.

Caso o baud rate definido no código não corresponda ao baud rate configurado no **Monitor Serial**, os dados exibidos poderão aparecer corrompidos ou ilegíveis. Certifique-se de selecionar `9600 baud` no Monitor Serial para correta exibição das informações.

## 📩 Contato
Dúvidas ou sugestões? Entre em contato com o professor!


