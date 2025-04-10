## 🚗 Projeto: Estacionamento Inteligente com ESP32 e Sensor Ultrassônico

Este projeto consiste na criação de um **sistema de estacionamento inteligente** que utiliza um **sensor ultrassônico (HC-SR04)** e **LEDs** para sinalização de distância, além de uma **interface web local** hospedada no ESP32. 

A página web mostra a **distância detectada em tempo real**, o **nível de segurança (seguro, alerta ou perigo)** e oferece um **botão de modo noturno**, que desativa as medições e os LEDs.

---

### 🎯 Objetivos
- Medir distância com sensor HC-SR04.
- Indicar níveis com LEDs (verde, amarelo e vermelho).
- Atualizar uma página web com os dados.
- Implementar botão virtual de ativação/desativação do sistema.

---

### 🔧 Componentes Necessários
- 1x ESP32
- 1x Sensor Ultrassônico HC-SR04
- 3x LEDs (verde, amarelo, vermelho)
- 3x resistores 220Ω
- Protoboard e jumpers

---

### 🧠 Lógica do Funcionamento (Pseudocódigo)

```plaintext
INICIAR Wi-Fi e servidor web
CONFIGURAR pinos do TRIG e ECHO
CONFIGURAR pinos dos LEDs como saída

ENQUANTO o sistema estiver ATIVO:
    MEDIR distância com sensor ultrassônico
    SE distância < 30 cm:
        NIVEL = PERIGO
        LIGAR LED VERMELHO
        DESLIGAR outros LEDs
    SENÃO SE distância entre 30 e 100 cm:
        NIVEL = ALERTA
        LIGAR LED AMARELO
        DESLIGAR outros LEDs
    SENÃO:
        NIVEL = SEGURO
        LIGAR LED VERDE
        DESLIGAR outros LEDs
    ATUALIZAR página HTML com distância e nível
FIM ENQUANTO

SE botão "Modo Noturno" for pressionado:
    PARAR medições
    DESLIGAR todos os LEDs
    MOSTRAR mensagem de inatividade na página

SE botão "Ativar Sistema" for pressionado:
    RETOMAR medições e lógica dos LEDs
```

---

### 🖥️ Funcionalidades da Página Web
- Exibição da distância medida
- Nível de segurança com cores e texto
- Botão de "Modo Noturno" para parar o sistema
- Botão de "Ativar Sistema" para retomar medições

---

### 🧪 Desafio Final
Implemente a interface descrita acima e a lógica completa de controle por web. Use HTML básico com rotas definidas no `WebServer` do ESP32.

Você deve conseguir:
- Acessar a página web pelo IP local do ESP32, lembrando de estar na mesma rede do ESP32, ou seja utilize o celular;
- Ver os dados atualizados a cada requisição;
- Acionar os LEDs corretamente conforme a distância;
- Interromper e retomar o sistema com os botões.

---
### Código Exemplo
```cpp
// Estacionamento Inteligente com ESP32, HC-SR04 e LEDs

#include <WiFi.h>
#include <WebServer.h>

//Insira o seus defines aqui

const char* ssid = "SEU_SSID"; # Insira os dados do hostpot aqui!
const char* password = "SUA_SENHA";

WebServer server(80);
bool sistemaAtivo = true;

void setup() {
  Serial.begin(115200);

  //Insira os setups de entrada e saídas aqui

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
  Serial.println(WiFi.localIP());

  server.on("/", handlePagina);
  server.on("/modo-noturno", HTTP_POST, handleDesligar);
  server.on("/ativar", HTTP_POST, handleAtivar);
  server.begin();
}

void loop() {
  server.handleClient();

  if (sistemaAtivo) {
    float distancia = medirDistancia();
    atualizarLEDs(distancia);
  } else {
    desligarLEDs();
  }
  delay(500);
}

float medirDistancia() {
  //Insira o seu código aqui
}

void atualizarLEDs(float distancia) {
  //Insira o seu código aqui
}

void desligarLEDs() {
  //Insira o seu código aqui
}

void handlePagina() {
  String html = "<html><head><meta http-equiv='refresh' content='2'/></head><body>";
  html += "<h1>Estacionamento Inteligente</h1>";

  if (sistemaAtivo) {
    float d = medirDistancia();
    html += "<p>Distancia: " + String(d) + " cm</p>";
    String status = (d < 30) ? "<span style='color:red;'>PERIGO</span>" : (d < 100) ? "<span style='color:orange;'>ALERTA</span>" : "<span style='color:green;'>SEGURO</span>";
    html += "<p>Status: " + status + "</p>";
    html += "<form action='/modo-noturno' method='POST'><button type='submit'>Modo Noturno</button></form>";
  } else {
    html += "<p><b style='color:gray;'>Sistema em modo noturno</b></p>";
    html += "<form action='/ativar' method='POST'><button type='submit'>Ativar Sistema</button></form>";
  }

  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleDesligar() {
  sistemaAtivo = false;
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleAtivar() {
  sistemaAtivo = true;
  server.sendHeader("Location", "/");
  server.send(303);
}

```
---

### 📚 Requisitos para Entrega
- Código comentado (Arduino)
- Apresentação na sala de aula
---

### 🚀 Extras (opcional)
- Adicione ícones visuais na interface (emojis ou HTML)
- Estilize a página com CSS inline
- Mostre a hora da última medição

---

Esse projeto é ideal para consolidar os conceitos de IoT, web server embarcado e lógica de controle físico com feedback visual! 🌐📏💡
