## 📘 Aula: Controle de Acesso com ESP32, RFID e Google Sheets

Nesta aula, vamos desenvolver um sistema completo de **registro de acessos com cartão RFID** utilizando o **ESP32** e a integração com o **Google Sheets** como banco de dados online. Essa solução é acessível, gratuita e não exige servidores dedicados, ideal para projetos de IoT educacionais.

---

### 🎯 Objetivos da Aula
- Compreender o funcionamento do sensor RFID RC522
- Realizar a leitura de UID de cartões RFID
- Verificar permissões de acesso
- Criar um Web App no Google Apps Script para receber dados
- Integrar ESP32 com o Google Sheets via HTTP POST

---

### 🧰 Materiais Necessários
- ESP32
- Módulo RFID RC522
- Tags ou cartões RFID
- Protoboard e jumpers
- Wi-Fi ativo
- Conta Google (para o Google Sheets)

---
## 🧩 Etapa 1 – Leitura do Cartão RFID com ESP32
### 🎯 Objetivo
Ler e exibir o UID do cartão/tag no Serial Monitor.
![RC522](https://www.electrovigyan.com/wp-content/uploads/2021/04/pinout.jpg)

```cpp

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 21
#define RST_PIN 22

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  SPI.begin();              // Inicializa SPI
  rfid.PCD_Init();          // Inicializa RFID
  Serial.println("Aproxime a tag...");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

  Serial.print("UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(i < rfid.uid.size - 1 ? " " : "");
  }
  Serial.println();
  delay(1000);
}

```

### ✅ Testar: Aproxime a tag e veja o UID no Monitor Serial.

## 📝 Etapas do Projeto

### 1. Preparar a Planilha Google
1. Acesse [https://sheets.google.com](https://sheets.google.com)
2. Crie uma nova planilha chamada **Acessos RFID**
3. Na primeira linha, adicione os títulos:
   ```
   UID | Status | Timestamp
   ```

### 2. Criar o Script Web App
1. Vá em **Extensões > Apps Script**
2. Apague o conteúdo e cole:
```javascript
function doPost(e) {
  var ss = SpreadsheetApp.getActiveSpreadsheet();
  var sheet = ss.getActiveSheet();

  var data = JSON.parse(e.postData.contents);
  var uid = data.uid;
  var status = data.status;
  var timestamp = data.timestamp;

  sheet.appendRow([uid, status, timestamp]);
  return ContentService.createTextOutput("OK");
}
```
3. Clique em **Implantar > Implantar como aplicativo da web**
   - Executar como: Você mesmo
   - Acesso: Qualquer pessoa
4. Clique em **Implantar** e copie o **URL do Web App gerado**

### 3. Programar o ESP32
- Abra o código do projeto (veja o arquivo `esp32_google_sheets_rfid.ino`)
- Atualize os seguintes dados:
```cpp
const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";
const char* scriptURL = "https://script.google.com/macros/s/SEU_ENDPOINT/exec";
```
- Compile e envie para o ESP32 via Arduino IDE
- O código:
```cpp
// ESP32 + RFID + Google Sheets (via Web Apps)
#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 21
#define RST_PIN 22
MFRC522 rfid(SS_PIN, RST_PIN);

const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";
const char* scriptURL = "https://script.google.com/macros/s/SEU_ENDPOINT/exec"; // Substitua com seu link do Apps Script

String uid_autorizado = "0A F3 1B 7C";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");

  SPI.begin();
  rfid.PCD_Init();
  Serial.println("Aproxime uma tag...");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i], HEX);
    if (i < rfid.uid.size - 1) uid += " ";
  }
  uid.toUpperCase();

  String status = (uid == uid_autorizado) ? "Acesso autorizado" : "Acesso negado";
  String timestamp = String(millis() / 1000);

  sendToSheet(uid, status, timestamp);
  delay(3000);
}

void sendToSheet(String uid, String status, String timestamp) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(scriptURL);
    http.addHeader("Content-Type", "application/json");

    String json = "{\"uid\":\"" + uid + "\",\"status\":\"" + status + "\",\"timestamp\":\"" + timestamp + "\"}";
    int httpCode = http.POST(json);
    Serial.println("Resposta: " + String(httpCode));
    http.end();
  }
}

```

### 4. Testar e Visualizar Resultados
- Abra o Monitor Serial
- Aproxime um cartão RFID ao leitor
- Verifique se a planilha recebe os dados

---

### 🧠 O Que os Alunos Devem Entregar
- Código funcional com comentários
- Print da planilha com os dados registrados
- Explicação do que acontece em cada etapa do sistema

---

## 💡 Desafio Extra
- Adicionar mais de um UID autorizado no código
- Exibir no Monitor Serial se o cartão é autorizado ou não
- Adicionar campo "nome da pessoa" ao lado do UID (no código e planilha)

---

### 📌 Resultados Esperados
- A cada aproximação de um cartão, o ESP32 envia UID, status e hora ao Google Sheets
- Os acessos ficam registrados automaticamente em tempo real
- Nenhum servidor Node-RED ou Firebase é necessário

---
