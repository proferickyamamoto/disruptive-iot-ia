# 📘 Aula 15.2 — Fundamentos de Processamento de Imagem (clássico) no Google Colab

> **Objetivo da aula:**  
> Entender como imagens são representadas (matriz/tensor), aplicar operações clássicas de processamento (conversões de cor, equalização, filtros, bordas, limiarização) e conectar cada etapa à base teórica.  
> **Ambiente:** Google Colab (Python, OpenCV, scikit-image, NumPy, Matplotlib).

---

## 🔎 Por que aprender processamento “clássico”?

Em visão computacional clássica, **você define** operações (ex.: blur, detecção de bordas, limiarização). Em **Deep Learning**, especialmente CNNs, os “filtros” são **aprendidos** a partir de dados. Saber o básico clássico ajuda a **pré-processar**, **interpretar** e **depurar** pipelines (inclusive quando DL entra depois). Para fundamentos: Szeliski (capítulos sobre filtros, bordas), tutoriais oficiais do OpenCV e scikit-image, e o artigo original do Canny para bordas. :contentReference[oaicite:0]{index=0}

---

## 🧰 Preparação no Colab

> Execute célula por célula.

```python
# Instalar dependências (versões estáveis p/ Colab)
!pip -q install opencv-python-headless==4.10.0.84 scikit-image==0.25.2 matplotlib==3.9.2 numpy==1.26.4
````

```python
# Imports e helper para exibir imagens corretamente (BGR->RGB)
import cv2, numpy as np, matplotlib.pyplot as plt
from skimage import data, filters, feature, exposure

def imshow(img, title="", cmap=None, size=(5,5)):
    plt.figure(figsize=size)
    if img.ndim == 2:
        plt.imshow(img, cmap=cmap or "gray")
    else:
        plt.imshow(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))
    plt.title(title); plt.axis("off"); plt.show()
```

---

## 🧱 1) Representação de imagens (teoria mínima)

* **Imagem digital** = matriz 2D (cinza) ou tensor H×W×C (RGB).
* **Amostragem e quantização**: resolução espacial e níveis (8 bits → 0–255).
* **Canais de cor**: RGB; conversões para **HSV**/**Lab** ajudam a separar iluminação (V/L) da crominância. (`cv2.cvtColor`) ([Documentação OpenCV][1])
* **Operadores locais (convolução)**: suavização (Gaussiano), gradiente (Sobel/Scharr/Prewitt) e **Canny** (multi-estágio, com critérios ótimos). ([scikit-image.org][2])

Leituras:

* Szeliski, *Computer Vision* (filtros, bordas) ([Huree Library][3])
* Canny (1986), *A Computational Approach to Edge Detection* (teoria) ([cse.ust.hk][4])
* scikit-image (Sobel/Scharr/Prewitt) ([scikit-image.org][2])

---

## 🖼️ 2) Carregar imagens de exemplo (sem upload)

Usaremos imagens públicas do `skimage.data` para padronizar.

```python
# Amostras: astronauta (RGB) e moedas (grayscale)
img_rgb = data.astronaut()                                # RGB (numpy)
img_bgr = cv2.cvtColor(img_rgb, cv2.COLOR_RGB2BGR)        # OpenCV usa BGR
img_gray = data.coins().astype(np.uint8)                  # grayscale

imshow(img_bgr, "RGB original (astronauta)")
imshow(img_gray, "Gray (coins)", cmap="gray")
```

---

## 🎨 3) Conversões de cor & equalização de contraste

**Por quê:** trabalhar em HSV/Lab permite atuar sobre **iluminação** sem distorcer cores; equalização pode realçar detalhes (especialmente com baixa luz). ([Documentação OpenCV][1])

```python
# HSV e Lab para visualização (conversão ida e volta)
img_hsv = cv2.cvtColor(img_bgr, cv2.COLOR_BGR2HSV)
img_lab = cv2.cvtColor(img_bgr, cv2.COLOR_BGR2Lab)

# Equalização do canal V (HSV) e L (Lab)
h,s,v = cv2.split(img_hsv)
v_eq = cv2.equalizeHist(v)                        # equalização global
img_eq_hsv = cv2.cvtColor(cv2.merge([h,s,v_eq]), cv2.COLOR_HSV2BGR)

L,a,b = cv2.split(img_lab)
L_eq = cv2.equalizeHist(L)
img_eq_lab = cv2.cvtColor(cv2.merge([L_eq,a,b]), cv2.COLOR_Lab2BGR)

