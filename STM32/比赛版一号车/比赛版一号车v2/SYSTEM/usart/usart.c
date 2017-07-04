#include "sys.h"
#include "usart.h"
#include "led.h"
#include "delay.h"
#include "timer.h"
#include "Algorithm.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif


//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)  
//解决HAL库使用时,某些情况可能报错的bug
int _ttywrch(int ch)    
{
    ch=ch;
	return ch;
}
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

#if EN_USART1_RX   //如果使能了接收   	
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
//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率 
void uart1_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV@OVER8=0
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分@OVER8=0 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->AHB1ENR|=1<<1;   	//使能PORTA口时钟  
	RCC->APB2ENR|=1<<4;  	//使能串口1时钟 
	GPIO_Set(GPIOB,PIN6|PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);//PA9,PA10,复用功能,上拉输出
 	GPIO_AF_Set(GPIOB,6,7);	//PA9,AF7
	GPIO_AF_Set(GPIOB,7,7);//PA10,AF7  	   
	//波特率设置
 	USART1->BRR=mantissa; 	//波特率设置	 
	USART1->CR1&=~(1<<15); 	//设置OVER8=0
  USART1->CR1&=~(1<<9);	
	USART1->CR1|=1<<3;  	//串口发送使能 
#if EN_USART1_RX		  	//如果使能了接收
	//使能接收中断 
	USART1->CR1|=1<<2;  	//串口接收使能
	USART1->CR1|=1<<5;    	//接收缓冲区非空中断使能	    	
	MY_NVIC_Init(0,0,USART1_IRQn,2);//组2，最低优先级 
#endif
	USART1->CR1|=1<<13;  	//串口使能
}
void uart2_init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
  mantissa<<=4;
	mantissa+=fraction; 

	RCC->AHB1ENR|=1<<3;   		//使能PORTA口时钟   
	GPIO_Set(GPIOD,PIN5|PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);	
 	GPIO_AF_Set(GPIOD,5,7);		
	GPIO_AF_Set(GPIOD,6,7);		
 
	RCC->APB1ENR|=1<<17;  		//使能串口2时钟  
	RCC->APB1RSTR|=1<<17;   	//复位串口2
	RCC->APB1RSTR&=~(1<<17);	//停止复位	   	   
	//波特率设置
 	USART2->BRR=mantissa; 		// 波特率设置	 
	USART2->CR1|=0X200C;  		//1位停止,无校验位.
	//USART2->CR1|=1<<10;     //开校验
  USART2->CR1&=~(1<<9);   //偶校验	
#if EN_USART2_RX		  		//如果使能了接收
	//使能接收中断 
	USART2->CR1|=1<<2;  		//串口接收使能
	USART2->CR1|=1<<5;    		//接收缓冲区非空中断使能	    	
	MY_NVIC_Init(0,3,USART2_IRQn,2);//组2，最低优先级 
#endif
	
}

void USART1_IRQHandler(void)
{
	//static u8 romote_i;
  //static u8 num;
	if(USART1->SR&(1<<5))//接收到数据
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
	if(USART2->SR&(1<<5))//接收到数据
	{	 
		res2=USART2->DR; 
		if((USART_RX_STA2&0x8000)==0)//接收未完成
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
				 if(USART_RX_STA2>(USART_REC_LEN-1))USART_RX_STA2=0;//接收数据错误,重新开始接收	  
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
  {ussign=1;     //超时标志位
	 USART_RX_STA=0;    //超时后干的事
	}

}


