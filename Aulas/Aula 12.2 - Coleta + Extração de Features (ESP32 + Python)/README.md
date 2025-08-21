# 📘 Aula 1 — Coleta + Extração de *Features* (ESP32 + Python)

> **Objetivo:**  
> Nesta primeira aula vamos **coletar dados** de dois potenciômetros conectados ao ESP32, **enviar janelas via Serial** para o PC, **extrair features em Python** (*mean, std, rms, ptp*) e **controlar um LED** como simulação de limite (alarme).

---

## 🎯 Metas de Aprendizagem
- Compreender como coletar sinais analógicos com o **ESP32**.  
- Formar **janelas de dados** (200 amostras a 1 kHz).  
- Processar os sinais no **Python** para extrair *features*.  
- Implementar um **limiar dinâmico** usando o segundo potenciômetro.  
- Controlar **LEDs** no ESP32 a partir da lógica feita no Python.

---

## 🔌 Montagem do Circuito

### Materiais
- 1x **ESP32 DevKit**  
- 2x **Potenciômetros** de 10 kΩ  
- 2x **LEDs** + 2x resistores 220 Ω (pode usar o LED onboard no GPIO2)  
- Protoboard e jumpers

### Conexões
- **POT1 (sinal medido)**:  
  - terminal central → **GPIO34**  
  - laterais → **3V3** e **GND**
- **POT2 (limiar de referência)**:  
  - terminal central → **GPIO35**  
  - laterais → **3V3** e **GND**
- **LED_PROCESS**: GPIO4 → resistor → LED → GND  
- **LED_LIMIT**: GPIO2 (LED onboard em muitas placas)

---

## 🧩 Código do ESP32 (Arduino/PlatformIO)

### Arquivo: `src/main.cpp`
```cpp
#include <Arduino.h>

constexpr int ADC_POT1 = 34;    // POT1: sinal
constexpr int ADC_POT2 = 35;    // POT2: limiar
constexpr gpio_num_t LED_PROCESS = GPIO_NUM_4; 
constexpr gpio_num_t LED_LIMIT   = GPIO_NUM_2; 

constexpr int FS_HZ = 1000;     // taxa de amostragem
constexpr int WIN_MS = 200;     // janela de 200 ms
constexpr int N = FS_HZ * WIN_MS / 1000;

static uint16_t buf1[N], buf2[N];

void setup() {
  Serial.begin(115200);
  pinMode(LED_PROCESS, OUTPUT);
  pinMode(LED_LIMIT, OUTPUT);

  analogReadResolution(12);
  analogSetPinAttenuation(ADC_POT1, ADC_11db);
  analogSetPinAttenuation(ADC_POT2, ADC_11db);

  Serial.println("#seq,ts_ms,sample,pot1_raw,pot2_raw"); // cabeçalho CSV
}

void loop() {
  static uint32_t seq = 0;
  const uint32_t Ts_us = 1000000UL / FS_HZ;
  uint32_t t0 = millis();

  // 1) Coleta da janela
  for (int i = 0; i < N; ++i) {
    buf1[i] = analogRead(ADC_POT1);
    buf2[i] = analogRead(ADC_POT2);
    delayMicroseconds(Ts_us);
  }

  // 2) Impressão em formato CSV
  uint32_t ts = millis() - t0;
  for (int i = 0; i < N; ++i) {
    Serial.print(seq); Serial.print(',');
    Serial.print(ts);  Serial.print(',');
    Serial.print(i);   Serial.print(',');
    Serial.print(buf1[i]); Serial.print(',');
    Serial.println(buf2[i]);
  }
  seq++;

  // 3) Feedback visual: LED_PROCESS pisca
  digitalWrite(LED_PROCESS, HIGH); delay(20); digitalWrite(LED_PROCESS, LOW);

  // 4) Comandos vindos do Python
  while (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd.equalsIgnoreCase("LED ON"))  digitalWrite(LED_LIMIT, HIGH);
    if (cmd.equalsIgnoreCase("LED OFF")) digitalWrite(LED_LIMIT, LOW);
  }
}
````

---

## 🔍 Explicação do Código

### 1. Definição de pinos e parâmetros

```cpp
constexpr int FS_HZ = 1000;
constexpr int WIN_MS = 200;
constexpr int N = FS_HZ * WIN_MS / 1000;
```

* Captura a **1000 amostras por segundo** (1 kHz).
* Cada janela dura **200 ms** → **200 amostras**.

### 2. `setup()`

* Inicializa a **Serial** a 115200 bps.
* Configura **LEDs** como saída.
* Ajusta ADC do ESP32 para **12 bits** e faixa **0–3.3 V**.
* Imprime **cabeçalho CSV** para o Python reconhecer os campos.

### 3. `loop()`

* Lê **200 amostras** de cada potenciômetro com espaçamento de **1 ms**.
* Imprime cada amostra como linha CSV:
  `seq, ts_ms, sample_index, pot1_raw, pot2_raw`
* Pisca **LED\_PROCESS** ao final de cada janela → confirma captura.
* Escuta comandos **"LED ON"/"LED OFF"** e controla **LED\_LIMIT**.

---

## 🐍 Código Python (PC) — Extração de Features + Controle de LED

### Arquivo: `aula1_coleta_features.py`

```python
import serial, time
import numpy as np
import pandas as pd
from collections import deque

