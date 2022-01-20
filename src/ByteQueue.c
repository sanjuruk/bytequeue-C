#include "ByteQueue.h"

int GetFirstDataPos(circular_buffer* cbuf)
{
	return ((cbuf->lastBufferPos + 1) == cbuf->BUFFER_SIZE) ? 0 : cbuf->lastBufferPos + 1;
}

int GetLastDataPos(circular_buffer* cbuf)
{
	return (cbuf->firstBufferPos == 0) ? cbuf->BUFFER_SIZE - 1 : cbuf->firstBufferPos - 1;
}

int Count(circular_buffer* cbuf)
{
	int temp = cbuf->firstBufferPos - cbuf->lastBufferPos - 1;

	if (temp < 0)
	{
		return temp + cbuf->BUFFER_SIZE;
	}
	else
	{
		return temp;
	}
}

int CountUntilACertainIndex(circular_buffer* cbuf, int index)
{
	int temp = index - cbuf->lastBufferPos;
	if (temp < 0)
	{
		return temp + cbuf->BUFFER_SIZE;
	}
	else
	{
		return temp;
	}
}

int EmptyCount(circular_buffer* cbuf)
{
	return cbuf->BUFFER_SIZE - 1 - Count(cbuf);
}

void Enqueue(circular_buffer* cbuf, uint8_t* data, size_t addSize)
{
	/*Exception: Buffer is full*/
	if (addSize > EmptyCount(cbuf))
		return;
	
	int lastEmptySize = cbuf->BUFFER_SIZE - cbuf->firstBufferPos;

	if (lastEmptySize > addSize)
	{
		memcpy(cbuf->buffer + cbuf->firstBufferPos, data, addSize);
		cbuf->firstBufferPos += addSize;
	}
	else
	{
		memcpy(cbuf->buffer + cbuf->firstBufferPos, data, lastEmptySize);
		memcpy(cbuf->buffer, data + lastEmptySize, addSize - lastEmptySize);
		cbuf->firstBufferPos = addSize - lastEmptySize;
	}
}

void EnqueueFromCertainIndex(circular_buffer* cbuf, uint8_t* data, int index, int length)
{
	uint8_t* temp = malloc(length);
	memcpy(temp, data + index, length);
	Enqueue(cbuf, temp, length);
	free(temp);
}

uint8_t* DequeueCertainSize(circular_buffer* cbuf, int size)
{
	uint8_t* data = GetDataCertainSize(cbuf, size);

	if(data != NULL){
	cbuf->lastBufferPos += size;
	if (cbuf->lastBufferPos >= cbuf->BUFFER_SIZE)
		cbuf->lastBufferPos -= cbuf->BUFFER_SIZE;
	}

	return data;
}

uint8_t* DequeueUntilACertainIndex(circular_buffer* cbuf, int index, int* dequedBufferSize)
{
	int dequeuingBufferSize = CountUntilACertainIndex(cbuf, index);
	*dequedBufferSize = dequeuingBufferSize;
	return DequeueCertainSize(cbuf, dequeuingBufferSize);
}

uint8_t* Dequeue(circular_buffer* cbuf, int* dequedBufferSize)
{
	int dequeuingBufferSize = Count(cbuf);
	*dequedBufferSize = dequeuingBufferSize;
	return DequeueCertainSize(cbuf, dequeuingBufferSize);
}

uint8_t* GetData(circular_buffer* cbuf)
{
	return GetDataCertainSize(cbuf, Count(cbuf));
}

uint8_t* GetDataCertainSize(circular_buffer* cbuf, int size)
{
	return GetDataCetainSizeAndFromIndex(cbuf, 0, size);
}

uint8_t* GetDataCetainSizeAndFromIndex(circular_buffer* cbuf, int startIndex, int size)
{
	uint8_t* data;
	//data = malloc(size);
	if (size <= 0)
		return NULL;

	int availableCount= Count(cbuf);
	if (size + startIndex > availableCount) {
		return NULL; 
		/*Exception: Buffer is empty*/
	}

	int firstDataPos =  GetFirstDataPos(cbuf);
	//int lastDataPos = GetLastDataPos(cbuf);
	int lastDataLength = cbuf->BUFFER_SIZE - firstDataPos;

	data = malloc(size);

	if (data == NULL) {
		/*"Byte Queue NULL malloc buffer*/
		free(data);
		return NULL;
	}

	if (size + startIndex <= lastDataLength)
	{
		memcpy(data, cbuf->buffer + (firstDataPos + startIndex), size);
	}
	else
	{
		if (startIndex < lastDataLength)
		{
			memcpy(data, cbuf->buffer + (firstDataPos + startIndex), lastDataLength - startIndex);
			memcpy(data + (lastDataLength - startIndex), cbuf->buffer, size - lastDataLength + startIndex);
		}
		else
		{
			memcpy(data, cbuf->buffer + (startIndex - lastDataLength), size);
		}
	}

	return data;
}

void InitBufferSize(circular_buffer* cbuf, uint8_t* buffer, int size)
{
	cbuf->BUFFER_SIZE = size;

	cbuf->buffer = buffer;

	cbuf->firstBufferPos = 0;

	cbuf->lastBufferPos = size - 1;
}

int IncreasePosition(circular_buffer* cbuf, int i) //ref int i
{
	i++;
	if (i == cbuf->BUFFER_SIZE)
		i = 0;

	return i;
}
