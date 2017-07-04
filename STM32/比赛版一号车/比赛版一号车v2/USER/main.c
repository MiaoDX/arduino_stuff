#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"

#include "can.h"
#include "math.h"
#include "timer.h"

#include "Algorithm.h"


int main(void)
{
   	u8 key;
	  u16 t=0;
	  u8 canbuf_r[8];
	  u8 res;

		int id[1];
	  
	  //s16 yun_pitch_pv_r;
	  
	  
		
    HAL_Init();                     
    Stm32_Clock_Init(360,12,2,8);   
    delay_init(180);                
    //uart1_init(90,100000);  //串口1 波特率100000           
		//uart2_init(45,100000);  //串口2 波特率100000        
    LED_Init();                     
    KEY_Init();
    //CAN1_Mode_Init(CAN_SJW_1TQ,CAN_BS2_6TQ,CAN_BS1_8TQ,3,CAN_MODE_NORMAL);  //波特率1M	
		TIM3_Init(2,597-1);      //定时器时钟为90M
		TIM4_Init(999,89);       //1ms
		TIM5_Init(9,89);        //10us
		
		uart1_init(90,100000);  //串口1 波特率100000           
		uart2_init(45,100000);  //串口2 波特率100000 
    CAN1_Mode_Init(CAN_SJW_1TQ,CAN_BS2_6TQ,CAN_BS1_8TQ,3,CAN_MODE_NORMAL);  //波特率1M			

	
 	
    while(1)
    {
	
		 
		 key=CAN1_Receive_Msg(canbuf_r,id);
		 if(key)
		 {if(id[0]==0x205) 
			 {yun_yaw_pv=(canbuf_r[0]<<8|canbuf_r[1]);  //0 ~ 8191
				yun_yaw_pv=Position_Cal(yun_yaw_pv,1,Pos_Stan_Yaw);//顺时针增加 逆时针减小
				 }
		  if(id[0]==0x206) 
			 {yun_pitch_pv=(canbuf_r[0]<<8|canbuf_r[1]);  //0 ~ 8191
				yun_pitch_pv=Position_Cal(yun_pitch_pv,0,Pos_Stan_Pitch);
			 }
			if(id[0]==0x201)  moto_201_pv=(canbuf_r[2]<<8|canbuf_r[3]);  //-7500~+7500
			if(id[0]==0x202)  moto_202_pv=-(canbuf_r[2]<<8|canbuf_r[3]);
			if(id[0]==0x203)  moto_203_pv=(canbuf_r[2]<<8|canbuf_r[3]);
			if(id[0]==0x204)  moto_204_pv=-(canbuf_r[2]<<8|canbuf_r[3]);     			 
		 }
		
    //if(s1==2) PGout(13)=0; else PGout(13)=1;
		 
		t++;

		delay_us(50);
		if(t==4000)
		{ 
			delay_us(20);
			//printf("%d   ",(s16)yun_pitch_sv);
			//printf("%d          ",(s16)yun_pitch_pv);
			//printf("%d   ",(s16)Pout_Pitch);
			//printf("%d   ",(s16)Iout_Pitch);
			//printf("%d   ",(s16)Dout_Pitch);
			//printf("%d    \r\n",(s16)remote_p);
			
			//printf("%d   ",moto_201_out);
			//printf("%d   ",moto_202_out);
			//printf("%d   ",(s16)Yaw_E_k);
			//printf("%d   ",(s16)Yaw_E_k_1);
			//printf("%d   \r\n",(s16)yun_pitch_sv);
			t=0;
			//LED1=!LED1;
		}
    //if(i==200) state=0;		
	} 		    
}
