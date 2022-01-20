#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	uint8_t* buffer;
	size_t firstBufferPos;
	size_t lastBufferPos;
	size_t BUFFER_SIZE;
} circular_buffer;

int Count(circular_buffer* cbuf);
int EmptyCount(circular_buffer* cbuf);
int GetLastDataPos(circular_buffer* cbuf);
void Enqueue(circular_buffer* cbuf, uint8_t* data, size_t addSize);
void EnqueueFromCertainIndex(circular_buffer* cbuf, uint8_t* data, int index, int length);
uint8_t* DequeueCertainSize(circular_buffer* cbuf, int size);
//uint8_t* Dequeue(circular_buffer* cbuf);
uint8_t* DequeueUntilACertainIndex(circular_buffer* cbuf, int index, int* dequedBufferSize);
uint8_t* Dequeue(circular_buffer* cbuf, int* dequedBufferSize);
uint8_t* GetData(circular_buffer* cbuf);
uint8_t* GetDataCertainSize(circular_buffer* cbuf, int size);
uint8_t* GetDataCetainSizeAndFromIndex(circular_buffer* cbuf, int startIndex, int size);
void InitBufferSize(circular_buffer* cbuf, uint8_t* buffer, int size);
