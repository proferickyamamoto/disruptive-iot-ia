# 📘 Aula — Trabalhando com os Núcleos do ESP32 (Dual-Core)

> **Objetivo da Aula:**  
> Entender como o ESP32 pode executar **duas tarefas em paralelo**, uma em cada núcleo (Core 0 e Core 1), utilizando **FreeRTOS**.  
> Os alunos vão implementar duas *tasks* simples: cada uma pisca um LED em um intervalo diferente e imprime o **CoreID** em execução no **Serial Monitor**.

---

## 🎯 Metas de Aprendizagem
- Criar **tarefas FreeRTOS** no ESP32.  
- Fixar cada tarefa em um núcleo específico com `xTaskCreatePinnedToCore`.  
- Usar `xPortGetCoreID()` para verificar em qual **Core** a tarefa está rodando.  
- Observar a execução paralela com **LEDs piscando em ritmos diferentes**.  
- Experimentar **troca de núcleos** e **mudança de prioridades**.

---

## 🔌 Materiais Necessários
- 1x ESP32 DevKit (WROOM/WROVER)  
- 2x LEDs + 2 resistores 220 Ω (ou use o LED onboard no GPIO2)  
- Protoboard e jumpers

### Ligações
- **LED_CORE0** → **GPIO4** → resistor 220 Ω → LED → GND  
- **LED_CORE1** → **GPIO2** (LED onboard em muitas placas, pode usar externo se preferir)

---

## 🧩 Código Completo (Arduino/PlatformIO)

### Arquivo: `platformio.ini`
```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
build_unflags = -std=gnu++11
build_flags = -std=gnu++17
````

### Arquivo: `src/main.cpp`
```.cpp
/*
  Demo Dual-Core (ESP32) com Botões por Task — Código Comentado
  --------------------------------------------------------------
  Objetivo:
    - Demonstrar execução em núcleos distintos (Core 0 e Core 1) com FreeRTOS.
    - Cada task pisca um LED em período diferente.
    - Dois botões (com debounce por polling) suspendem/retomam cada task.

  Botões:
    - BTN0 -> alterna (suspende/retoma) taskCore0 (Core 0)
    - BTN1 -> alterna (suspende/retoma) taskCore1 (Core 1)

  O que observar:
    - Ao suspender uma task, o LED correspondente para de piscar.
    - A outra task segue ativa, mostrando independência entre núcleos.
*/

#include <Arduino.h>

// ----------------- Pinos -----------------
constexpr gpio_num_t LED_CORE0 = GPIO_NUM_4; // LED da task no Core 0
constexpr gpio_num_t LED_CORE1 = GPIO_NUM_2; // LED da task no Core 1 (onboard em muitas placas)

constexpr gpio_num_t BTN0 = GPIO_NUM_25;     // Botão para controlar taskCore0
constexpr gpio_num_t BTN1 = GPIO_NUM_26;     // Botão para controlar taskCore1

// ----------------- Protótipos -----------------
void taskCore0(void*);   // pisca LED_CORE0 no Core 0
void taskCore1(void*);   // pisca LED_CORE1 no Core 1
void taskButtons(void*); // faz polling com debounce e suspende/retoma tasks

// ----------------- Handles das Tasks -----------------
TaskHandle_t hTask0 = nullptr;
TaskHandle_t hTask1 = nullptr;

// Estados de suspensão (apenas para log e decisão de resume/suspend)
volatile bool isSuspended0 = false;
volatile bool isSuspended1 = false;

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("\n[Dual-core + Buttons] Iniciando...");

  // LEDs como saída
  pinMode(LED_CORE0, OUTPUT);
  pinMode(LED_CORE1, OUTPUT);
  digitalWrite(LED_CORE0, LOW);
  digitalWrite(LED_CORE1, LOW);

  // Botões com pull-up interno; botão pressionado -> nível LOW
  pinMode(BTN0, INPUT_PULLUP);
  pinMode(BTN1, INPUT_PULLUP);

  // --------- Criação das Tasks Pinned por Núcleo ---------

  // Task do Core 0 (pisca rápido)
  xTaskCreatePinnedToCore(
    taskCore0,
    "taskCore0",
    2048,
    nullptr,
    2,          // prioridade
    &hTask0,    // guardamos o handle para suspender/retomar
    0           // <-- pinada no Core 0
  );

  // Task do Core 1 (pisca mais lento)
  xTaskCreatePinnedToCore(
    taskCore1,
    "taskCore1",
    2048,
    nullptr,
    2,
    &hTask1,
    1           // <-- pinada no Core 1
  );

  // Task de leitura de botões (pode rodar em qualquer core; vamos de Core 0)
  xTaskCreatePinnedToCore(
    taskButtons,
    "taskButtons",
    2048,
    nullptr,
    3,          // prioridade ligeiramente maior para reagir bem ao clique
    nullptr,
    0
  );
}

void loop() {
  // Não usado; tudo roda nas tasks.
  vTaskDelay(pdMS_TO_TICKS(1000));
}

// ----------------- Implementações -----------------

// Task no Core 0: pisca LED_CORE0 a cada 250 ms
void taskCore0(void*){
  const TickType_t period = pdMS_TO_TICKS(250);
  TickType_t last = xTaskGetTickCount();

  for(;;){
    // Exibe em qual core está executando (deve ser 0)
    Serial.print("[taskCore0] CoreID=");
    Serial.print(xPortGetCoreID());
    Serial.print("  Suspenso? ");
    Serial.println(isSuspended0 ? "SIM":"NAO");

    // Toggle do LED
    digitalWrite(LED_CORE0, !digitalRead(LED_CORE0));

    // Temporização precisa (melhor que delay)
    vTaskDelayUntil(&last, period);
  }
}

