# 📘 Aula 2 — Treinando um Classificador Leve no PyCaret e Exportando para C

> **Objetivo:**  
> Nesta segunda aula vamos pegar o **dataset de features** gerado na **Aula 1** (`features_aula1.csv`), **rotular os dados**, treinar um **classificador leve** no **PyCaret**, e exportar o modelo para **C** usando a biblioteca `m2cgen`.  
> O resultado será um arquivo `model.c` pronto para ser embarcado no ESP32 na **Aula 3**.

---

## 🎯 Metas de Aprendizagem
- Entender como rotular janelas de features (normal/vazamento).  
- Utilizar o **PyCaret** para comparar modelos de classificação.  
- Selecionar modelos **leves** adequados para microcontroladores (Logistic Regression, SVM Linear, Decision Tree rasa).  
- Exportar o modelo treinado para **C** com `m2cgen`.  
- Gerar o arquivo `model.c` que será usado no ESP32.

---

## 📂 Pré-requisitos
- Arquivo `features_aula1.csv` salvo na **Aula 1**.  
- Ambiente virtual configurado com os pacotes:
  ```bash
  pip install pycaret==3.3.2 m2cgen pandas numpy scikit-learn

* VS Code configurado para usar o ambiente virtual (`.venv`).

---

## 🔍 Estrutura do Dataset (Aula 1 → Aula 2)

Cada linha representa **uma janela** de 200 amostras:

| seq | ts\_ms | mean | std  | rms  | ptp  | pot2\_mean |
| --- | ------ | ---- | ---- | ---- | ---- | ---------- |
| 1   | 210    | 0.51 | 0.03 | 0.52 | 0.07 | 0.41       |
| 2   | 210    | 0.48 | 0.02 | 0.49 | 0.06 | 0.62       |
| …   | …      | …    | …    | …    | …    | …          |

> Para treinar o modelo precisamos de uma coluna adicional: **`label`**.

* `label = 0` → janela sem vazamento (normal)
* `label = 1` → janela com vazamento (condição de alarme)

📌 Na prática, os alunos podem **rotular manualmente** algumas janelas (com base em experimentos controlados) ou criar uma regra inicial (ex.: se `rms > valor`, define como 1).

---

## 🧩 Código em Python — Treinando e Exportando

Arquivo: `aula2_train_export.py`

```python
# Aula 2: Treinando classificador e exportando modelo
import pandas as pd
from pycaret.classification import *
import m2cgen as m2c

# 1) Carregar features da Aula 1
df = pd.read_csv("features_aula1.csv")

# 2) Rotular os dados
# Exemplo didático: usar a mediana do RMS como critério (apenas para a aula)
# Em prática real, os alunos devem rotular manualmente
if "label" not in df.columns:
    cutoff = df["rms"].median()
    df["label"] = (df["rms"] > cutoff).astype(int)

print(df.head())

# 3) Definir features e alvo
features = ["mean", "std", "rms", "ptp"]
target = "label"

# 4) Configuração do PyCaret
s = setup(
    data=df[features + [target]],
    target=target,
    session_id=42,
    normalize=True,      # Normaliza para estabilizar modelos lineares
    fold=5,
    fix_imbalance=True,  # Se classes desbalanceadas
    verbose=False
)

# 5) Comparar modelos leves
best = compare_models(include=["lr", "svm", "dt"])  
print("Melhor modelo:", best)

# 6) Finalizar pipeline
final_model = finalize_model(best)
save_model(final_model, "clf_aula2")

# 7) Exportar para C
c_code = m2c.export_to_c(final_model)
with open("model.c", "w") as f:
    f.write(c_code)

print("Modelo exportado para model.c")
```

---

## 🔍 Explicação Bloco a Bloco

### 1. Carregar features

```python
df = pd.read_csv("features_aula1.csv")
```

* Lê o CSV salvo na Aula 1.

### 2. Rotular dados

```python
cutoff = df["rms"].median()
df["label"] = (df["rms"] > cutoff).astype(int)
```

* Exemplo didático: se `rms > mediana` → `label=1`.
* Em experimentos reais, deve-se rotular manualmente ou a partir de eventos conhecidos.

### 3. Definir features

```python
features = ["mean", "std", "rms", "ptp"]
target = "label"
```

* Selecionamos apenas as colunas úteis para o modelo.

### 4. Configuração do PyCaret

```python
setup(..., normalize=True, fold=5, fix_imbalance=True)
```

* `normalize=True`: deixa todas as features na mesma escala.
* `fold=5`: validação cruzada 5-fold.
* `fix_imbalance=True`: corrige se classes desbalanceadas.

### 5. Comparação de modelos leves

```python
best = compare_models(include=["lr", "svm", "dt"])
```

* Incluímos apenas modelos **leves** e adequados para MCU.
* PyCaret retorna o **melhor modelo** com base na métrica padrão (AUC/Accuracy).

### 6. Finalização

```python
final_model = finalize_model(best)
save_model(final_model, "clf_aula2")
```

* `finalize_model`: treina o modelo final em todo o dataset.
* `save_model`: salva o pipeline (`clf_aula2.pkl`).

### 7. Exportação para C

```python
c_code = m2c.export_to_c(final_model)
with open("model.c", "w") as f:
    f.write(c_code)
```

* Converte o pipeline em **código C puro** com a função `score(double features[])`.
* O arquivo `model.c` será usado no ESP32.

---

## 📦 Resultado da Aula 2

1. Arquivo **`clf_aula2.pkl`** → modelo salvo para futuras análises.
2. Arquivo **`model.c`** → código em C com a função `score()` pronta para ser embarcada no ESP32.

---

## 🧪 Experimento em Sala

1. Carregue o `features_aula1.csv` no Python.
2. Rotule algumas janelas (manual ou via regra simples).
3. Rode o script `aula2_train_export.py`.
4. Observe os resultados do `compare_models` (qual modelo foi melhor).
5. Verifique o arquivo `model.c` gerado.

---

## ✅ Próximos Passos

Na **Aula 3** vamos:

* Criar um projeto no **PlatformIO** com `features.h`, `model.c` e `main.cpp`.
* Rodar **feature extraction em tempo real no Core 0**.
* Rodar a **classificação no Core 1** com histerese.
* Acender LEDs de acordo com a decisão do modelo.

---
