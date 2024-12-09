#include "print_readings.h"

// Define the 'message' variable here
char message[100] = "";

void UART_TransmitMessage(char *message, UART_HandleTypeDef huart2) {
    HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), 1000);
}
