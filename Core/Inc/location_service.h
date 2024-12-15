/**
 * @file location_service.h
 * @author Pierre Victor
 *         Mariana Leite
 * @brief Service to manage location.
 * The location is calculated using UTM coordinates.
 * Can you convert latitude and longitude to UTM using the following link:
 * https://sigam.ambiente.sp.gov.br/sigam3/Controles/latlongutm.htm?latTxt=ctl00_con
 * @version 0.1
 * @date 2023-10-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __LOCATION_SERVICE_H
#define __LOCATION_SERVICE_H

#define MASTER_BLE_NAME "MASTERBOAT"
#define SLAVE_BEACON_NAME_B1 "PSE2022_B1"
#define SLAVE_BEACON_NAME_B2 "PSE2022_B2"
#define SLAVE_BEACON_NAME_B3 "PSE2022_B3"
#define PRECISION_BLE_METERS 0.5

#define MEASURED_POWER -82

#include "stm32f4xx.h"

typedef struct {
	float latitude;
	float longitude;
} location_t;

/**
 * @brief Initialize location service.
 * 
 * @param huart UART_HandleTypeDef pointer to serial handler structure.
 * @param htim TIM_HandleTypeDef pointer to timer handler structure.
 */
void LocationService_Init(UART_HandleTypeDef* huart, TIM_HandleTypeDef* htim);

/**
 * @brief Calculate the distance between the beacon and the master.
 * 
 * @param rssi RSSI of the beacon.
 * @return float Distance between the beacon and the master.
 */
float LocationService_CalculateDistance(int rssi);

/**
 * @brief Calculates and updates the system location.
 * 
 */
void LocationService_UpdateLocation();

/**
 * @brief Get the Location of the boat.
 * 
 * @return location_t with latitude and longitude in Universal Transversa de Mercator.
 */
location_t LocationService_GetLocation();

/**
 * @brief Get the angle between the arrival and the boat.
 * 
 * @return float Calculated angle.
 */
float LocationService_GetArrivalAngle();

/**
 * @brief Inform if the boat is in the destiny.
 * The precision is PRECISION_BLE_METERS, you can define in this file.
 * 
 * @return	true if the boat is in the destiny, false otherwise.
 */
uint8_t LocationService_IsInDestiny();

#endif /* __LOCATION_SERVICE_H */
