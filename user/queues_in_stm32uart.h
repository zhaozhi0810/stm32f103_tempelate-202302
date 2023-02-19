
#ifndef __QUEUE_IN_STM32UART_H__
#define __QUEUE_IN_STM32UART_H__

#include <stm32f10x.h>

#define UART_BUFF_FULL_COVER
#define MAX_QUEUE_UART_LEN (64)  //每个队列缓存空间


typedef struct{
	uint8_t sendIndex;
	uint8_t recvIndex;
	uint8_t length;      //接收到的数据长度，类型跟开辟的空间有关
	uint8_t dataBuf[MAX_QUEUE_UART_LEN];
}Queue_UART_STRUCT;


#endif


