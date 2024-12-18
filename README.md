# Competição de Barcos - Barco Marrom
  
 O projeto aqui exposto é o código desenvolvido pelos integrantes do grupo equivalente ao _Barco Marrom_ para seu funcionamento no trabalho final de _Programação de Sistemas Embarcados_.
 
## Índice

1. [Integrantes](#itegrantes)
2. [Leme e Motor](#leme-e-Motor)
3. [Magnetômetro HMC5883L](#magnetômetro-hmc5883l)
4. [BLE](#conexao-bluetooth-via-ble)
5. [Instalação](#instalação)

### 1. Integrantes:

* Elias Nacif
* Everson Elias
* Vincent Pernarh
  
### 2. Leme e Motor:

Como primeira porção do trabalho desenvolvemos funções concernindo o funcionamento e uso do leme e motor do barco durante a competição.
Para tal, reconhecendo que os componentes MG90S (servo motor) e o Motor DC utilizam-se de sinais PWM para seu funcionamento desenvolveu-se a função _setPWM_ representada abaixo. A função em questão teve o fim de setar os diversos parâmetros de funcionamento dos timers e sinais PWM's utilizados no funcionamento dos componentes supracitados.
``` C
// Função PWM
void setPWM(TIM_HandleTypeDef timer, uint32_t channel, uint16_t period, uint16_t pulse);
```
Ademais, para o uso devido e amplo do Leme ao longo do projeto desenvolvemos a seguinte função de controle:
``` C
// Função de Controle do Leme
void Rudder_Control(TIM_HandleTypeDef timer, uint32_t channel, uint16_t newPosition, uint8_t speed);
```
A função acima toma em consideração os parâmetros:
* _timer e channel_, apontam para o sinal de PWM que será utilizado para o funcionamento do servo-motor pelo timer e seu canal de saída específico,
* _newPosition_, indica qual é a nova posição que o leme deve tomar. Tomando em consideração que o servo motor MG90S tem um range de 180°, as entradas aceitas para o funcionamento da função variam de 44 a 144.
  > 44 : 0° (leme totalmente à direita tomando em consideração a frente do barco)
  >
  > 94 : 90° (leme centralizado e em sua posição neutra)
  > 
  > 144 : 180° (leme totalmente à esquerda tomando em consideração a frente do barco)
* _speed_, indica a velocidade com que o leme irá variar para sua nova posição. Os valores programados para tal parâmetro variam entre 1 e 10 de forma crescente. Considerando a conexão direta do parâmetro em questão com a parte de controle e direcionamento do barco ao longo da sua trajetória, essa porção do código ainda está sujeita a reajustes e melhoramentos futuros.

Por fim, desenvolveu-se a função de controle do Motor DC como pode ser vista abaixo:
``` C
// Função de Controle do Motor
void Motor_Control(TIM_HandleTypeDef timer, uint32_t channel, bool enable, bool direction, uint16_t speed)
```
A função acima, em seguida, toma em consideração os parâmetros:
* _timer e channel_, apontam para o sinal de PWM que será utilizado para o funcionamento do servo-motor pelo timer e seu canal de saída específico,
* _enable_, o parâmetro booleano em questão tem a função de ativar ou desativar o motor em questão
* _direction_, aponta para o sentido de movimentação que o barco irá tomar. _true_ = "para frente" e _false_ = "ré"
* _speed_, aponta para a velocidade desejada de operação do motor. O _speed_ pode ter valores que variam de 0 a 1000 em uma relação direta de 10x (500 = 50% da potência, 200 = 20%, etc). Importante notar que para valores iguais a 100 ou menores, o motor não possuirá potência suficiente para funcionar. 

Para testar a operação de ambas as funções em questão, adicione o código em questão ao seu main.c nos locais apontados:
``` C
//  ==== Include ====
#include "MOTOReLEME.h"
// ==================

// O código abaixo deve ser considerado o while(1) presente na função main:
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  Rudder_Control(htim4, TIM_CHANNEL_1, 44,10);
	  HAL_Delay(2000);
	  Rudder_Control(htim4, TIM_CHANNEL_1, 144,10);

	  Motor_Control(htim3, TIM_CHANNEL_2, true, true, 500); // Enable motor, forward direction, 50% speed
	  HAL_Delay(2000); // Wait for 2 seconds
	  Motor_Control(htim3, TIM_CHANNEL_2, false, true, 0); // Disable motor
  }
  /* USER CODE END 3 */
```
### 3. Magnetômetro HMC5883L

O magnetômetro HMC5883L foi integrado ao projeto para medir a direção do barco com base no campo magnético terrestre. A comunicação com o sensor é feita via I2C, e as leituras de dados brutos são processadas para calcular a direção do barco em relação ao Norte magnético.

#### a. Funções do Magnetômetro

O módulo *magnetometro.h* contém as seguintes funções para configuração, leitura e processamento de dados do sensor HMC5883L:

1. **`HMC5883L_Init(I2C_HandleTypeDef *hi2c)`**
   - **Descrição**: Inicializa e configura o sensor HMC5883L para seu funcionamento desejado. Configura o número de amostras, taxa de dados e o modo de medição.
   - **Parâmetros**:
     - `hi2c`: Ponteiro para uma estrutura `I2C_HandleTypeDef`, que contém as informações de configuração do I2C.
   - **Exemplo de uso**:
     ```c
     HMC5883L_Init(&hi2c1);
     ```

2. **`HMC5883L_ReadData(I2C_HandleTypeDef *hi2c, XYCoordinates reading[8])`**
   - **Descrição**: Lê os dados brutos do campo magnético nos eixos X e Y. O sensor retorna um array de 8 leituras, com as coordenadas em dois eixos.
   - **Parâmetros**:
     - `hi2c`: Ponteiro para a estrutura `I2C_HandleTypeDef`.
     - `reading`: Array de estruturas `XYCoordinates` para armazenar os dados brutos de cada leitura.
   - **Exemplo de uso**:
     ```c
     XYCoordinates readings[8];
     HMC5883L_ReadData(&hi2c1, readings);
     ```

3. **`Filter_Data(XYCoordinates reading[8])`**
   - **Descrição**: Filtra as leituras do magnetômetro, removendo os dados que se desviam significativamente da média (centróide). Retorna o número de leituras válidas restantes.
   - **Parâmetros**:
     - `reading`: Array de leituras do magnetômetro.
   - **Exemplo de uso**:
     ```c
     uint8_t validReadings = Filter_Data(readings);
     ```

4. **`Measure_Distance(XYCoordinates a, XYCoordinates b)`**
   - **Descrição**: Calcula a distância euclidiana entre dois pontos no plano 2D (eixos X e Y). Essa função é usada para determinar a proximidade das leituras em relação ao centróide.
   - **Parâmetros**:
     - `a`: Primeira coordenada (estrutura `XYCoordinates`).
     - `b`: Segunda coordenada (estrutura `XYCoordinates`).
   - **Exemplo de uso**:
     ```c
     float distance = Measure_Distance(reading[i], centroid);
     ```

5. **`Data_Means(XYCoordinates reading[8], uint8_t n)`**
   - **Descrição**: Calcula a média das coordenadas X e Y de um conjunto de leituras do magnetômetro.
   - **Parâmetros**:
     - `reading`: Array de leituras do magnetômetro.
     - `n`: Número de leituras.
   - **Exemplo de uso**:
     ```c
     XYCoordinates centroid = Data_Means(readings, 8);
     ```

6. **`XY_to_Degrees(XYCoordinates coordinates)`**
   - **Descrição**: Converte as coordenadas X e Y para um ângulo em graus, com base no campo magnético e ajustado para o Norte 	magnético. A calibração foi feita com base em uma bússola.
   - **Parâmetros**:
     - `coordinates`: Estrutura `XYCoordinates` contendo as coordenadas X e Y.
   - **Exemplo de uso**:
     ```c
     int16_t angle = XY_to_Degrees(centroid);
     ```

7. **`RudderDegree(uint16_t boatAngle, int16_t arrivalAngle)`**
   - **Descrição**: Calcula o ângulo do leme necessário para orientar o barco em direção ao ângulo de chegada desejado.
   - **Parâmetros**:
     - `boatAngle`: Ângulo atual do barco.
     - `arrivalAngle`: Ângulo de chegada desejado.
   - **Exemplo de uso**:
     ```c
     int16_t rudderAngle = RudderDegree(boatHeading, targetHeading);
     ```

#### b. Exemplo de Integração no `main.c`

Após a inclusão do arquivo *magnetometro.h*, o código do `main.c` pode ser atualizado para incluir o uso do magnetômetro:

```c
// ==== Includes ====
#include "magnetometro.h"
#include "main.h"
// ==================

// Função principal
int main(void) {
    HAL_Init();
    HMC5883L_Init(&hi2c1);

    XYCoordinates readings[8];

    while (1) {
        // Leitura dos dados do magnetômetro
        HMC5883L_ReadData(&hi2c1, readings);

        // Filtra os dados
        uint8_t validReadings = Filter_Data(readings);

        // Calcula o ângulo
        int16_t heading = XY_to_Degrees(Data_Means(readings, validReadings));

        // Usa a direção para ajustar o leme
        Rudder_Control(htim4, TIM_CHANNEL_1, heading, 10);
        
        HAL_Delay(1000);
    }
}
 ```

### 4. BLE : Conexão Bluetooth via BLE:
O Bluetooth Low Energy (BLE) é uma tecnologia de comunicação sem fio projetada para dispositivos que precisam transferir dados com baixo consumo de energia. Nesse caso, a tecnologia BLE foi utilizada para localizar a posição de um barco ao longo do lago.  

Três posições no lago foram demarcadas com o uso de **beacons**, que se comunicam com o barco por meio do BLE. Cada *beacon* fornece dados sobre a distância entre ele e o barco, permitindo ao sistema calcular a posição exata do barco com base nessas informações.  

A imagem abaixo ilustra essa configuração, onde as distâncias entre o barco e os três *beacons* (d1, d2 e d3) são utilizadas para determinar a posição do barco por meio de técnicas de _trilateração_.

<img src="/img/arquivo.gif">

### 5. Instalação:

Para a instalação do código aqui presente, clone este repositório:  
   ```bash
   https://github.com/eversoneliasg/pse-barco-marrom.git
