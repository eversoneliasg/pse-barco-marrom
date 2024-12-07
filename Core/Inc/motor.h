/*
 * motor.h
 *
 *  Created on: Nov 23, 2024
 *      Author: Elias Nacif, Vincent, Everson Elias
*/

#ifndef MOTOR_H
#define MOTOR_H

/* Maximum and minimum speed limits */
#define MOTOR_SPEED_MIN 0
#define MOTOR_SPEED_MAX 1000

/* Function prototypes */
void Motor_Control(TIM_HandleTypeDef *timer, bool enable, bool direction, uint16_t speed);

#endif // MOTOR_H
