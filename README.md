# Competição de Barcos - Barco Marrom
  
 O projeto aqui exposto é o código desenvolvido pelos integrantes do grupo equivalente ao _Barco Marrom_ para seu funcionamento no trabalho final de _Programação de Sistemas Embarcados_.

## Índice

1. [Integrantes](#itegrantes)
2. [Leme e Motor](#leme-e-Motor)
3. [Instalação](#instalação)

### 1. Integrantes:

* Elias Miranda Nacif Rocha (2022054720)
* Everson Elias
* Vincent

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

### 3. Instalação:

Para a instalação do código aqui presente, clone este repositório:  
   ```bash
   https://github.com/eversoneliasg/pse-barco-marrom.git
