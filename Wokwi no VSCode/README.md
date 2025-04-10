# Projeto IoT com ESP32 e MQTT Simulado no Wokwi

## Descrição

Este projeto demonstra como configurar um ESP32 para conectar-se a uma rede Wi-Fi e enviar e receber dados via MQTT. A simulação é realizada no [Wokwi](https://wokwi.com/), um simulador online de hardware. O desenvolvimento é feito utilizando o Visual Studio Code com a extensão PlatformIO.

## Adaptação

Esse repositório é uma adaptação deste tutorial.[https://docs.google.com/document/d/1y6IfbOT_rAimZx41tNBL9NlscoB1ObjgaPmy2g4UGO0/edit?usp=sharing](https://docs.google.com/document/d/1y6IfbOT_rAimZx41tNBL9NlscoB1ObjgaPmy2g4UGO0/edit?usp=sharing) 


## Funcionalidades

- **Conexão Wi-Fi**: O ESP32 conecta-se automaticamente à rede Wi-Fi especificada.
- **Comunicação MQTT**: Envia e recebe mensagens através de um broker MQTT.
- **Simulação no Wokwi**: Permite testar o projeto sem a necessidade de hardware físico.

## Pré-requisitos

- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO IDE](https://platformio.org/install/ide?install=vscode)
- Conta no [Wokwi](https://wokwi.com/)
- Broker MQTT (como o [Mosquitto](https://mosquitto.org/) ou serviços online como o [HiveMQ](https://www.hivemq.com/))

## Instalação

1. **Clone o repositório do prof. Arnaldo:**

   ```bash
   git clone https://github.com/arnaldojr/iot-esp32-wokwi-vscode.git
   ```

2. Abra o projeto no VSCode:

    Abra o Visual Studio Code e navegue até a pasta do projeto clonado.

3. Instale as dependências:

    O PlatformIO irá instalar automaticamente as bibliotecas necessárias durante a primeira compilação.

## Configuração

Credenciais Wi-Fi e MQTT:

No arquivo src/main.cpp, insira suas credenciais de Wi-Fi e as informações do broker MQTT:

```cpp

// Configurações de WiFi
const char *SSID = "Wokwi-GUEST"; // não precisa alterar no simulador
const char *PASSWORD = "";        // 

// Configurações de MQTT
const char *BROKER_MQTT = "broker.hivemq.com"; // seu broker mqtt
const int BROKER_PORT = 1883;
const char *ID_MQTT = "esp32_mqtt";
const char *TOPIC_SUBSCRIBE_LED = "fiap/iot/led";  // seu topico SUB
const char *TOPIC_PUBLISH_TEMP_HUMI = "fiap/iot/temphumi"; // seu tópico PUB
```

## Uso

1. Compilar o projeto:

No PlatformIO, clique em Build para compilar o código.

2. Iniciar a simulação:

- No Wokwi, inicie a simulação.
- Observe a saída serial para verificar a conexão Wi-Fi e a comunicação MQTT.

3. Testar a comunicação MQTT:

- Use um cliente MQTT (como o node-red) para publicar e subscrever tópicos para interagir com o ESP32.

4. Simulação Dicas:

- O Wokwi permite simular o comportamento do ESP32 em tempo real, mas preste atenção no tempo de execução que pode variar e ficar lento:
- Utilize o monitor serial para acompanhar os logs da aplicação.
