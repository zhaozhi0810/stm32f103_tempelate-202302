

#include "includes.h"

/*
	2021-09-16
	
	1. ��һ����ֲ���ļ���
*/



static __IO uint32_t g_localtime = 0; /* for creating a time reference incremented by 1ms */

static uint32_t ticks = 0;
static uint32_t tflag = 0;




/*!
    \brief      insert a delay time
    \param[in]  ncount: number of 10ms periods to wait for
    \param[out] none
    \retval     none
*/
void delay_1ms(uint32_t ncount)
{
    /* capture the current local time */
	uint32_t tEnd = g_localtime + ncount;
	
	/* wait until the desired delay finish */  
	while (tEnd != g_localtime);	
}


static u8  fac_us=72;	//systick����Ƶ��		72MHz û��8��Ƶ			//us��ʱ������
//59652323ns��59652ms��59s
void delay_us(uint32_t nus)
{
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD��ֵ
	ticks=nus*fac_us; 						//��Ҫ�Ľ�����
//	delay_osschedlock();					//��ֹOS���ȣ���ֹ���us��ʱ
	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;
		if(tnow!=told)
		{
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;
			told=tnow;
			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}
	}
}



/*!
    \brief      updates the system local time
    \param[in]  none
    \param[out] none
    \retval     none
*/
void systick_int_update(void)
{
    static uint8_t times = 0;
	g_localtime ++;
	
	//���������ִ��
	if(g_localtime % TASK1_TICKS_INTERVAL == 0)
	{
		task_id |= 1;  //����1����Ƶ�л����غ��ϵ翪��ɨ��
	}
			
	if(g_localtime % TASK16_TICKS_INTERVAL == 10)
	{
		task_id |= 1<<15;   //����16��work_led
	}
		
//	if(cpu_run_status > LS3A_POWEROFF)   //�ǹػ�ģʽɨ��
	{
//		if(cpu_run_status == LS3A_RUN_OS)
		{
			if(g_localtime % TASK2_TICKS_INTERVAL == 2)
			{
				task_id |= 2;   //����2��18������ɨ��
			}
					
			if(g_localtime % TASK5_TICKS_INTERVAL == 0)
			{
				task_id |= (1<<4);   //����5��500msɨ�裬��ʱ�㱨�¶ȵ�����
			}
		}		
		//��ȡ�¶ȣ�Ϊ֮�����Ļ������׼����2021-12-15
		if(g_localtime % TASK4_TICKS_INTERVAL == 0)
		{
			task_id |= 8;   //����4��200msɨ�裬��ʪ�ȣ���ѹ��ض�ȡ����
		}		
	}
	
	//����״̬ɨ����2022-01-04�޸�
	if(SET == GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13))   //�ϵ�� 2022-01-04���ӡ�v1.2 ����ǰ1.8���״̬
	{		
		if(times < 180)	//�����ϵ��1.5���״̬ 2022-01-04���ӡ�v1.5 �����ϵ��1.8���״̬
		{
			times++;
		}
		else if(g_localtime % TASK3_TICKS_INTERVAL == 4)
		{
			task_id |= 4;   //����3��10msɨ�裬���ػ�������
		}		
	}
	else if(times) //�����ϵ��1.5���״̬��2022-01-17  v1.5���ӡ�
	{
		times = 0;
	}
	
	
//	if(g_localtime % TASK16_TICKS_INTERVAL == 0)
//	{
//		task_id |= (1<<15);   //����16��1sɨ�裬����led��˸����
//	}
		
   
}



#if 0
/*!
    \brief      task ticks increase��
//���գ������ִ�е����񳬹�1ms���������tick��û�м�ʱ���ӡ���ִ�������ѹ��Ӱ�졣
    \param[in]  none
    \param[out] none
    \retval     none
*/
void tasktick_increase(void)
{
    static uint32_t delay2tick = 0;

    if(delay2tick != g_localtime)
    {
        delay2tick = g_localtime;
        tflag = 1;
        ticks++;
    }
    else
    {
        tflag = 0;
    }
}

/*!
    \brief      loop a time in milliseconds
    \param[in]  count: count in milliseconds
    \param[out] none
    \retval     none
*/
int8_t tasktick_1ms(uint32_t count)
{
    if(((ticks % count) == 0) && tflag)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#endif

/*!
    \brief      configure systick
    \param[in]  none
    \param[out] none
    \retval     0 �� �ɹ� �� ��0�� ʧ��
*/
int systick_config(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);    //ֱ����HCLKʱ�ӣ�
    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 1000U )){
        /* capture error */
        return 1;     //����ʧ��
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x02U);
	
	return 0;
}