imshow(img_eq_hsv, "Equalização no V (HSV)")
imshow(img_eq_lab, "Equalização no L (Lab)")
```

> **Nota:** Equalização (global/CLAHE) pode “preparar” melhor a imagem para segmentação/bordas. Tutoriais do OpenCV detalham `cvtColor`, histogramas e equalização. ([Documentação OpenCV][1])

---

## ✂️ 4) Suavização + detecção de bordas (Sobel/Scharr/Canny)

**Ideia:** reduzir ruído (Gaussian Blur), calcular gradientes (Sobel/Scharr), e usar **Canny** (redução de ruído → gradiente → supressão não-máxima → histerese de thresholds). ([Documentação OpenCV][1])

```python
# Suavização (reduz ruído, essencial antes de Canny)
g = cv2.GaussianBlur(img_gray, (5,5), 1.0)

# Sobel em X/Y e magnitude
sx = cv2.Sobel(g, cv2.CV_64F, 1, 0, ksize=3)
sy = cv2.Sobel(g, cv2.CV_64F, 0, 1, ksize=3)
mag = cv2.magnitude(sx, sy)
mag8 = cv2.convertScaleAbs(mag)

# Canny com histerese (ajuste 100–200 conforme a imagem)
edges = cv2.Canny(g, 100, 200)

imshow(g, "Gaussian Blur (5x5, sigma=1)", cmap="gray")
imshow(mag8, "Magnitude do gradiente (Sobel)", cmap="gray")
imshow(edges, "Canny (100, 200)", cmap="gray")
```

Para comparar operadores no `scikit-image` (Sobel/Prewitt/Scharr) e ver notas de **invariância rotacional**, consulte a doc e exemplo de bordas: o **Scharr** tem melhor rotação do que Sobel/Prewitt. ([scikit-image.org][2])

```python
from skimage import filters, feature

sk_sobel  = filters.sobel(img_gray)     # float [0,1]
sk_scharr = filters.scharr(img_gray)
sk_canny  = feature.canny(img_gray, sigma=1.0)

imshow((sk_sobel*255).astype(np.uint8),  "skimage Sobel", cmap="gray")
imshow((sk_scharr*255).astype(np.uint8), "skimage Scharr", cmap="gray")
imshow(sk_canny.astype(np.uint8)*255,    "skimage Canny (sigma=1)", cmap="gray")
```

> Referências: tutorial Canny (OpenCV) e doc scikit-image para filtros de borda. ([Documentação OpenCV][1])

---

## 🧮 5) Limiarização (Threshold): global, Otsu e adaptativa

**Por quê:** separar fundo/objeto. **Otsu** escolhe limiar ótimo via **minimização da variância intra-classe** (e/ou maximização da variância entre classes) a partir do histograma; muito usado em imagens *bimodais*. ([Escola de Engenharia Purdue][5])

```python
# Global (valor fixo)
_, th_global = cv2.threshold(img_gray, 120, 255, cv2.THRESH_BINARY)

# Otsu (automático a partir do histograma)
_, th_otsu = cv2.threshold(img_gray, 0, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)

# Adaptativo (média local por janelas; lida melhor com iluminação irregular)
th_adapt = cv2.adaptiveThreshold(img_gray, 255, cv2.ADAPTIVE_THRESH_MEAN_C,
                                 cv2.THRESH_BINARY, 35, 5)

imshow(th_global, "Threshold Global (120)", cmap="gray")
imshow(th_otsu,   "Otsu (automático)",      cmap="gray")
imshow(th_adapt,  "Adaptativo (janela=35, C=5)", cmap="gray")
```

Leitura complementar (didática) sobre Otsu: *Bioimage Analysis book* / FutureLearn (explicação variância intra/entre classes). ([bioimagebook.github.io][6])

---

## 🧪 6) *Hands-on* guiados

### 6.1 Detector simples de moedas (Canny + contornos)

```python
# 1) Blur -> 2) Canny -> 3) Contornos
g = cv2.GaussianBlur(img_gray, (5,5), 1.0)
edges = cv2.Canny(g, 80, 160)
contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

out = cv2.cvtColor(img_gray, cv2.COLOR_GRAY2BGR)
count = 0
for c in contours:
    x,y,w,h = cv2.boundingRect(c)
    if w*h > 150:  # filtra ruídos pequenos
        cv2.rectangle(out, (x,y), (x+w,y+h), (0,0,255), 2)
        count += 1

