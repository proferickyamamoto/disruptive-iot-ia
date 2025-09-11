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
  Demo Dual-Core (ESP32) – Código Comentado
  -------------------------------------------------------
  O objetivo é demonstrar como criar duas tarefas FreeRTOS,
  cada uma executando em um núcleo (core) diferente do ESP32.

  O que este exemplo faz:
  - Cria taskCore0 "pinnada" no Core 0 e taskCore1 no Core 1.
  - Cada task pisca um LED com um período próprio (250 ms e 700 ms).
  - Cada task imprime no Serial o ID do core em que está rodando.
  - Usa vTaskDelayUntil para obter temporização estável (melhor que delay()).

  Observações:
  - ESP32-WROOM/WROVER tem 2 cores (0 e 1).
  - ESP32-C3 é single-core (este exemplo ainda roda, mas ambas as tasks
    executam no mesmo core).
*/

#include <Arduino.h>

// ---------- Pinos dos LEDs ----------
// Escolhemos dois pinos diferentes para visualizar cada task.
// Em muitas devkits, o GPIO2 tem LED onboard (caso não tenha, use externo).
constexpr gpio_num_t LED_CORE0 = GPIO_NUM_4; // LED controlado pela task no Core 0
constexpr gpio_num_t LED_CORE1 = GPIO_NUM_2; // LED controlado pela task no Core 1 (onboard em várias placas)

// ---------- Protótipos das tarefas ----------
// Cada função abaixo será criada como uma "Task" FreeRTOS.
// Elas vão rodar em loop infinito em núcleos distintos.
void taskCore0(void*);  // pisca LED + loga o CoreID (Core 0)
void taskCore1(void*);  // pisca LED + loga o CoreID (Core 1)

void setup() {
  // Inicializa a porta serial para depuração (logs)
  Serial.begin(115200);
  // Pequena espera para estabilizar a serial após reset
  delay(200);

  Serial.println();
  Serial.println("[Dual-core demo] Criando tasks em cores diferentes...");

  // Configura os pinos dos LEDs como saída
  pinMode(LED_CORE0, OUTPUT);
  pinMode(LED_CORE1, OUTPUT);

  // Garante que os LEDs iniciem desligados
  digitalWrite(LED_CORE0, LOW);
  digitalWrite(LED_CORE1, LOW);

  // ---------- Criação das Tarefas com Pino de Core (xTaskCreatePinnedToCore) ----------

  // Task "taskCore0" – rodará no Core 0
  // Parâmetros:
  //  - taskCore0: função da task
  //  - "taskCore0": nome legível para debug
  //  - 2048: tamanho da stack (palavras) — ajuste se necessário
  //  - nullptr: parâmetro passado à task (não usado aqui)
  //  - 2: prioridade (maior número = maior prioridade)
  //  - nullptr: handle da task (não precisamos capturar)
  //  - 0: ID do core para "pinnar" (Core 0)
  xTaskCreatePinnedToCore(
    taskCore0,
    "taskCore0",
    2048,
    nullptr,
    2,
    nullptr,
    0 // <-- fixa no Core 0
  );

  // Task "taskCore1" – rodará no Core 1
  xTaskCreatePinnedToCore(
    taskCore1,
    "taskCore1",
    2048,
    nullptr,
    2,
    nullptr,
    1 // <-- fixa no Core 1
  );
}

void loop() {
  // O loop() NÃO é usado ativamente.
  // As tarefas FreeRTOS criadas acima executam "para sempre".
  // Mantemos um pequeno delay para não ocupar CPU desnecessariamente.
  vTaskDelay(pdMS_TO_TICKS(1000));
}

// ---------- Implementação da Task do Core 0 ----------
// Esta task:
//  - Alterna (toggle) o LED_CORE0 a cada 250 ms
//  - Imprime qual Core está executando a task (deve ser 0)
void taskCore0(void*){
  // vTaskDelayUntil usa "ticks" do RTOS; convertemos ms -> ticks
  const TickType_t period = pdMS_TO_TICKS(250);  // período de 250 ms
  // last guarda o tick da última execução para temporização precisa
  TickType_t last = xTaskGetTickCount();

  for(;;){ // loop infinito da task
    // xPortGetCoreID() retorna o núcleo corrente (0 ou 1)
    Serial.print("[taskCore0] CoreID=");
    Serial.print(xPortGetCoreID());
    Serial.println("  (toggle LED_CORE0)");

    // Inverte o estado do LED (ON->OFF ou OFF->ON)
    digitalWrite(LED_CORE0, !digitalRead(LED_CORE0));

    // Aguarda até completar exatamente 'period' desde 'last'
    // Isso evita acumular erros de tempo e mantém o piscar regular.
    vTaskDelayUntil(&last, period);
  }
}

// ---------- Implementação da Task do Core 1 ----------
// Esta task:
//  - Alterna (toggle) o LED_CORE1 a cada 700 ms
//  - Imprime qual Core está executando a task (deve ser 1)
void taskCore1(void*){
  const TickType_t period = pdMS_TO_TICKS(700);  // período de 700 ms
  TickType_t last = xTaskGetTickCount();

  for(;;){
    Serial.print("[taskCore1] CoreID=");
    Serial.print(xPortGetCoreID());
    Serial.println("  (toggle LED_CORE1)");

    digitalWrite(LED_CORE1, !digitalRead(LED_CORE1));
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


