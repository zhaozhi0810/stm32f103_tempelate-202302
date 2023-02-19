/*
	2021-09-28
	
	��ʮ������ֲ���ļ���
		
	�жϴ�����ͳһ����stm32f103_it.c��
	
	2021-09-28  stm32 3�����ڶ���ʹ��
	uart1-����ʹ��
	uart2/uart3-����3A3000��һ�������ϴ��¶ȵ�ѹ����Ϣ��һ�������ϴ�18����������Ϣ���Լ�18��led�Ŀ��Ʋ���
	
	usart1 -> ����pc����Ҫ�����ڵ���
	
	
	2021-12-01.
	����1���ⲿ���ӣ���Ŀǰȫ��ע��
	
*/




#include "includes.h"
#include <stdio.h>

#ifdef  DEBUG

//static uint8_t txe_enable = 0 ;  //txe�жϿ����� 0��ʾû�п�����1��ʾ����


static Queue_UART_STRUCT Queue_UART1_Recv;
#ifdef UART1_SEND_IRQ
static Queue_UART_STRUCT Queue_UART1_Send;   //����
#endif
//static uint8_t com1_data[8];   //���ջ���


/*!
    \brief      configure COM port
    \param[in]  
      \arg        bandrate: ������
    \param[out] none
    \retval     none
*/
static void usart1_init(uint32_t bandrate)
{    
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//1.ʱ��ʹ�ܣ�����ʹ�ܣ��ⲿ�豸����ʹ�ã�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	
	//2.����IO�˿ڵ�ģʽ�����ù��ܣ�
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStruct);	
	
		
	//3.��ʼ�����ڿ�����
	USART_StructInit(& USART_InitStruct);  //��ʼ���ṹ��
	USART_InitStruct.USART_BaudRate = bandrate;
	USART_Init(USART1, &USART_InitStruct);

	//4.�������ڿ�����
	USART_Cmd(USART1, ENABLE);
	
	//5.�����ж�����
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);   //���������ж�
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);   //���տ����ж�
#ifdef UART1_SEND_IRQ
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);   //���ͻ�����ж�
#endif
	//6.����nvic�������ж�
	NVIC_InitStruct.NVIC_IRQChannel= USART1_IRQn;  
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;    //ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);

}


void  usart1_init_all(uint32_t bandrate)
{
	memset((void *)&Queue_UART1_Recv, 0, sizeof(Queue_UART_STRUCT));

	usart1_init(bandrate);   //UART2 ��LS7A uart0����
}



static void UART1_TX(uint8_t ch)
{
#ifdef UART1_SEND_IRQ	
	USART_SendData(USART1, ch); 
	if(txe_enable == 0){
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);   //�����ж�
		txe_enable = 1;  //�ж��ѿ���
	}
#else	
	while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE))  //�ȴ�ǰ������ݷ��ͽ���
		;
    USART_SendData(USART1, ch);  
#endif	
}


void UART1_TX_STRING(uint8_t *str, uint8_t length)
{
	uint8_t i;
	
	for(i = 0; i < length; i++)
		UART1_TX(str[i]);
}


/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    UART1_TX(ch);
    return ch;
}

#ifdef UART1_SEND_IRQ    //�жϵķ�ʽ����
void com1_send_irq(void)
{
	uint8_t len;
	uint8_t dat;
	len = QueueUARTDataLenGet(&Queue_UART1_Send);
	if(len)
	{
		QueueUARTDataDele(&Queue_UART1_Send,&dat);
		UART1_TX(dat);
	}
	else{
		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);   //ֻҪ�ǿգ���һֱ���жϡ�û���ݵ�ʱ���ȹر�
		txe_enable  = 0; //�ж��ѹر�
	}
}
#endif





#if 0 //2021-12-01
void com1_idle_int_handle(void)
{
	int32_t err;
	uint8_t length,i;
	
	length = QueueUARTDataLenGet(&Queue_UART1_Recv);	
	if(length < UART_PROTOCOL_FRAME_LENGTH - UART_PROTOCOL_HEAD_LENGTH)   //С��8���ֽڣ�����������һ֡
		return ;   //�����ȴ�
		
	length = UART_PROTOCOL_FRAME_LENGTH - UART_PROTOCOL_HEAD_LENGTH;   //����6���ֽ���
	for(i=0;i<length;i++)
	{
		//���ﲻ�жϴ����ˣ�ǰ���Ѿ����ȷʵ����ô���ֽڡ�
		QueueUARTDataDele(&Queue_UART1_Recv,com1_data+i+2) ;  //com_data �ճ������ֽڣ�Ϊ�˼���֮ǰ��У����㷨�������ݽ����㷨
	}
	
	if(0 == verify_data(com1_data,UART_PROTOCOL_FRAME_LENGTH))
	{
		com1_message_handle();
		//У�����ȷ��
	}	
	else  //��ʹ�յ������֡��ҲӦ�ûظ�����
	{
		printf("check sum error!");    //prinf���͵�uart1
	}	
}