imshow(edges, "Edges (Canny)", cmap="gray")
imshow(out, f"Contornos detectados: {count}")
print("Total (após filtro de área):", count)
```

**Discussão:** ajuste os thresholds do Canny e o tamanho do blur; veja como o resultado muda (ruído x bordas nítidas). OpenCV recomenda suavizar antes do Canny (critério do algoritmo). ([Documentação OpenCV][1])

---

### 6.2 Segmentação por Otsu + pós-processamento morfológico

```python
# Otsu -> Abertura morfológica p/ remover ruído
_, otsu = cv2.threshold(img_gray, 0, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)
kernel = np.ones((3,3), np.uint8)
opened = cv2.morphologyEx(otsu, cv2.MORPH_OPEN, kernel, iterations=1)

imshow(otsu,   "Otsu (binário)", cmap="gray")
imshow(opened, "Após abertura 3x3", cmap="gray")
```

**Discussão:** Otsu funciona muito bem em histograma bimodal e iluminação razoavelmente uniforme; com iluminação irregular, prefira **threshold adaptativo** (ou CLAHE antes), como visto nos tutoriais. ([Documentação OpenCV][1])

---

## 🧠 7) Ponte para Deep Learning

* Camadas iniciais de CNNs aprendem **filtros do tipo borda/textura** (análogos aos filtros manuais, mas otimizados a partir de dados).
* Processos clássicos (equalização, redução de ruído) continuam úteis para **pré-processar** entradas de modelos.
* Para teoria e visão geral moderna, ver Szeliski (2ª ed.). ([Huree Library][3])

---

## 🔥 Desafio (entrega curta)

> **“Contagem robusta de objetos”**
> Escolha uma imagem com **múltiplos objetos** (parafusos, grãos, pílulas, sementes) e entregue:
>
> 1. Um **pipeline clássico** para **contar** os objetos (ex.: CLAHE → blur → Canny **ou** Otsu/adaptativo → morfologia → `findContours`), com **parâmetros explicados**.
> 2. **Três variações** (ex.: mudar `sigma` do blur, thresholds do Canny, janela/C do adaptativo) e uma **tabela** com a contagem/FP/FN em cada variação.
> 3. Uma **figura final** com as caixas desenhadas.
> 4. **Breve análise (5–10 linhas)**: qual pré-processamento mais ajudou? Como você **prepararia** essa imagem para um modelo de DL depois?

---

## 📚 Referências

* **OpenCV (Python) — Canny e tópicos básicos**: teoria e passos do algoritmo; conversões de cor, histogramas e equalização. ([Documentação OpenCV][1])
* **scikit-image — filtros de borda** (exemplo comparando Sobel/Prewitt/Scharr) e API de filtros. ([scikit-image.org][2])
* **Canny, J. (1986)** — *A Computational Approach to Edge Detection* (PAMI). Fundamentos teóricos, critérios ótimos, histerese. ([cse.ust.hk][4])
* **Otsu, N. (1979)** — *A Threshold Selection Method from Gray-Level Histograms*. Clássico de limiarização automática. ([Escola de Engenharia Purdue][5])
* **Textos didáticos sobre Otsu** (explicação variância intra/entre classes). ([bioimagebook.github.io][6])
* **Szeliski, R. (2022)** — *Computer Vision: Algorithms and Applications (2nd ed.)*. Visão geral moderna de processamento e visão. ([Huree Library][3])

```

---

::contentReference[oaicite:22]{index=22}
```

[1]: https://docs.opencv.org/4.x/da/d22/tutorial_py_canny.html?utm_source=chatgpt.com "Canny Edge Detection"
[2]: https://scikit-image.org/docs/0.25.x/auto_examples/edges/plot_edge_filter.html?utm_source=chatgpt.com "Edge operators — skimage 0.25.2 documentation"
[3]: https://library.huree.edu.mn/data/202295/2024-06-03/Computer%20Vision%20-%20Algorithms%20and%20Applications%202nd%20Edition%2C%20Richard%20Szeliski.pdf?utm_source=chatgpt.com "Computer Vision: Algorithms and Applications, 2nd Edition"
[4]: https://www.cse.ust.hk/~quan/comp5421/notes/canny1986.pdf?utm_source=chatgpt.com "A Computational Approach to Edge Detection"
[5]: https://engineering.purdue.edu/kak/computervision/ECE661.08/OTSU_paper.pdf?utm_source=chatgpt.com "A Tlreshold Selection Method from Gray-Level Histograms"
[6]: https://bioimagebook.github.io/chapters/2-processing/3-thresholding/thresholding.html?utm_source=chatgpt.com "Thresholding - Introduction to Bioimage Analysis"
