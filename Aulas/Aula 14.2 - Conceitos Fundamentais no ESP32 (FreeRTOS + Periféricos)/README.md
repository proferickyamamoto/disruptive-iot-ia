# 📘 Aula — Conceitos Fundamentais no ESP32 (FreeRTOS + Periféricos)

> Este documento serve como material de apoio para compreender termos comuns ao programar no ESP32 com **FreeRTOS** e lidar com **botões, LEDs e multitarefa**.

---

## 🔂 Debounce

### O que é?
Quando pressionamos um botão físico, os contatos internos **não fecham de maneira limpa**.  
Eles vibram (bouncing), gerando **vários sinais HIGH/LOW em milissegundos**.

- Sem debounce:
```

HIGH ↓↓↑↓↑↓↓↑↓  LOW

```
- Com debounce:
```

HIGH ────────── LOW

````

### Por que precisamos?
Sem debounce, o programa pode interpretar **vários cliques** em vez de apenas um.

### Como resolver?
- **Hardware:** capacitor + resistor (filtro RC).  
- **Software:** descartar mudanças rápidas demais.

### Exemplo em software:
```cpp
int prev = HIGH;
unsigned long lastDebounce = 0;
const unsigned long debounceDelay = 50; // 50ms

void loop() {
  int leitura = digitalRead(25);
  if (leitura != prev) {
    lastDebounce = millis(); // mudança detectada
  }
  if ((millis() - lastDebounce) > debounceDelay) {
    if (leitura == LOW) {
      Serial.println("Clique válido!");
    }
  }
  prev = leitura;
}
````

---

## 🔍 Polling

### O que é?

**Polling** significa ficar verificando (lendo) o estado de um pino em intervalos regulares.

### Analogia

É como ficar olhando para a porta de casa a cada 5 segundos para ver se alguém chegou.

### Exemplo:

```cpp
void loop() {
  if (digitalRead(25) == LOW) {
    Serial.println("Botão pressionado!");
  }
  delay(10); // polling a cada 10ms
}
```

---

## ⏱️ Interrupção

### O que é?

Uma **interrupção** é um mecanismo em que o hardware **avisa automaticamente** o microcontrolador que algo aconteceu, sem precisar de polling.

### Analogia

É como instalar uma campainha: você não precisa checar a porta o tempo todo, a campainha toca quando alguém chega.

### Exemplo no ESP32:

```cpp
void IRAM_ATTR botaoISR() {
  Serial.println("Interrupção: botão pressionado!");
}

void setup() {
  pinMode(25, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(25), botaoISR, FALLING);
}
```

---

## ⚡ ISR (Interrupt Service Routine)

* **ISR** é a função chamada quando ocorre uma interrupção.
* Deve ser **curta e rápida**, pois pausa outras tarefas.
* Não pode usar funções que bloqueiam (ex.: `delay()`).

---

## 🧩 Task (Tarefa no FreeRTOS)

### O que é?

Uma **Task** é como um “mini-programa” que roda em paralelo dentro do ESP32.
O FreeRTOS gerencia várias tasks e decide quando cada uma roda.

### Exemplo:

```cpp
void taskPisca(void *param) {
  for(;;) {
    digitalWrite(2, !digitalRead(2));
    vTaskDelay(pdMS_TO_TICKS(500)); // pausa 500ms
  }
}

void setup() {
  pinMode(2, OUTPUT);
  xTaskCreate(taskPisca, "PiscaLED", 2048, NULL, 1, NULL);
}
```

---

## 🏷️ TaskHandle\_t (Handle de Tarefa)

### O que é?

Um **handle** é uma “referência” para uma task criada.
Com ele, podemos **controlar** a task depois (suspender, retomar, deletar).

### Exemplo:

```cpp
TaskHandle_t hPisca = NULL;

void taskPisca(void *param) {
  for(;;) {
    digitalWrite(2, !digitalRead(2));
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void setup() {
  pinMode(2, OUTPUT);
  xTaskCreate(taskPisca, "Pisca", 2048, NULL, 1, &hPisca);
  // suspende após 3s
  delay(3000);
  vTaskSuspend(hPisca);
}
```

---

## 📦 Queue (Fila do FreeRTOS)

### O que é?

Uma **queue** é usada para passar mensagens entre tasks (inclusive entre **cores diferentes**).

### Analogia

É como uma caixa de correio: uma task coloca mensagens dentro, outra retira.

### Exemplo:

```cpp
QueueHandle_t fila;

void taskSender(void *param) {
  int count = 0;
  for(;;) {
    xQueueSend(fila, &count, portMAX_DELAY);
    count++;
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void taskReceiver(void *param) {
  int valor;
  for(;;) {
    if (xQueueReceive(fila, &valor, portMAX_DELAY) == pdPASS) {
      Serial.print("Recebido: ");
      Serial.println(valor);
    }
  }
}

void setup() {
  Serial.begin(115200);
  fila = xQueueCreate(5, sizeof(int));
  xTaskCreatePinnedToCore(taskSender, "Sender", 2048, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskReceiver, "Receiver", 2048, NULL, 1, NULL, 1);
}
```

---

## 🧠 Núcleos (Core 0 e Core 1)

### O que é?

O ESP32 (clássico) tem **2 núcleos de CPU**:

* **Core 0** → geralmente usado para **Wi-Fi/Bluetooth** + outras tasks.
* **Core 1** → geralmente roda o **loop principal do Arduino**.

Podemos **pinnar tasks** em um core específico ou deixar o FreeRTOS escolher.

### Exemplo:

```cpp
void taskCore0(void *param) {
  for(;;) {
    Serial.print("Task no core ");
    Serial.println(xPortGetCoreID()); // mostra 0 ou 1
    vTaskDelay(1000);
  }
}

void setup() {
  xTaskCreatePinnedToCore(taskCore0, "Core0Task", 2048, NULL, 1, NULL, 0); // Core 0
}
```

---

## 📌 Resumindo os Termos

| Termo             | Significado                                                           |
| ----------------- | --------------------------------------------------------------------- |
| **Debounce**      | Filtrar ruídos de botão (evitar múltiplos cliques falsos).            |
| **Polling**       | Ler o pino periodicamente em loop.                                    |
| **Interrupção**   | O hardware avisa automaticamente quando algo acontece.                |
| **ISR**           | Função que roda quando ocorre a interrupção (rápida).                 |
| **Task**          | Função que roda em paralelo, gerida pelo FreeRTOS.                    |
| **TaskHandle\_t** | Identificador da task para controlá-la (suspender, retomar, deletar). |
| **Queue**         | Estrutura para passar mensagens entre tasks/cores.                    |
| **Core**          | Um dos dois núcleos de processamento do ESP32.                        |

---

## 🧪 Desafio para os alunos

1. Modifique o exemplo da **queue** para que cada clique de botão no Core 0 mande uma mensagem para o Core 1, e o Core 1 registre no Serial quantos cliques já recebeu.
2. Adicione debounce no botão para não contar cliques falsos.
3. Compare o funcionamento com **polling** vs **interrupção** para o mesmo botão.
