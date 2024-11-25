/*
 * MOTOReLEME.h
 *
 *  Created on: Nov 23, 2024
 *      Author: Elias Nacif, Vincent, Everson Elias
 */

#ifndef INC_MOTORELEME_H_
#define INC_MOTORELEME_H_

// ==== VARIAVEIS ====
extern uint16_t posicaoAtualLeme;
// ==== FUNCOES ====

void AtualizarLeme(TIM_HandleTypeDef timer, uint16_t posicaoNova, uint8_t velocidade);

void AtualizarMotor(TIM_HandleTypeDef timer, uint16_t velocidade);



#endif /* INC_MOTORELEME_H_ */
