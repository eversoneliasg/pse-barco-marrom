#ifndef INC_PRINT_READINGS_H_
#define INC_PRINT_READINGS_H_
#include "main.h"
#include "string.h"

// Declare 'message' as extern
extern char message[100];

// Function to transmit a message over UART
void UART_TransmitMessage(char *message, UART_HandleTypeDef huart2);

#endif /* INC_PRINT_READINGS_H_ */
