#ifndef __USART_H
#define __USART_H 
#include "sys.h"
#include "stdio.h"	  


#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[18]; 
extern u8 remote_i;         		
extern u8  USART_RX_BUF2[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA2;         		//接收状态标记

extern float Kp_Usart;       //串口设PID参数用，usart_key为1有效       	
extern float Ki_Usart;         	
extern float Kd_Usart;         	
extern u8 usart_key;

extern u8 s1;
extern u8 s2;

void uart1_init(u32 pclk2,u32 bound);
void uart2_init(u32 pclk1,u32 bound);
void UsartMonitor(u8 ms);

#endif
