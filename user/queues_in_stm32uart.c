/*
	主要用于串口的缓存。
*/


#include "queues_in_stm32uart.h"
#define NULL ((void*)0)



#define UART_BUFF_FULL_COVER


//队列插入数据
int32_t QueueUARTDataInsert(Queue_UART_STRUCT *Queue,uint8_t data)
{
	if(MAX_QUEUE_UART_LEN == Queue->length)
	{
#ifdef UART_BUFF_FULL_COVER
		Queue->sendIndex = (Queue->sendIndex + 1) % MAX_QUEUE_UART_LEN;
		Queue->length--;
#else
		return -1;
#endif
	}
	
	Queue->dataBuf[Queue->recvIndex] = data;
	
	Queue->recvIndex = (Queue->recvIndex + 1) % MAX_QUEUE_UART_LEN;
	
	Queue->length++;
	
	return 0;
}

int32_t QueueUARTDataDele(Queue_UART_STRUCT *Queue,uint8_t *data)
{
	if(0 == Queue->length)
		return -1;
	
	*data = Queue->dataBuf[Queue->sendIndex];
	
	Queue->sendIndex = (Queue->sendIndex + 1) % MAX_QUEUE_UART_LEN;
	
	Queue->length--;
	
	return 0;
}

void QueueUARTDataIndexRecover(Queue_UART_STRUCT *Queue)
{
	Queue->sendIndex = (Queue->sendIndex - 1) % MAX_QUEUE_UART_LEN;
	
	Queue->length++;
}

uint32_t QueueUARTDataLenGet(Queue_UART_STRUCT *Queue)
{
	return Queue->length;
}

uint8_t checksum(uint8_t *buf, uint8_t len)
{
	uint8_t sum;
	uint8_t i;

	for(i=0,sum=0; i<len; i++)
		sum += buf[i];

	return sum;
}



//校验数据
int32_t verify_data(uint8_t *data,uint8_t len)
{
	uint8_t check;
	int32_t ret = -1;
	
	if(data == NULL)
		return -1;
	
	//读取原数据中的校验值
	check = data[len - 1];
	
	//重新计算校验值
	if(check==checksum(data,len - 1))
		ret = 0;
	
	return ret;
}