/*
	�����յ������Ĵ���
		ǰ�᣺ �յ����������ݰ���У�����ȷ��

*/
void com1_message_handle(void)
{
	UART_PROTOCOL_TYPE type = (UART_PROTOCOL_TYPE)com1_data[2];
//	uint8_t i;
	uint8_t ret;
	uint8_t *data = com1_data+3;   //ָ����Ч���ݣ�4���ֽڡ�
	//��������������������
	
	switch(type)
	{
		case BTN_QUERY:   //��ѯ������״̬��������ȷ��񣬴��ڶ��᷵�ء�������������
			ret = btns_query(data[0] - BTN_CODE_START);  //������ð�����λ��
		//	lf_uart_protocol_send(BTN_REPLY, data[0], ret);		//����ֵ������255������ͬ���������ݣ�����������ʾ������
			break;
		case LED_CTL:  //led����			
			if(data[1] == 1)
				ret = set_led_on(data[0] - LED_CODE_START);
			else
				ret = set_led_off(data[0] - LED_CODE_START);
			//������ȷ��񣬵�Ƭ������������Ӧ��
		//	lf_uart_protocol_send(LED_CTL, data[0], ret);	
			break;

		case LED_QUERY:   //led״̬��ѯ
			ret = get_led_value(data[0] - LED_CODE_START);
			if(ret != 255)
				ret = (ret==RESET) ? 1 : 0;//�͵�ƽ��ʾ�����ߵ�ƽ��
		//	lf_uart_protocol_send(LED_REPLY, data[0], ret);			
			break;
						
		case PWM_CTL:   //pwm����
//			if(data[0] == CODE_PWM)
//			{
//				pwm = data[1];
//				if(pwm > 100) pwm=100;
//				
//				gd32_pwm_out(pwm);
//			}
			break;
		case PWM_QUERY:  //pwm��ѯ
//			lf_uart_protocol_send(PWM_REPLY, CODE_PWM, pwm);
			break;
		case DVI_CTL:    //��Ƶ�л�����
			if(data[0] == CODE_DVI)
			{
				if(DVI_LOONGSON == data[1])
					dvi_switch_set(DVI_LOONGSON);
				else
					dvi_switch_set(DVI_OTHER);
			}
			break;
		case DVI_QUERY:  //��ʾ��Ƶ��ѯ
			ret = dvi_switch_get();
		//	lf_uart_protocol_send(DVI_REPLY, CODE_DVI, ret);
			break;	
		case FAN_CTL:   //���ȿ���
//			if(data[0] == CODE_FAN)
//			{
//				switch(data[1])
//				{
//					case FAN_ON_1:
//						fan_enable(0);
//						break;
//					case FAN_ON_2:
//						fan_enable(1);
//						break;
//					case FAN_ON_ALL:
//						fan_enable(0);
//						fan_enable(1);
//						break;
//					case FAN_OFF_1:
//						fan_disable(0);
//						break;
//					case FAN_OFF_2:
//						fan_disable(1);
//						break;
//					case FAN_OFF_ALL:
//						fan_disable(0);
//						fan_disable(1);
//						break;
//					default:
//						break;
//				}
//			}
			break;
		case FAN_QUERY:  //����״̬��ѯ
//			if(fan_get_value(0) == ENABLE)
//			{
//				if(fan_get_value(1) == ENABLE)
//				{
//					fan = FAN_ON_ALL;
//				}
//				else
//					fan = FAN_ON_1;
//			}
//			else
//			{
//				if(fan_get_value(1) == ENABLE)
//				{
//					fan = FAN_ON_2;
//				}
//				else
//					fan = FAN_OFF_ALL;
//			}
//				
//			lf_uart_protocol_send(FAN_REPLY, CODE_FAN, fan);
			break;	
		case HOT_CTL:   //���ȿ���
			if(data[0] == CODE_HOT)
			{
				set_heat_status(data[1]);
			}			
			break;
		case HOT_QUERY:  //����״̬����
			ret = get_heat_status();
		//	lf_uart_protocol_send(HOT_REPLY, CODE_HOT, ret);
			break;		
		case TMP_CTL:   //�¶ȿ���
//			if(data[0] == CODE_TMP)
//			{
//				if(1 == data[1])
//					temperat_send = 1;
//				else
//					temperat_send = 0;
//			}
			break;
		case TMP_QUERY:  //�¶Ȳ�ѯ
		//	lf_uart_protocol_send(TMP_REPLY, CODE_TMP, temperat_send);
			break;				
		default:
			break;
	}
}


#endif


#endif
