/**
 * @file jdy18_driver.h
 * @author Pierre Victor
 *         Mariana Leite
 * @brief Driver to Bluetooth 4.2 BLE module.
 * @version 0.1
 * @date 2023-10-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __JDY18_DRIVER_H
#define __JDY18_DRIVER_H

#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_MODULE_RESPONSE_SCAN "+DEV:"
#define END_RESPONSE_SCAN "+STOP:SCAN"

#define MAX_SIZE_DATA 30
#define MAX_SIZE_UART_BUFFER 1000
#define MAX_DEVICE_LIST 10
#define MAC_ADDRESS_SIZE 12
#define MAX_DEVICE_NAME_SIZE 30

typedef enum {
	SET_NAME = 0,
	SET_BAUD_RATE,
	SET_ROLE,
	SET_PARITY,
	SET_STOP_BIT,
	MASTER_SCAN_SLAVE,
	AT_INSTRUCTIONS_SIZE
} AtInstruction_t;

typedef enum {
	BAUD_1200 = 1,
	BAUD_2400,
	BAUD_4800,
	BAUD_9600,
	BAUD_19200,
	BAUD_38400,
	BAUD_57600,
	BAUD_115200,
	BAUD_230400
} BaudRate_t;

typedef enum {
	SLAVE = 0,
    MASTER = 1,
    IBEACON = 3,
} RoleParam_t;

typedef enum {
	NO_PARITY = 0,
    ODD_PARITY,
    EVEN_PARITY,
} ParityParam_t;

typedef struct {
	UART_HandleTypeDef* huart;
	char* name;
	BaudRate_t baudRate;
	RoleParam_t role;
	ParityParam_t parity;
	uint8_t stopBit;
} JDY18_HandleTypeDef;

typedef struct {
	char name[MAX_DEVICE_NAME_SIZE];
	char mac[MAC_ADDRESS_SIZE];
	int rssi;
} device_t;

typedef struct {
	device_t devices[MAX_DEVICE_LIST];
	size_t size;
} scan_t;

/**
 * @brief Initialize the bluetooth module.
 * 
 * @param handler JDY18_HandleTypeDef pointer to module handler.
 */
void JDY18Driver_Init(JDY18_HandleTypeDef* handler);

/**
 * @brief Send data to Bluetooth module.
 * 
 * @param huart UART_HandleTypeDef pointer to UART handler associated to the Bluetooth module.
 * @param data Data to send to Bluetooth module.
 */
void JDY18Driver_SendData(UART_HandleTypeDef* huart, char *data);

/**
 * @brief Set the name of Bluetooth module.
 * 
 * @param huart UART_HandleTypeDef pointer to UART handler associated to the Bluetooth module.
 * @param name  Name of Bluetooth module.
 */
void JDY18Driver_SetName(UART_HandleTypeDef* huart, char *name);

/**
 * @brief Set the baud rate of Bluetooth module.
 * 
 * @param huart UART_HandleTypeDef pointer to UART handler associated to the Bluetooth module.
 * @param baudRate Baud rate of Bluetooth module.
 */
void JDY18Driver_SetBaudRate(UART_HandleTypeDef* huart, BaudRate_t baudRate);

/**
 * @brief Set the role of Bluetooth module.
 * 
 * @param huart UART_HandleTypeDef pointer to UART handler associated to the Bluetooth module.
 * @param role Role of Bluetooth module.
 */
void JDY18Driver_SetRole(UART_HandleTypeDef* huart, RoleParam_t role);

/**
 * @brief Set the parity of Bluetooth module.
 * 
 * @param huart UART_HandleTypeDef pointer to UART handler associated to the Bluetooth module.
 * @param parity Parity of Bluetooth module.
 */
void JDY18Driver_SetParity(UART_HandleTypeDef* huart, ParityParam_t parity);

/**
 * @brief Set the stop bit of Bluetooth module.
 * 
 * @param huart UART_HandleTypeDef pointer to UART handler associated to the Bluetooth module.
 * @param stopBit Stop bit of Bluetooth module.
 */
void JDY18Driver_SetStopBit(UART_HandleTypeDef* huart, uint8_t stopBit);

/**
 * @brief Master mode: Inquire Bluetooth devices.
 * 
 * @param huart UART_HandleTypeDef pointer to UART handler associated to the Bluetooth module.
 */
void JDY18Driver_InquireDevices(UART_HandleTypeDef* huart);

/**
 * @brief Return the latest scanned devices.
 * 
 * @param scan scan_t pointer to structure where devices information are located.
 */
void JDY18Driver_GetScannedDevices(scan_t* scan);

/**
 * @brief Populate device with information available in string.
 * 
 * @param start char pointer to the start of the string.
 * @param end  char pointer to the end of the string.
 * @param device device_t pointer to structure to be populated.
 */
void JDY18Driver_LoadDeviceInfo(char* start, char* end, device_t* device);

/**
 * @brief Parse scan response.
 * 
 * @param scanResponse char pointer to complete scan response.
 * @param scan scan_t pointer to structure that should be populated.
 */
void JDY18Driver_ParseScanResponse(char* scanResponse, scan_t* scan);

/**
 * @brief Rx Transfer completed callbacks.
 *
 * @param huart UART_HandleTypeDef pointer to UART handler associated to the Bluetooth module.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif /* __JDY18_DRIVER_H */