// Task no Core 1: pisca LED_CORE1 a cada 700 ms
void taskCore1(void*){
  const TickType_t period = pdMS_TO_TICKS(700);
  TickType_t last = xTaskGetTickCount();

  for(;;){
    Serial.print("[taskCore1] CoreID=");
    Serial.print(xPortGetCoreID());
    Serial.print("  Suspenso? ");
    Serial.println(isSuspended1 ? "SIM":"NAO");

    digitalWrite(LED_CORE1, !digitalRead(LED_CORE1));
    vTaskDelayUntil(&last, period);
  }
}

// Task de botões: polling + debounce simples; alterna suspensão/retomada
void taskButtons(void*){
  // Estados anteriores para detectar borda (HIGH->LOW = pressionado)
  int prev0 = HIGH, prev1 = HIGH;

  // Contadores para debounce (em ticks ou ms)
  const TickType_t period = pdMS_TO_TICKS(10); // varredura a cada ~10 ms
  TickType_t last = xTaskGetTickCount();

  // Duração mínima do "LOW" para considerar um clique (debounce)
  const uint8_t STABLE_COUNT = 4; // ~40 ms

  uint8_t lowCount0 = 0;
  uint8_t lowCount1 = 0;

  for(;;){
    int r0 = digitalRead(BTN0);
    int r1 = digitalRead(BTN1);

    // --- Debounce BTN0 ---
    if (r0 == LOW) { // pressionado (pull-up)
      if (lowCount0 < 255) lowCount0++;
    } else {
      lowCount0 = 0;
    }

    // Se manteve LOW por tempo suficiente e antes estava HIGH -> clique válido
    if (lowCount0 == STABLE_COUNT && prev0 == HIGH) {
      // Alterna estado: suspender ou retomar taskCore0
      if (!isSuspended0) {
        vTaskSuspend(hTask0);   // pausa a task do Core 0
        isSuspended0 = true;
        Serial.println("[Buttons] taskCore0 SUSPENSA");
      } else {
        vTaskResume(hTask0);    // retoma a task do Core 0
        isSuspended0 = false;
        Serial.println("[Buttons] taskCore0 RETOMADA");
      }
    }
    prev0 = r0;

    // --- Debounce BTN1 ---
    if (r1 == LOW) {
      if (lowCount1 < 255) lowCount1++;
    } else {
      lowCount1 = 0;
    }

    if (lowCount1 == STABLE_COUNT && prev1 == HIGH) {
      if (!isSuspended1) {
        vTaskSuspend(hTask1);   // pausa a task do Core 1
        isSuspended1 = true;
        Serial.println("[Buttons] taskCore1 SUSPENSA");
      } else {
        vTaskResume(hTask1);    // retoma a task do Core 1
        isSuspended1 = false;
        Serial.println("[Buttons] taskCore1 RETOMADA");
      }
    }
    prev1 = r1;

    // Mantém período fixo de leitura dos botões
    vTaskDelayUntil(&last, period);
  }
}

```

---

## 🔍 Explicação do Código

1. **`xTaskCreatePinnedToCore`**

   * Cria uma tarefa FreeRTOS e fixa em um núcleo específico (`0` ou `1`).
   * Parâmetros principais:

     * Nome da função (`taskCore0` / `taskCore1`)
     * Nome simbólico da tarefa
     * Stack size (2048)
     * Prioridade (2)
     * Núcleo (`0` ou `1`)

2. **`xPortGetCoreID()`**

   * Retorna o número do núcleo em que a tarefa está rodando.
   * Usado para imprimir no **Serial Monitor** e confirmar.

3. **`vTaskDelayUntil`**

   * Faz a tarefa “dormir” até o próximo tick, garantindo precisão no tempo de pisca.
   * Melhor que `delay()`, pois não atrapalha o escalonamento.

4. **LEDs**

   * Cada task controla um LED independente, piscando em períodos diferentes.
   * Demonstra execução em paralelo.

---

## 🧪 Experimentos em Sala

1. **Rodar o código padrão**:

   * Observar no Serial Monitor os logs:

     * `taskCore0` sempre em **Core 0**
     * `taskCore1` sempre em **Core 1**
   * LEDs piscam em ritmos diferentes (250ms vs 700ms).

2. **Trocar os núcleos**:

   * Alterar o último parâmetro de cada `xTaskCreatePinnedToCore`.
   * Verificar que os CoreIDs se invertem no log.

3. **Alterar prioridades**:

   * Colocar `taskCore0` com prioridade 3 e `taskCore1` com prioridade 1.
   * Discutir se o comportamento muda (ambas usam `vTaskDelayUntil`, então rodam cooperativamente).

4. **Mudar os períodos**:

   * Ex.: 100ms e 1000ms.
   * Verificar independência temporal entre as tasks.

---

## ❓ Perguntas para Discussão

* Qual a vantagem de usar **dois núcleos** em projetos reais?
* Em que situações seria melhor **pinar** tarefas a um núcleo específico?
* O que acontece se usarmos `delay()` em vez de `vTaskDelayUntil`?
* Como poderíamos estender este exemplo para **captura de dados no Core 0** e **processamento no Core 1**?

---

## ✅ Conclusão

Nesta aula aprendemos a:

* Criar tarefas no ESP32 fixadas em diferentes núcleos.
* Usar `xPortGetCoreID()` para confirmar a execução.
* Visualizar o paralelismo através do piscar de LEDs.

👉 Na próxima etapa, podemos expandir para **comunicação inter-core** (Queue), simulando um pipeline de **captura de dados → processamento → decisão**.

---


