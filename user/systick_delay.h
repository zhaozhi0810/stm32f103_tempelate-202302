

#ifndef __SYSTICK_DELAY_H__
#define __SYSTICK_DELAY_H__

#include <stdint.h>

//1ms��ʱ
int systick_config(void);

//����tick�����������������׼ȷ��
//���գ������ִ�е����񳬹�1ms���������tick��û�м�ʱ���ӡ�
void tasktick_increase(void);
//��ʱ����count��ʾ����msִ��һ��
int8_t tasktick_1ms(uint32_t count);

//��ʱ1ms
void delay_1ms(uint32_t ncount);

//systick�жϴ�����
void systick_int_update(void);


//us����ʱ
void delay_us(uint32_t nus);
#endif
