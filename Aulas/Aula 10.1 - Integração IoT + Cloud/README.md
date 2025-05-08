## 📘 Aula 10.1: Registro de Acessos com ESP32 e RFID na Plataforma ThingSpeak

Esta aula guia o desenvolvimento de um sistema de **registro de acessos com RFID**, utilizando o **ESP32** e a plataforma **ThingSpeak** para envio e visualização dos dados na nuvem.

---

### 🎯 Objetivos
- Realizar leitura de cartões/tag RFID com ESP32
- Verificar se o UID lido está autorizado
- Enviar UID, status e timestamp para a nuvem
- Visualizar os dados remotamente no painel do ThingSpeak

---

### 🧰 Materiais Necessários
- 1x ESP32
- 1x Leitor RFID RC522
- 1x Cartão ou Tag RFID
- Jumpers + Protoboard
- Wi-Fi ativo
- Conta gratuita no [ThingSpeak](https://thingspeak.com)

---

### 📝 Etapas

#### 1. Criar um Canal no ThingSpeak
1. Acesse [https://thingspeak.com](https://thingspeak.com)
2. Crie uma conta e clique em **"New Channel"**
3. Adicione os seguintes campos:
   - Field 1: UID
   - Field 2: Status
   - Field 3: Timestamp
4. Salve e anote sua **API Write Key**

#### 2. Configurar o Código no ESP32

- Configure sua rede Wi-Fi:
```cpp
const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";
```

- Configure sua **API Key** do ThingSpeak:
```cpp
const char* apiKey = "SUA_API_KEY";
```

- Substitua o conteúdo de `esp32_thingspeak_rfid.ino` com o código fornecido na aula.

#### 3. Conectar, Ler e Enviar
- Após fazer o upload do código para o ESP32
- Aproxime um cartão RFID do leitor RC522
- Os dados serão exibidos no Serial Monitor e enviados ao ThingSpeak

#### 4. Visualizar os Dados
- Acesse seu canal
- Vá até a aba **Private View** ou **Public View**
- Adicione blocos (widgets) para os campos UID, Status e Timestamp
- Veja os acessos em tempo real

---

### 🧪 Desafio da Aula
- Adicione um segundo UID autorizado no código
- Mude a lógica para exibir o nome da pessoa autorizada no campo `status`
- Crie um gráfico no ThingSpeak que mostre acessos autorizados e negados ao longo do tempo

---

### ✅ Avaliação
| Critério                            | Pontos |
|-------------------------------------|--------|
| Código funcional com leitura RFID   | 2.0    |
| Verificação de autorização          | 2.0    |
| Envio bem-sucedido ao ThingSpeak    | 3.0    |
| Organização e comentários no código | 1.0    |
| Print ou visualização do canal      | 2.0    |

---

### 🧠 Expansões Futuras
- Uso de sensor adicional (ex: temperatura, PIR)
- Gravação em planilha Google Sheets simultaneamente
- Envio de notificação via Telegram ou Email (via Node-RED)

---

📌 **Esta aula reforça conceitos de IoT, redes sem fio e integração com plataformas de dados em nuvem de forma prática e acessível.**
