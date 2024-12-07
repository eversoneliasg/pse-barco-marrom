/*
 * Motor.c
 *
 *  Created on: Dec 6, 2024
 *      Authors: Elias Nacif, Everson Elias, Vincent Pena
 */
#include "motor.h"
#include "main.h"

/* Motor control function to manage motor state */
void Motor_Control(TIM_HandleTypeDef *timer, bool enable, bool direction, uint16_t speed) {
  if (enable) {
    // Clamp speed to the allowed range
    if (speed > MOTOR_SPEED_MAX) {
      speed = MOTOR_SPEED_MAX;
    } else if (speed < MOTOR_SPEED_MIN) {
      speed = MOTOR_SPEED_MIN;
    }

    // Set motor direction
    HAL_GPIO_WritePin(L293D_C_LK_GPIO_Port, L293D_C_LK_Pin, direction ? GPIO_PIN_SET : GPIO_PIN_RESET);

    // Set PWM duty cycle for speed control (PA7)
    __HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_2, speed);

    // Start PWM
    HAL_TIM_PWM_Start(timer, TIM_CHANNEL_2);
  } else {
    // Stop motor by disabling PWM
    HAL_TIM_PWM_Stop(timer, TIM_CHANNEL_2);
  }
}


