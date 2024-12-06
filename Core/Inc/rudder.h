/*
 * rudder.h
 *
 *  Created on: Nov 23, 2024
 *      Author: Elias Nacif, Vincent, Everson Elias
*/

#ifndef RUDDER_H
#define RUDDER_H
#include "main.h"

/* Definition of rudder limits */
#define RUDDER_MIN 44  /* Minimum angle (-90°) */
#define RUDDER_MAX 144 /* Maximum angle (90°) */

/* Global variable to track the current position of the rudder */
extern uint16_t currentRudderPosition;

/**
  * @brief Function for updating the rudder position
  * @param timer (TIM_HandleTypeDef): Pointer to the timer used on the rudder
  * @param posicaoNova (uint16_t): New angular position of the rudder, limited between LEME_MIN and LEME_MAX.
  * @param speed (uint8_t): Rudder rotation speed [1 to 10]
 */
void Rudder_Update(TIM_HandleTypeDef timer, uint16_t newPosition, uint8_t speed);

/**
  * @brief Configures PWM based on the given parameters
  * @param timer (TIM_HandleTypeDef): Pointer to timer
  * @param channel (uint32_t): PWM channel
  * @param prescaler (uint16_t): Prescaler for the PWM
  * @param pulse (uint16_t): Pulse (servo angle)
  */
void setPWM(TIM_HandleTypeDef timer, uint32_t channel, uint16_t prescaler, uint16_t pulse);

#endif /* RUDDER_H */
