#ifndef _KEY_H
#define _KEY_H
#include "sys.h"





//����ķ�ʽ��ͨ��ֱ�Ӳ���HAL�⺯����ʽ��ȡIO
#define KEY0        HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_10)  //KEY0����PH3

#define KEY0_PRES 	1

void KEY_Init(void);
u8 KEY_Scan(u8 mode);
#endif
