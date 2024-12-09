/*
 * MAGNETOMETRO.c
 *
 *  Created on: Dec 4, 2024
 *      Author: Elias Nacif, Everson Elias, Vincent
 */

#include <math.h>
#include "main.h"
#include "MAGNETOMETRO.h"
#include "print_readings.h"
#include <stdio.h>

/*************************** I2C Device Address ********************************/

#define HMC5883L_ADDRESS          0x1E<<1

/****************************** Registers ************************************/

#define REG_CONFIG_A              0x00
#define REG_CONFIG_B              0x01
#define REG_MODE                  0x02
#define REG_DATA_OUT_X_MSB        0x03
#define REG_DATA_OUT_X_LSB        0x04
#define REG_DATA_OUT_Z_MSB        0x05
#define REG_DATA_OUT_Z_LSB        0x06
#define REG_DATA_OUT_Y_MSB        0x07
#define REG_DATA_OUT_Y_LSB        0x08
#define REG_STATUS                0x09
#define REG_IDENT_A               0x0A
#define REG_IDENT_B               0x0B
#define REG_IDENT_C               0x0C


// Configuration Register A
// Layout for the bits of Configuration Register A (CRA)
#define HMC5883L_CRA_SA_POS       6 // SAMPLE AVERAGE position on CRA
#define HMC5883L_CRA_SA_TAM       2 // LENGTH of bits
#define HMC5883L_CRA_DATARATE_POS 4 // DATA RATE position on CRA
#define HMC5883L_CRA_DATARATE_TAM 3
#define HMC5883L_CRA_MM_POS       1 // Measurement Mode position on CRA
#define HMC5883L_CRA_MM_TAM       2

// Samples averaged
#define HMC5883L_SAMPLE_AVERAGE_1 0x00 //(Default)
#define HMC5883L_SAMPLE_AVERAGE_2 0x01
#define HMC5883L_SAMPLE_AVERAGE_4 0x02
#define HMC5883L_SAMPLE_AVERAGE_8 0x03

// Typical Data Output Rate (Hz)
#define HMC5883L_DATARATE_0_75_HZ 0x00
#define HMC5883L_DATARATE_1_5HZ   0x01
#define HMC5883L_DATARATE_3HZ     0x02
#define HMC5883L_DATARATE_7_5HZ   0x03
#define HMC5883L_DATARATE_15HZ    0x04 //(Default)
#define HMC5883L_DATARATE_30HZ    0x05
#define HMC5883L_DATARATE_75HZ    0x06

// Measurement Mode
#define HMC5883L_MEAS_MODE_NORMAL 0x00 //(Default)
#define HMC5883L_MEAS_MODE_POS    0x01
#define HMC5883L_MEAS_MODE_NEG    0x02


// Configuration Register B
// Layout for the bits of Configuration Register B (CRB)
#define HMC5883L_CRB_RANGE_POS    7 // RANGE position on CRB
#define HMC5883L_CRB_RANGE_TAM    3 // LENGTH of bits

// RANGE -> GAIN
#define HMC5883L_RANGE_0_88GA   	0x00
#define HMC5883L_RANGE_1_3GA	    0x01 //(Default)
#define HMC5883L_RANGE_1_9GA	    0x02
#define HMC5883L_RANGE_2_5GA      0x03
#define HMC5883L_RANGE_4GA        0x04
#define HMC5883L_RANGE_4_7GA      0x05
#define HMC5883L_RANGE_5_7GA      0x06
#define HMC5883L_RANGE_8_1GA      0x07

// Mode Register
// Layout for the bits of Mode Register
#define HMC5883L_MODEREG_POS      1
#define HMC5883L_MODEREG_TAM      2

#define HMC5883L_MODE_CONTINUOUS    0x00
#define HMC5883L_MODE_SINGLE        0x01
#define HMC5883L_MODE_IDLE          0x02
uint8_t mode = HMC5883L_MODE_CONTINUOUS;                                  // Modo contínuo

