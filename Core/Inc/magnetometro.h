/*
 * MAGNETOMETRO.h
 *
 *  Created on: Dec 4, 2024
 *      Author: Elias Nacif, Everson Elias, Vincent
 */

#ifndef INC_MAGNETOMETRO_H_
#define INC_MAGNETOMETRO_H_


typedef struct {
	int16_t x;
	int16_t y;
} XYCoordinates;

void HMC5883L_Init(I2C_HandleTypeDef *hi2c); //Inicialização

void HMC5883L_ReadData(I2C_HandleTypeDef *hi2c, XYCoordinates *reading[8]); //Leitura

uint8_t Filter_Data(XYCoordinates *reading[8], XYCoordinates *result[8]); //Filtragem de valores expúrios

float Measure_Distance(XYCoordinates a, XYCoordinates b); //Devolve a distância entre dois pontos

XYCoordinates Data_Means(XYCoordinates reading[8], uint8_t n); //Devolve a Média dos valores lidos

XYCoordinates Calibrate_Data(XYCoordinates reading, XYCoordinates Delta); //Calibração

float XY_to_Degrees(XYCoordinates *coordinates);

uint16_t RudderDegree(uint16_t NewDirection);

#endif /* INC_MAGNETOMETRO_H_ */
