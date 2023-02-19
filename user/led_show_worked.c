


#include "includes.h"

/*
	工作指示灯   PB4 低电平点亮
*/



void Led_Show_Work_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE); 						 
//=============================================================================
//LED -> PC13
//=============================================================================			 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void Led_Show_Work_On(void)
{	
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void Led_Show_Work_Off(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}



//工作灯闪烁
void Task_Led_Show_Work(void)
{
	static uint8_t n = 0;
	
	if(n%2)
		Led_Show_Work_On();
	else
		Led_Show_Work_Off();
	
	n++;
}



