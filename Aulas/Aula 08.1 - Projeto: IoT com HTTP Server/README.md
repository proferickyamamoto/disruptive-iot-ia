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
