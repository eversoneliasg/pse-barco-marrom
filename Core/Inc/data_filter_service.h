/**
 * @file data_filter_service.h
 * @author Mariana Leite
 * @brief 
 * @version 0.1
 * @date 2023-11-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __DATA_FILTER_SERVICE_H
#define __DATA_FILTER_SERVICE_H

#include <stdint.h>
#include <memory.h>

#define MAX_BUFFER_SIZE 10

typedef struct {
	float data[MAX_BUFFER_SIZE];
	float sum;
	size_t size;
	size_t position;
} buffer_t;

void DataFilterService_InitBuffer(buffer_t* buffer);

/**
 * @brief Calculates moving average from buffer
 * 
 * @param buffer buffer_t pointer to data to be filtered
 * @param newValue float value to be inserted in the buffer
 * @return float value representing the moving average from buffer
 */
float DataFilterService_MovingAverage(buffer_t* buffer, float newValue);

#endif /* __DATA_FILTER_SERVICE_H */
