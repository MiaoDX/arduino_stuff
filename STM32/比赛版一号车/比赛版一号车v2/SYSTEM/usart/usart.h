#ifndef __USART_H
#define __USART_H 
#include "sys.h"
#include "stdio.h"	  


#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[18]; 
extern u8 remote_i;         		
extern u8  USART_RX_BUF2[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA2;         		//����״̬���

extern float Kp_Usart;       //������PID�����ã�usart_keyΪ1��Ч       	
extern float Ki_Usart;         	
extern float Kd_Usart;         	
extern u8 usart_key;

extern u8 s1;
extern u8 s2;

void uart1_init(u32 pclk2,u32 bound);
void uart2_init(u32 pclk1,u32 bound);
void UsartMonitor(u8 ms);

#endif