void HMC5883L_Init(I2C_HandleTypeDef *hi2c){
	uint8_t configA = (HMC5883L_SAMPLE_AVERAGE_8 << HMC5883L_CRA_SA_POS) |   // 8 amostras por leitura
	                      (HMC5883L_DATARATE_15HZ << HMC5883L_CRA_DATARATE_POS) | // Taxa de 15Hz
	                      (HMC5883L_MEAS_MODE_NORMAL << HMC5883L_CRA_MM_POS);     // Modo de medição normal
	uint8_t configB = (HMC5883L_RANGE_1_3GA << HMC5883L_CRB_RANGE_POS);       // Ganho de ±1.3 Gauss



    // Write to Configuration Register A
    HAL_I2C_Mem_Write(hi2c, HMC5883L_ADDRESS, REG_CONFIG_A, 23, &configA, 1, HAL_MAX_DELAY);

    // Write to Configuration Register B
    HAL_I2C_Mem_Write(hi2c, HMC5883L_ADDRESS, REG_CONFIG_B, 1, &configB, 1, HAL_MAX_DELAY);

    // Write to Mode Register
    HAL_I2C_Mem_Write(hi2c, HMC5883L_ADDRESS, REG_MODE, 1, &mode, 1, HAL_MAX_DELAY);
}

void HMC5883L_ReadData(I2C_HandleTypeDef *hi2c, XYCoordinates reading[8]){
	uint8_t rawData[6];
    for (int i = 0; i < 8; i++) {
        // Lê 6 bytes a partir do registrador REG_DATA_OUT_X_MSB
        HAL_I2C_Mem_Read(hi2c, HMC5883L_ADDRESS, REG_DATA_OUT_X_MSB, 1, rawData, 6, HAL_MAX_DELAY);

        // Converte os dados brutos para inteiros
        reading[i].x = (int16_t)((rawData[0] << 8) | rawData[1]);  // X MSB e LSB
        reading[i].y = (int16_t)((rawData[4] << 8) | rawData[5]);  // Y MSB e LSB

    }
}

// *********************************** FUNCTIONS FOR FILTERING THE DATA ****************************

uint8_t Filter_Data(XYCoordinates reading[8]){
	const float MIN_DISTANCE = 2;
	XYCoordinates centroid = Data_Means(reading, 8);
	uint8_t resultSize=8;
	for(uint8_t i=0; i<8; i++){
		if(Measure_Distance(reading[i], centroid) > MIN_DISTANCE){
			reading[i].x = 0;
			reading[i].y = 0;
			resultSize--;
		}
	}

	return resultSize;
}

float Measure_Distance(XYCoordinates a, XYCoordinates b){
	return sqrt(pow((a.x-b.x),2)+pow((a.y-b.y),2));
}

XYCoordinates Data_Means(XYCoordinates reading[8], uint8_t n){
	int16_t meansX = 0;
	int16_t meansY = 0;
	for(uint8_t i=0; i<8; i++){
		meansX += reading[i].x;
		meansY += reading[i].y;
	}
	XYCoordinates coordinateMeans = {meansX/n, meansY/n};
	return coordinateMeans;
}

// ************************************************************************************************

//XYCoordinates Calibrate_Data(XYCoordinates reading, XYCoordinates Delta){
//	XYCoordinates calibrated = {(reading.x + Delta.x), (reading.y + Delta.y)};
//	return calibrated;
//}


int16_t XY_to_Degrees(XYCoordinates coordinate){

	 int16_t angle = atan2(coordinate.y, coordinate.x)*(180.0/M_PI);
	if(angle < 0) angle += 360;
	return (angle - 75);
}

int16_t RudderDegree(uint16_t boatAngle, int16_t arrivalAngle){
	int16_t rudderAngleShift = boatAngle - arrivalAngle;
	return (int16_t) (-(5.0/18.0)*rudderAngleShift + 94);
}
