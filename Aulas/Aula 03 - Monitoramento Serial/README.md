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

## 🔍 Explicação da Conversão de Temperatura
A linha de código abaixo realiza a conversão do valor analógico do sensor de temperatura para graus Celsius:

```cpp
float temperatura = (leituraAnalogica * 5.0 / 1023.0) * 100.0;
```

### 📌 Como funciona essa conversão?
1. **`analogRead(SENSOR_TEMP)`**: Retorna um valor entre 0 e 1023, correspondente à leitura do pino analógico.
2. **`(leituraAnalogica * 5.0 / 1023.0)`**: Converte o valor lido para tensão (de 0V a 5V).
3. **`* 100.0`**: O sensor LM35 possui uma relação de 10mV/°C, ou seja, cada 0.01V equivale a 1°C. Multiplicamos por 100 para obter a temperatura real em graus Celsius.

Exemplo:
- Se `leituraAnalogica = 512`, então:
  - `tensão = (512 * 5.0 / 1023.0) ≈ 2.5V`
  - `temperatura = 2.5V * 100 = 25°C`

Isso significa que o código converte corretamente a leitura analógica do sensor para um valor de temperatura utilizável.

## 🖥️ Como Funciona a Função Serial.print
A função `Serial.print()` é usada para enviar dados do Arduino para o computador via comunicação serial, exibindo as informações no **Monitor Serial** da IDE Arduino.

### 📌 Principais Comandos
- **`Serial.print(valor);`** → Exibe um valor no monitor serial **sem** pular para a linha seguinte.
- **`Serial.println(valor);`** → Exibe um valor e pula para a próxima linha.

### 📍 Exemplo
```cpp
Serial.print("Temperatura: ");
Serial.print(temperatura);
Serial.println(" °C");
```
Isso exibirá no Monitor Serial:
```
Temperatura: 25.00 °C
Temperatura: 25.10 °C
Temperatura: 24.90 °C
```
Caso `Serial.println()` fosse removido, os valores apareceriam concatenados na mesma linha.

## 💡 TIPS
> - Certifique-se de que a taxa de transmissão configurada no Monitor Serial corresponde ao valor definido no código (`Serial.begin(9600);`).
> - Caso veja caracteres estranhos no Monitor Serial, tente ajustar a taxa de transmissão para o valor correto.
> - Se o sensor estiver retornando valores inconsistentes, verifique as conexões e a alimentação.
> - Utilize `Serial.print()` para imprimir dados no mesmo formato e `Serial.println()` para pular uma linha e manter a saída organizada.

## 📩 Contato
Dúvidas ou sugestões? Entre em contato com o professor!

