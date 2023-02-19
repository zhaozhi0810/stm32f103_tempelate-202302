

#include "includes.h"


/*

*/



/*!
	1ms定时器
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
    /* update the g_localtime by adding SYSTEMTICK_PERIOD_MS each SysTick interrupt */
    systick_int_update();
}


/*
	外部中断12，外部中断13，是0-16号按键的中断,暂不使用
*/
void EXTI15_10_IRQHandler(void)
{
//	if(SET == EXTI_GetITStatus(EXTI_Line12))   //外部中断12，按键1-9触发
//	{
//		btn_start_scan = 1;   //开始扫描，左边按键 iic2
//		
//		//用定时器扫描吧，消抖
//		EXTI_ClearITPendingBit(EXTI_Line12);
//	}
//	else if(SET == EXTI_GetITStatus(EXTI_Line13))   //外部中断13，按键10-18触发
//	{
//		btn_start_scan = 10;   //开始扫描，右边按键 iic1
//		
//		//用定时器扫描吧，消抖
//		EXTI_ClearITPendingBit(EXTI_Line13);
//	}
}


/*
	2021-12-09,
	外部中断0，是视频切换按键的中断
*/
void EXTI0_IRQHandler(void)
{
//	exint0_handle();
//	//用定时器扫描吧，消抖
//	EXTI_ClearFlag(EXTI_Line0);

}


/*
	2021-12-09,
	外部中断1，是电源按键的中断
*/
void EXTI1_IRQHandler(void)
{
//	exint1_handle();
//	//用定时器扫描吧，消抖
//	EXTI_ClearFlag(EXTI_Line1);	
}



/*!
    \brief      串口2的中断处理函数，开启了接收中断和空闲中断
				串口2主要是与cpu进行数据通信
    \param[in]  none
    \param[out] none
    \retval     none
*/
//void USART2_IRQHandler(void)
//{	
//	if(USART_GetITStatus(USART2, USART_IT_RXNE))
//	{
//		com_rne_int_handle();
//		USART_ClearITPendingBit(USART2, USART_IT_RXNE);   //清中断标志
//	}
//	else if(USART_GetITStatus(USART2, USART_IT_IDLE))  //空闲中断，表示一帧数据已结束
//	{
//		//解析命令，并处理。
//		USART_ClearITPendingBit(USART2, USART_IT_IDLE);  //清中断标志
//	}
//}


//运行状态改变中断
void EXTI9_5_IRQHandler(void)
{
//	if(SET == EXTI_GetITStatus(EXTI_Line8) || SET == EXTI_GetITStatus(EXTI_Line9))
//	{	
//		exint8_9_handle();
//		EXTI_ClearITPendingBit(EXTI_Line8);
//		EXTI_ClearITPendingBit(EXTI_Line9);
//	}
}




//void USART1_IRQHandler(void)
//{	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE))
//	{
//	//	com1_rne_int_handle();
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);   //清中断标志
//	}
//	else if(USART_GetITStatus(USART1, USART_IT_IDLE))  //空闲中断，表示一帧数据已结束
//	{
//		//解析命令，并处理。
//		USART_ClearITPendingBit(USART1, USART_IT_IDLE);  //清中断标志
//	}
//#ifdef 	UART1_SEND_IRQ
//	else if(USART_GetITStatus(USART1, USART_IT_TXE))  //发送缓存空
//	{
//		com1_send_irq();
//		USART_ClearITPendingBit(USART1, USART_IT_TXE);  //清中断标志
//	}
//#endif	
//}



void USART3_IRQHandler(void)
{	
//	if(USART_GetITStatus(USART3, USART_IT_RXNE))
//	{
//		com3_rne_int_handle();
//		USART_ClearITPendingBit(USART3, USART_IT_RXNE);   //清中断标志
//		USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);   //允许空闲中断,不使用空闲中断
//	}
//	else if(USART_GetITStatus(USART3, USART_IT_IDLE))  //空闲中断，表示一帧数据已结束
//	{
//		USART_ITConfig(USART3, USART_IT_IDLE, DISABLE);   //禁止空闲中断，只要一次空闲中断就好了！！！
//		//解析命令，并处理。
//		com3_frame_handle();
//		USART_ClearITPendingBit(USART3, USART_IT_IDLE);  //清中断标志		
//	}
}