PORT = "COM3"   # ajuste p/ Linux: /dev/ttyUSB0
BAUD = 115200
N = 200  # tamanho da janela

def extract_feats(x):
    x = x.astype(np.float32)
    return dict(
        mean=float(x.mean()),
        std=float(x.std(ddof=1)),
        rms=float(np.sqrt((x**2).mean())),
        ptp=float(x.max() - x.min())
    )

def raw_to_unit(raw): return raw / 4095.0

ser = serial.Serial(PORT, BAUD, timeout=1)
time.sleep(2.0)

rows = []
buf1, buf2 = deque(maxlen=N), deque(maxlen=N)

print("Coletando… CTRL+C para parar")
try:
    while True:
        line = ser.readline().decode(errors="ignore").strip()
        if not line or line.startswith("#"): continue
        seq, ts_ms, sample, r1, r2 = line.split(',')
        r1, r2 = int(r1), int(r2)

        buf1.append(r1); buf2.append(r2)

        if len(buf1) == N:
            x = np.array(buf1)/4095.0
            z = np.array(buf2)/4095.0
            feats = extract_feats(x)
            feats['seq'], feats['pot2_mean'] = int(seq), float(z.mean())
            rows.append(feats)

            # Simulação de limite
            TH_ON  = max(0.05, min(0.95, feats['pot2_mean']))
            TH_OFF = max(0.0, TH_ON - 0.1)
            score  = feats['rms']

            leak = getattr(extract_feats, "_leak", False)
            if not leak and score >= TH_ON:
                ser.write(b"LED ON\n"); extract_feats._leak = True
            elif leak and score <= TH_OFF:
                ser.write(b"LED OFF\n"); extract_feats._leak = False

            print(f"[seq {seq}] rms={score:.3f} "
                  f"THon={TH_ON:.2f} THoff={TH_OFF:.2f} "
                  f"LED={'ON' if getattr(extract_feats,'_leak',False) else 'OFF'}")

            buf1.clear(); buf2.clear()

except KeyboardInterrupt:
    pass
finally:
    ser.close()

if rows:
    df = pd.DataFrame(rows)
    df.to_csv("features_aula1.csv", index=False)
    print("Arquivo salvo: features_aula1.csv")
```

---

## 🔎 Explicação do Script Python

1. **Leitura da Serial**: captura linhas CSV enviadas pelo ESP32.
2. **Buffers circulares**: juntam 200 amostras (1 janela).
3. **Extração de Features**: calcula `mean`, `std`, `rms`, `ptp`.
4. **Mapeamento de limiar**: POT2 define `TH_ON`; `TH_OFF = TH_ON - 0.1`.
5. **Decisão com histerese**: LED liga quando `RMS ≥ TH_ON` e só desliga quando `RMS ≤ TH_OFF`.
6. **Comando ao ESP32**: envia `"LED ON"` / `"LED OFF"`.
7. **Log e salvamento**: imprime os valores no terminal e salva em `features_aula1.csv`.

---

## 🧪 Experimento em Sala

1. Grave o código no **ESP32**.
2. Execute `python aula1_coleta_features.py`.
3. Gire **POT1** → altera o valor do RMS (*feature*).
4. Gire **POT2** → muda o limiar.
5. Observe:

   * **LED\_PROCESS** pisca a cada janela.
   * **LED\_LIMIT** acende/apaga de acordo com as condições.
   * O terminal mostra `mean, std, rms, ptp` e o estado do LED.

---

## ✅ Resultados Esperados

* Dataset `features_aula1.csv` com colunas: `seq, mean, std, rms, ptp, pot2_mean`.
* Entendimento prático de **janelação**, **features simples** e **limiar dinâmico**.
* LED funcionando como **indicador de alarme**.

---

## 📝 Próximos Passos

Na **Aula 2**, vamos:

* Carregar o CSV (`features_aula1.csv`).
* Rotular os dados (normal/vazamento).
* Treinar um classificador leve no **PyCaret**.
* Exportar o modelo para **C** (via `m2cgen`) para embarcar no ESP32.

---
