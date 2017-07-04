#include "sys.h"
#include "usart.h"
#include "led.h"
#include "delay.h"
#include "timer.h"
#include "Algorithm.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)  
//���HAL��ʹ��ʱ,ĳЩ������ܱ����bug
int _ttywrch(int ch)    
{
    ch=ch;
	return ch;
}
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

#if EN_USART1_RX   //���ʹ���˽���   	
u8 USART_RX_BUF[18];     

u16 USART_RX_STA=0;      


u8 USART_RX_BUF2[USART_REC_LEN]; 
u16 USART_RX_STA2=0;         		
float Kp_Usart;
float Ki_Usart;
float Kd_Usart;

u16 ch0;
u16 ch1;
u16 ch2_left_x;
u16 ch3_left_y;
u8 s1;
u8 s2;

u8 usart_key=1;

u8 remote_i;
 
#endif										 
//��ʼ��IO ����1
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������ 
void uart1_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV@OVER8=0
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������@OVER8=0 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->AHB1ENR|=1<<1;   	//ʹ��PORTA��ʱ��  
	RCC->APB2ENR|=1<<4;  	//ʹ�ܴ���1ʱ�� 
	GPIO_Set(GPIOB,PIN6|PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);//PA9,PA10,���ù���,�������
 	GPIO_AF_Set(GPIOB,6,7);	//PA9,AF7
	GPIO_AF_Set(GPIOB,7,7);//PA10,AF7  	   
	//����������
 	USART1->BRR=mantissa; 	//����������	 
	USART1->CR1&=~(1<<15); 	//����OVER8=0
  USART1->CR1&=~(1<<9);	
	USART1->CR1|=1<<3;  	//���ڷ���ʹ�� 
#if EN_USART1_RX		  	//���ʹ���˽���
	//ʹ�ܽ����ж� 
	USART1->CR1|=1<<2;  	//���ڽ���ʹ��
	USART1->CR1|=1<<5;    	//���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(0,0,USART1_IRQn,2);//��2��������ȼ� 
#endif
	USART1->CR1|=1<<13;  	//����ʹ��
}
void uart2_init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
  mantissa<<=4;
	mantissa+=fraction; 

	RCC->AHB1ENR|=1<<3;   		//ʹ��PORTA��ʱ��   
	GPIO_Set(GPIOD,PIN5|PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);	
 	GPIO_AF_Set(GPIOD,5,7);		
	GPIO_AF_Set(GPIOD,6,7);		
 
	RCC->APB1ENR|=1<<17;  		//ʹ�ܴ���2ʱ��  
	RCC->APB1RSTR|=1<<17;   	//��λ����2
	RCC->APB1RSTR&=~(1<<17);	//ֹͣ��λ	   	   
	//����������
 	USART2->BRR=mantissa; 		// ����������	 
	USART2->CR1|=0X200C;  		//1λֹͣ,��У��λ.
	//USART2->CR1|=1<<10;     //��У��
  USART2->CR1&=~(1<<9);   //żУ��	
#if EN_USART2_RX		  		//���ʹ���˽���
	//ʹ�ܽ����ж� 
	USART2->CR1|=1<<2;  		//���ڽ���ʹ��
	USART2->CR1|=1<<5;    		//���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(0,3,USART2_IRQn,2);//��2��������ȼ� 
#endif
	
}

void USART1_IRQHandler(void)
{
	//static u8 romote_i;
  //static u8 num;
	if(USART1->SR&(1<<5))//���յ�����
	{	 
		USART_RX_BUF[remote_i]=USART1->DR; 
		remote_i+=1;
		if(remote_i==17)
		{
			if((USART_RX_BUF[6]==0)&&(USART_RX_BUF[7]==0)&&(USART_RX_BUF[8]==0)&&(USART_RX_BUF[9]==0)&&(USART_RX_BUF[10]==0)&&(USART_RX_BUF[11]==0))
       {ch0=((USART_RX_BUF[0])|(USART_RX_BUF[1]<<8))&0x07ff;
			  ch1=((USART_RX_BUF[1]>>3)|(USART_RX_BUF[2]<<5))&0x07ff;
        ch2_left_x=((USART_RX_BUF[2]>>6)|(USART_RX_BUF[3]<<2)|(USART_RX_BUF[4]<<10))&0x07ff;
        ch3_left_y=((USART_RX_BUF[4]>>1)|(USART_RX_BUF[5]<<7))&0x07ff;
        s1=	((USART_RX_BUF[5]>>4)&0x0C)>>2;
        s2=	(USART_RX_BUF[5]>>4)&0x03;
			  if((ch0>300)&&(ch0<1700)) remote_x = ch0 -1024;
		    if((ch1>300)&&(ch1<1700)) remote_y = ch1 -1024;
			  if((ch2_left_x>300)&&(ch2_left_x<1700)) remote_rot=ch2_left_x-1024;
			  if((ch3_left_y>300)&&(ch3_left_y<1700)) 
         {remote_p=ch3_left_y-1024;
				  remote_p=remote_angle_cal(remote_p);
				  yun_pitch_sv+=remote_p;
			    if(yun_pitch_sv>600) yun_pitch_sv=600;else if(yun_pitch_sv<-600) yun_pitch_sv=-600;
				 }
			  //if(s1==1) {pwm_gate=79;pwm_gate2=68;
				if(s1==1) {pwm_gate=60;pwm_gate2=68;
				}
			  //else if(s1==3) {pwm_gate=79;pwm_gate2=75;
				else if(s1==3) {pwm_gate=60;pwm_gate2=75;
				}
			  else {pwm_gate=49;pwm_gate2=0;
				//PGout(13)=0;
				}				
		  }			 
       remote_i=0;			 
			}
			     
	} 
}

void USART2_IRQHandler(void)
{
	u8 res2;
	if(USART2->SR&(1<<5))//���յ�����
	{	 
		res2=USART2->DR; 
		if((USART_RX_STA2&0x8000)==0)//����δ���
		{
			if(res2==0x55)
			  {USART_RX_STA2=0x8000;
				 Kp_Usart=(float)USART_RX_BUF2[0]/10;
			   Ki_Usart=(float)USART_RX_BUF2[1]/1000;
			   Kd_Usart=(float)USART_RX_BUF2[2]/10;
			   if(USART_RX_BUF2[3]==0xaa) usart_key=1;else usart_key=0;
	   	   USART_RX_STA2=0;
				}
			else
				{
				 USART_RX_BUF2[USART_RX_STA2&0X3FFF]=res2;
				 USART_RX_STA2++;
				 if(USART_RX_STA2>(USART_REC_LEN-1))USART_RX_STA2=0;//�������ݴ���,���¿�ʼ����	  
			 }		 		
		}
    LED1=!LED1;		
	} 
}

void UsartMonitor(u8 ms)           
{u8 cnt;
 static u8 ustimer;
 static u8 cntbkp;
 static u8 ussign;
	
 cnt=(USART_RX_STA&0X3FFF);
 if(cnt==0){cntbkp=0;return;}
 if(cnt!=cntbkp){ustimer=0;cntbkp=cnt;ussign=0;return;}
 if(ussign!=0) return;
 ustimer+=1;
 if(ustimer>=ms) 
  {ussign=1;     //��ʱ��־λ
	 USART_RX_STA=0;    //��ʱ��ɵ���
	}

}


