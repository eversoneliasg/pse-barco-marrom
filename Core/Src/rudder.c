/*
 * rudder.h
 *
 *  Created on: Nov 23, 2024
 *      Author: Elias Nacif, Vincent, Everson Elias
*/
#include "rudder.h"

uint16_t currentRudderPosition = 94;

void AtualizarLeme(TIM_HandleTypeDef timer, uint16_t newPosition, uint8_t speed) {
	/* Limit the desired position to the minimum and maximum values */
    if (newPosition < RUDDER_MIN) {
    	newPosition = RUDDER_MIN;
    } else if (newPosition > RUDDER_MAX) {
    	newPosition = RUDDER_MAX;
    }
    /* Calculation of delay in milliseconds based on speed */
    uint32_t delayMs = 30 / (speed > 0 ? speed : 1);

    int i = 0;
    if (newPosition <= currentRudderPosition) {
        for (i = 0; i <= (currentRudderPosition - newPosition); i++) {
            setPWM(timer, TIM_CHANNEL_1, 1250, currentRudderPosition - i);
            HAL_Delay(delayMs);
        }
    } else {
        for (i = 0; i <= (newPosition - currentRudderPosition); i++) {
            setPWM(timer, TIM_CHANNEL_1, 1250, currentRudderPosition + i);
            HAL_Delay(delayMs);
        }
    }
    /* Update the current rudder position */
    currentRudderPosition = newPosition;
}

void setPWM(TIM_HandleTypeDef timer, uint32_t channel, uint16_t prescaler, uint16_t pulse) {
	/* Set the PWM on the specified channel */
    TIM_OC_InitTypeDef sConfigOC = {0};
    timer.Instance->PSC = prescaler;
    timer.Instance->ARR = 20000; // Standard period for servo PWM (20 ms)
    timer.Instance->CCR1 = pulse; // Update the pulse (angle)
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = pulse;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    HAL_TIM_PWM_ConfigChannel(&timer, &sConfigOC, channel);
    HAL_TIM_PWM_Start(&timer, channel);
}


