/**
 * @file ble_driver.c
 * @author Pierre Victor
 *         Mariana Leite
 * @brief Implementation of Bluetooth 4.2 BLE driver.
 * @version 0.1
 * @date 2023-11-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "BLE.h"
#include "print_readings.h"


char *atInstructions[] = {
    "AT+NAME",
    "AT+BAUD",
    "AT+ROLE",
    "AT+PARITY",
    "AT+USTP",
    "AT+INQ"
};

uint8_t uartBuffer[MAX_SIZE_UART_BUFFER];

scan_t gScan;

void ble_Init(BLE_HandleTypeDef* handler)
{
    ble_SetName(handler->huart, handler->name);
    ble_SetRole(handler->huart, handler->role);
    ble_SetBaudRate(handler->huart, handler->baudRate);
    ble_SetParity(handler->huart, handler->parity);
    ble_SetStopBit(handler->huart, handler->stopBit);

    HAL_Delay(200);

    gScan.size = 0;
    memset(uartBuffer, 0, sizeof(uartBuffer));
    __HAL_UART_ENABLE_IT(handler->huart, UART_IT_IDLE);
    HAL_UART_Receive_DMA(handler->huart, uartBuffer, sizeof(uartBuffer));
}

void ble_SendData(UART_HandleTypeDef* huart, char *data)
{
    char package[MAX_SIZE_DATA + 4];
    memset(package, 0, MAX_SIZE_DATA + 4);
    sprintf(package, "%s\r\n", data);
    HAL_UART_Transmit(huart, (uint8_t*)package, strlen(package), HAL_MAX_DELAY);
}

void ble_SetName(UART_HandleTypeDef* huart, char *name)
{
    char *instruction = atInstructions[SET_NAME];
    char data[MAX_SIZE_DATA];
    memset(data, 0, MAX_SIZE_DATA);
    sprintf(data, "%s%s", instruction, name);
    ble_SendData(huart, data);
}

void ble_SetBaudRate(UART_HandleTypeDef* huart, BaudRate_t baudRate)
{
    char *instruction = atInstructions[SET_BAUD_RATE];
    char data[MAX_SIZE_DATA];
    memset(data, 0, MAX_SIZE_DATA);
    sprintf(data, "%s%d", instruction, baudRate);
    ble_SendData(huart, data);
}

void ble_SetRole(UART_HandleTypeDef* huart, RoleParam_t role)
{
    char *instruction = atInstructions[SET_ROLE];
    char data[MAX_SIZE_DATA];
    memset(data, 0, MAX_SIZE_DATA);
    sprintf(data, "%s%d", instruction, role);
    ble_SendData(huart, data);
}

void ble_SetParity(UART_HandleTypeDef* huart, ParityParam_t parity)
{
    char *instruction = atInstructions[SET_PARITY];
    char data[MAX_SIZE_DATA];
    memset(data, 0, MAX_SIZE_DATA);
    sprintf(data, "%s%d", instruction, parity);
    ble_SendData(huart, data);
}

void ble_SetStopBit(UART_HandleTypeDef* huart, uint8_t stopBit)
{
    char *instruction = atInstructions[SET_STOP_BIT];
    char data[MAX_SIZE_DATA];
    memset(data, 0, MAX_SIZE_DATA);
    sprintf(data, "%s%d", instruction, stopBit);
    ble_SendData(huart, data);
}

void ble_InquireDevices(UART_HandleTypeDef* huart)
{
    char *instruction = atInstructions[MASTER_SCAN_SLAVE];
    char data[MAX_SIZE_DATA];
    memset(data, 0, MAX_SIZE_DATA);
    sprintf(data, "%s", instruction);
    ble_SendData(huart, data);
}


void ble_GetScannedDevices(scan_t* scan)
{
    *scan = gScan;
}

void ble_LoadDeviceInfo(char* start, char* end, device_t* device)
{
    size_t size = end - start;
    char temp[size];
    memcpy(temp, start, size);

    char* macAddressPointer = temp + 7;
    char* rssiPointer = macAddressPointer + MAC_ADDRESS_SIZE + 1;
    char* namePointer = strstr(rssiPointer, ",") + 1;

    size_t rssiSize = namePointer - rssiPointer - 1;
    char rssiStr[rssiSize];
    memcpy(rssiStr, rssiPointer, rssiSize);

    memcpy(device->mac, macAddressPointer, MAC_ADDRESS_SIZE);
    memcpy(device->name, namePointer, MAX_DEVICE_NAME_SIZE);
    device->rssi = atoi(rssiStr);
}



void ble_ParseScanResponse(char* scanResponse, scan_t* scan)
{
    char *start, *end;
    scan->size = 0;


    if((start = strstr(scanResponse, INIT_MODULE_RESPONSE_SCAN))) {
        while((end = strstr(start + 1, INIT_MODULE_RESPONSE_SCAN))) {

            ble_LoadDeviceInfo(start, end, &scan->devices[scan->size]);
            start = end;
            scan->size++;
        }
        if((end = strstr(start + 1, END_RESPONSE_SCAN))) {
            ble_LoadDeviceInfo(start, end, &scan->devices[scan->size]);
            scan->size++;
        }

    }

}


//void ble_ConnectToDevice(char* deviceName, UART_HandleTypeDef* huart)
//{
//    // Check if the device name matches "Vpena" and initiate connection
//    for (int i = 0; i < gScan.size; i++) {
//        if (strcmp(gScan.devices[i].name, deviceName) == 0) {
//            // Initiate connection process here (send appropriate AT command)
//            char connectCmd[MAX_SIZE_DATA];
//            memset(connectCmd, 0, MAX_SIZE_DATA);
//            sprintf(connectCmd, "AT+CONNECT=%s", gScan.devices[i].mac);  // Assuming mac address is used to connect
//            ble_SendData(huart, connectCmd);
//            break;
//        }
//    }
//}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))
    {
        __HAL_UART_CLEAR_IDLEFLAG(huart);


        if (strstr((char *)uartBuffer, END_RESPONSE_SCAN)) {
            HAL_UART_DMAStop(huart);

            // Parse the scan response if the END_RESPONSE_SCAN string is found
            ble_ParseScanResponse((char *)uartBuffer, &gScan);

            memset(uartBuffer, 0, sizeof(uartBuffer));
            HAL_UART_Receive_DMA(huart, uartBuffer, sizeof(uartBuffer));
        }
    }
}



void DisplayScannedDevices(scan_t* scan,UART_HandleTypeDef huart2) {

    for (int i = 0; i < 4; i++) {
        device_t* device = &scan->devices[i];

        // Format device information into the message buffer
        snprintf(message, sizeof(message),
                 "Device %d:\r\nMAC: %s\r\nName: %s\r\nRSSI: %d\r\n\r\n",
                 i + 1, device->mac, device->name, device->rssi);

        // Transmit the message via UART
        UART_TransmitMessage(message, huart2);
    }
}
