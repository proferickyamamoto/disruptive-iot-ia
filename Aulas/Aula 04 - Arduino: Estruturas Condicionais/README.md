## 📘 Aula 04 - Estruturas Condicionais no Arduino

Nesta aula, aprenderemos a utilizar as principais estruturas condicionais no Arduino: `if-else`, `do-while` e `for`. Essas estruturas são essenciais para o controle do fluxo de execução do código, permitindo decisões automáticas baseadas em condições e a repetição de ações conforme necessário.

---

### 🎯 Objetivo
Compreender e aplicar estruturas condicionais no Arduino para controlar fluxos de execução de código de forma eficiente. Vamos explorar as estruturas `if-else`, `do-while` e `for`, essenciais para a programação de sistemas embarcados.

### 🔥 Motivação
As estruturas condicionais são fundamentais para qualquer projeto em Arduino, pois permitem que o código tome decisões automáticas com base em sensores e variáveis. Com esse conhecimento, será possível criar desde sistemas simples de acionamento de LEDs até projetos avançados de automação e IoT.

---

### 🔧 Materiais Necessários
- **Placa Arduino UNO ou ESP32**
- **LEDs e resistores**
- **Buzzer ativo**
- **Sensor de Luz (LDR)**
- **Botão push-button**
- **Protoboard e jumpers**

---

### 🛠️ Estruturas Condicionais no Arduino

#### 📌 1. Estrutura `if-else`
O `if-else` é utilizado para executar blocos de código com base em condições lógicas.

```cpp
int sensorValue = 0;
void setup() {
    Serial.begin(9600);
}

void loop() {
    sensorValue = analogRead(A0);
    
    if (sensorValue > 500) {
        Serial.println("Valor alto detectado!");
    } else {
        Serial.println("Valor dentro do esperado.");
    }
    delay(1000);
}
```

---

#### 📌 2. Estrutura `do-while`
A estrutura `do-while` executa um bloco de código pelo menos uma vez e continua repetindo enquanto a condição for verdadeira.

```cpp
int contador = 0;

void setup() {
    Serial.begin(9600);
}

void loop() {
    do {
        Serial.print("Contador: ");
        Serial.println(contador);
        contador++;
    } while (contador < 5);
    
    delay(5000); // Aguarda 5 segundos antes de reiniciar
    contador = 0; // Reseta o contador
}
```

---

#### 📌 3. Estrutura `for`
O `for` é usado para executar um bloco de código um número determinado de vezes.

```cpp
void setup() {
    Serial.begin(9600);
}

void loop() {
    for (int i = 0; i < 5; i++) {
        Serial.print("Iteração: ");
        Serial.println(i);
        delay(1000);
    }
}
```

---

### 🚀 Como Testar
1. **Monte o circuito** de acordo com os exemplos fornecidos.
2. **Carregue o código** no Arduino usando a IDE Arduino.
3. **Abra o Monitor Serial** (Ferramentas > Monitor Serial).
4. **Observe os valores** sendo exibidos e verifique o funcionamento correto.

---

### 🎯 Atividade
1. Modifique o código `if-else` para acionar um LED no **pino 13** quando o valor do sensor for maior que 500.
2. Adapte o `do-while` para contar de 10 a 0 ao invés de 0 a 5.
3. Use a estrutura `for` para piscar um LED no **pino 9** cinco vezes.

---

### 🚀 Desafios

#### 🟢 Fácil
- Modifique o código `if-else` para que um **buzzer** toque um som quando o sensor detectar um valor acima de 700.

#### 🔵 Médio
- Utilize um botão no **pino 2** para iniciar um contador `do-while` que exibe os números no Serial Monitor.

#### 🔴 Difícil
- Crie um código que utilize todas as três estruturas condicionais (`if-else`, `do-while` e `for`) para:
  - Ler um sensor de luz (LDR) no **pino A0**.
  - Se a luz ambiente estiver abaixo de um certo nível, acender um LED.
  - Criar um efeito de piscagem controlado por `do-while`.
  - Exibir no **Serial Monitor** os valores lidos a cada ciclo do `for`.

📌 **Dica**: Combine as estruturas para criar um fluxo lógico eficiente!

---
Essa é uma excelente oportunidade para entender como os fluxos condicionais influenciam os sistemas embarcados! 🚀

---
📩 Contato
Dúvidas ou sugestões? Entre em contato com o professor!"
