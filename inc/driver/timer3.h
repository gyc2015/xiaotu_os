#pragma once

#include <stm32f407_timer.h>

/*
* tim3_init - ��ʼ��TIM3, ������������APB1��,����Ƶ�����Ϊ42MHz
*
* @pres: Ԥ��Ƶϵ��
* @period: ��������
*/
void tim3_init(uint16 pres, uint16 period);

