# 📘 Aula 01 - Introdução à Eletrônica e Microcontroladores

Nesta aula, exploraremos os conceitos básicos da eletrônica e dos microcontroladores, fundamentais para o desenvolvimento de projetos baseados em sistemas embarcados. O foco será compreender os princípios de funcionamento do Arduino e seus principais componentes.

## 📄 Conteúdo da Aula
- Conceitos básicos de eletrônica
- Introdução aos microcontroladores
- Conhecendo o **Arduino UNO**
- Pinos de Entrada e Saída (**INPUT/OUTPUT – I/Os**)
- Apresentação do **Arduino IDE**
- Ferramentas de simulação: **Tinkercad** e **Wokwi**

## 🔧 Materiais Necessários

- **Placa Arduino UNO**
- **Protoboard**
- **LEDs e resistores**
- **Botão (Push Button)**
- **Jumpers e cabos de conexão**

## 🔍 O que é um Microcontrolador?
Um **microcontrolador** é um pequeno computador integrado em um único chip, capaz de processar informações e controlar dispositivos externos. Ele contém:
- **CPU (Unidade Central de Processamento)**
- **Memória (RAM/ROM)**
- **Entradas e saídas digitais e analógicas**

### Microcontrolador x Microprocessador
A principal diferença entre microcontroladores e microprocessadores está na forma como são utilizados:
| Microcontrolador | Microprocessador |
|-----------------|-----------------|
| Componentes embutidos (memória, I/Os) | Necessita de componentes externos |
| Projetado para tarefas específicas | Projetado para execução geral |
| Baixo consumo de energia | Alto desempenho |

## 🔌 Pinos do Arduino UNO
O **Arduino UNO** possui diferentes tipos de pinos:
- **Pinos Digitais (0-13)**: Podem ser configurados como **entrada (INPUT)** ou **saída (OUTPUT)**.
- **Pinos Analógicos (A0-A5)**: Realizam leitura de sinais analógicos.
- **Pinos de Alimentação**: Fornecem **5V, 3.3V** e **GND** para os componentes.

## 💻 Instalando e Configurando o Arduino IDE
1. Baixe e instale o **Arduino IDE**: [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
2. Conecte o Arduino ao computador via cabo USB.
3. Selecione a **porta correta** (Ferramentas > Porta).
4. Escolha a **placa Arduino UNO** (Ferramentas > Placa > Arduino UNO).

## 🖥️ Primeiro Código no Arduino (Blink)
O primeiro programa que geralmente rodamos no Arduino é o **Blink**, que faz um LED piscar.

```cpp
void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Define o LED como saída
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH); // Liga o LED
  delay(1000); // Aguarda 1 segundo
  digitalWrite(LED_BUILTIN, LOW); // Desliga o LED
  delay(1000); // Aguarda 1 segundo
}
```

## 🚀 Como Testar
1. **Carregue o código** no Arduino através do Arduino IDE.
2. **Observe o LED** embutido na placa piscando a cada 1 segundo.
3. **Experimente alterar o tempo** do `delay()` para modificar o ritmo do piscar.

## 💡 TIPS
> - Certifique-se de que a placa está corretamente conectada ao computador.
> - Se houver erro na comunicação, verifique a **porta COM** e os drivers da placa.
> - Utilize simuladores como **Tinkercad** ou **Wokwi** para testar códigos sem um hardware físico.

## 📚 Materiais de Apoio
- [Documentação Oficial do Arduino](https://docs.arduino.cc/)
- [Tinkercad - Simulador Online](https://www.tinkercad.com/)
- [Wokwi - Simulador Online](https://wokwi.com/)

## 📩 Contato
Dúvidas ou sugestões? Entre em contato com o professor!

