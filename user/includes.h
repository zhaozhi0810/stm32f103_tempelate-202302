
#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include <stm32f10x.h>

#include <stdint.h>
#include <string.h>
#include <stdio.h>


//���Գ����������Ҫ�����ʱ��ע�͵���
#define DEBUG
#ifdef  DEBUG
#define MY_DEBUG(fmt, ...)	printf("%s, %d, %s "fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
#define MY_DEBUG(...)      
#endif





#include "systick_delay.h"
#include "task_cfg.h"
#include "queues_in_stm32uart.h"
#include "usart1.h"
#include "led_show_worked.h"

//#include "btns_leds.h"
////#include "hard_iic.h"
//#include "iic_app.h"
//#include "Nca9555.h"
//#include "task_cfg.h"
//#include "32_gpios.h"
//#include "usart3.h"
//#include "vol_temp.h"
//#include "lcd_pwm.h"
//#include "vol_adc.h"
////#include "fan_pwm.h"   //2021-12-09 ���Ȳ�ʹ��pwm����
////#include "18B20.h"   //�ɼ�lcd�¶�
//#include "hce75ms.h"  //�ɼ������cpu�¶�
//#include <stdio.h>
//#include "DS18B20.h"
////#include "siI1161.h"

//#define LCD_PWM_HEAT  //lcdʹ��pwm����
//#ifdef LCD_PWM_HEAT
//#include "lcdheat_pwm.h"
//#endif


typedef enum {
	LS3A_POWEROFF = 1,
	LS3A_REBOOT  =  2,
	LS3A_RUNNING  = 3,
	LS3A_RUN_OS = 4,
	LS3A_PRERUN  =  5
}system_run_status;


#endif



