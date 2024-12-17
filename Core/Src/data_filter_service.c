/**
 * @file data_filter_service.c
 * @author Mariana Leite
 * @brief 
 * @version 0.1
 * @date 2023-11-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "data_filter_service.h"

void DataFilterService_InitBuffer(buffer_t* buffer)
{
	buffer->size = 0;
	buffer->sum = 0;
	buffer->position = 0;
}

float DataFilterService_MovingAverage(buffer_t* buffer, float newValue) {
	if (buffer->size <= MAX_BUFFER_SIZE) buffer->size++;
  buffer->sum += newValue - buffer->data[buffer->position];
  buffer->data[buffer->position] = newValue;
  buffer->position = (buffer->position + 1) % MAX_BUFFER_SIZE;
  return (float)buffer->sum / buffer->size;
}
