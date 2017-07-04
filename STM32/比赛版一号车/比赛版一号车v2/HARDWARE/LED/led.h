#ifndef _LED_H
#define _LED_H
#include "sys.h"
	

#define LED0 PEout(7)   //LED0
#define LED1 PFout(14)   //LED1

void LED_Init(void);
#endif
