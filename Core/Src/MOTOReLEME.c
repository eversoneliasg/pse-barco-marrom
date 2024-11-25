/*
 * MOTOReLEME.c
 *
 *  Created on: Nov 23, 2024
 *      Author: Elias Nacif, Vincent, Everson Elias
*/

#include "main.h"
#include "MOTOReLEME.h" // Include rudder control
uint16_t posicaoAtualLeme = 94;

/**
  * @brief Função de mudança de posicionamento do leme
  * @param1 timer (TIM_HandleTypeDef) : Aponta para o timer usado no leme
  * @param2 posicaoNova (uint16_t) : Posição angular nova do leme que varia de 44 (-90°) até 144 (90°)
  *        com ponto neutro sendo 100 (0°)
  * @param1 velocidade (uint8_t) : Velocidade de giro do Leme [1 a 10]
  * @retval None
  */

void AtualizarLeme(TIM_HandleTypeDef timer, uint16_t posicaoNova, uint8_t velocidade){
	int i = 0;

	if(posicaoNova <= posicaoAtualLeme){
		for(i=0;i<(posicaoAtualLeme-posicaoNova)+1; i+=1){
			setPWM(timer, TIM_CHANNEL_1, 1250, posicaoAtualLeme-i);
			HAL_Delay(30);
		}
		posicaoAtualLeme = posicaoNova;
	}
	else if(posicaoNova > posicaoAtualLeme){
		for(i=0; i<(posicaoNova-posicaoAtualLeme)+1; i+=1){
			setPWM(timer, TIM_CHANNEL_1, 1250, posicaoAtualLeme+i);
			HAL_Delay(30);
		}
		posicaoAtualLeme = posicaoNova;
	}
}
