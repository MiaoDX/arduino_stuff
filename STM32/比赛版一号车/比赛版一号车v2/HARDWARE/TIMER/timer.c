#include "timer.h"
#include "led.h"
#include "Algorithm.h"
#include "usart.h"
#include "can.h"
//#include "math.h"

u8 pwm_gate=49;
u8 pwm_gate2=75;

float remote_x,remote_y,remote_rot,remote_p;
float yun_pitch_sv,yun_pitch_pv;
float yun_yaw_sv,yun_yaw_pv;
float moto_201_sv ,moto_202_sv ,moto_203_sv ,moto_204_sv ;
s16   moto_201_pv ,moto_202_pv ,moto_203_pv ,moto_204_pv ;
s16   moto_201_out,moto_202_out,moto_203_out,moto_204_out,moto_pitch_out,moto_yaw_out;
//u16 yun_pitch_pos;
float pv;
float sv;

s16 w1=0;  //��14 ƽ�Ʒ�ת��
s16 w2=0;  //��23 ƽ�Ʒ�ת��
s16 wz=0;  //��ת��ת��
s16 w_pitch;  //��̨pitchת��

TIM_HandleTypeDef TIM3_Handler;      //��ʱ����� 
TIM_HandleTypeDef TIM4_Handler;
TIM_HandleTypeDef TIM5_Handler;


//����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪHCLK/2)
void TIM3_Init(u16 arr,u16 psc)
{   
	  GPIO_InitTypeDef GPIO_Initure;
	  __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();           //����GPIOBʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_6; //PB1,0
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLDOWN;          
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
    GPIO_Initure.Pin=GPIO_PIN_9;
	  HAL_GPIO_Init(GPIOH,&GPIO_Initure);

    GPIO_Initure.Pin=GPIO_PIN_0;	
	  HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	  
	  HAL_GPIO_WritePin(GPIOH,GPIO_PIN_6|GPIO_PIN_9,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
	
    TIM3_Handler.Instance=TIM3;                          //ͨ�ö�ʱ��3
    TIM3_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM3_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM3_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM3_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE   
}

void TIM4_Init(u16 arr,u16 psc)
{   	
    TIM4_Handler.Instance=TIM4;                          //ͨ�ö�ʱ��3
    TIM4_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM4_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM4_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM4_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE   
}

void TIM5_Init(u16 arr,u16 psc)
{   	
    TIM5_Handler.Instance=TIM5;                          //ͨ�ö�ʱ��3
    TIM5_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM5_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM5_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM5_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE   
}



//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            
		HAL_NVIC_SetPriority(TIM3_IRQn,1,2);   
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          
	}
	  if(htim->Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();           
		HAL_NVIC_SetPriority(TIM4_IRQn,2,3);   
		HAL_NVIC_EnableIRQ(TIM4_IRQn);          
	}
	  if(htim->Instance==TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();            //ʹ��TIM3ʱ��
		HAL_NVIC_SetPriority(TIM5_IRQn,3,0);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM5_IRQn);          //����ITM3�ж�   
	}
}



//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}
void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM4_Handler);
}
void TIM5_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM5_Handler);
}


//�ص���������ʱ���жϷ���������
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{ 
    if(htim==(&TIM3_Handler))
    {   
			static u16 num;
      if(num<pwm_gate) {PHout(6)=1;PHout(9)=1;}
      else		{PHout(6)=0;PHout(9)=0;}
			
			if(num<pwm_gate2) PAout(0)=1; else PAout(0)=0;

      if(num==999) {num=0;LED0=!LED0;}
      else num++;			
    }
		if(htim==(&TIM4_Handler))
    { 
      u16  static begin_cnt;	  //�и����壬��ʱ����ʼ��200���ڵ�CAN1��ʼ������ٷ���CAN1����		
			u8 res_can;
      u8 canbuf_1FF[8],canbuf_200[8];
      //float ang,spd;
			
			static float moto_yaw_out_r;
			static float moto_yaw_out_rr;
			
			static float moto_pitch_out_r;
			static float moto_pitch_out_rr;
			
			if(begin_cnt<2800)  begin_cnt++;     //��һ�����壬�ȶ���ʼ�������ٷ�
			else {
			//ang=romote_ang_cal(remote_x,remote_y);
			//spd=sqrt(remote_x*remote_x+remote_y*remote_y);
		  //w1=8*spd*(cos(ang)+sin(ang));      //ң��ָ����� 
      //w2=8*spd*(-cos(ang)+sin(ang));
			w1=8*(remote_x+remote_y);
			w2=8*(-remote_x+remote_y);
			
			
			
			wz=7*remote_rot;
			//yun_pitch_sv+=remote_p;
			//if(yun_pitch_sv>600) yun_pitch_sv=600;else if(yun_pitch_sv<-600) yun_pitch_sv=-600;
			moto_201_sv=w1+wz;  moto_202_sv=w2-wz;  //ƽ�Ʒ���1 4�� 2 3����һ�� �� 1 3��һ�� 2 4����һ�� �ٶ��෴
			moto_203_sv=w2+wz;  moto_204_sv=w1-wz;
   		if(usart_key==1)
			{
			 moto_yaw_out_r= PID_Yaw(0,yun_yaw_pv);
			 moto_yaw_out_rr=0.1*moto_yaw_out_r+0.9*moto_yaw_out_rr;
			 moto_yaw_out=(s16)moto_yaw_out_rr;
			 
			 moto_pitch_out_r= PID_Pitch(yun_pitch_sv,yun_pitch_pv);
			 moto_pitch_out_rr=0.1*moto_pitch_out_r+0.9*moto_pitch_out_rr;
			 moto_pitch_out=-(s16)moto_pitch_out_rr;
			 
			 moto_201_out=PID_Bottom_moto1((float)moto_201_sv,moto_201_pv);
			 moto_202_out=-PID_Bottom_moto2((float)moto_202_sv,moto_202_pv);
			 moto_203_out=PID_Bottom_moto3((float)moto_203_sv,moto_203_pv);
			 moto_204_out=-PID_Bottom_moto4((float)moto_204_sv,moto_204_pv);
			 //moto_201_out=6000;
			 //moto_202_out=6000;
			 //moto_203_out=6000;
			 //moto_204_out=6000;
			}
			else {w_pitch=0;moto_201_out=0;moto_202_out=0;moto_203_out=0;moto_204_out=0;}
		  if(s2==2) 
			{
			canbuf_1FF[0]=moto_yaw_out  >>8; canbuf_1FF[1]=moto_yaw_out  ; 
			canbuf_1FF[2]=moto_pitch_out>>8; canbuf_1FF[3]=moto_pitch_out;
		  canbuf_1FF[4]=0x00; canbuf_1FF[5]=0x00; canbuf_1FF[6]=0x00;  canbuf_1FF[7]=0x00;
			canbuf_200[0]=moto_201_out>>8; canbuf_200[1]=moto_201_out;       //ID1
			canbuf_200[2]=moto_202_out>>8; canbuf_200[3]=moto_202_out;       //ID2  
		  canbuf_200[4]=moto_203_out>>8; canbuf_200[5]=moto_203_out;       //ID3
			canbuf_200[6]=moto_204_out>>8; canbuf_200[7]=moto_204_out;       //ID4
			}
			else
			{canbuf_1FF[0]=0x00; canbuf_1FF[1]=0x00; canbuf_1FF[2]=0x00; canbuf_1FF[3]=0x00;
		   canbuf_1FF[4]=0x00; canbuf_1FF[5]=0x00; canbuf_1FF[6]=0x00;  canbuf_1FF[7]=0x00;
			 canbuf_200[0]=0x00;canbuf_200[1]=0x00;  canbuf_200[3]=0x00;canbuf_200[4]=0x00;
			 canbuf_200[4]=0x00;canbuf_200[5]=0x00;  canbuf_200[6]=0x00;canbuf_200[7]=0x00;}
      
			res_can=CAN1_Send_Msg(canbuf_200,8,0x200);
			res_can=CAN1_Send_Msg(canbuf_1FF,8,0x1FF);
		  }
    }
		
		
		
		if(htim==(&TIM5_Handler))
    {   
		 static u8 romote_test_i;
     static u8 remote_i_r;
     if(remote_i!=0) 
		 {
			if(remote_i!=remote_i_r)
			{romote_test_i=0;}
			else
			  {if(romote_test_i>20) {remote_i=0;remote_i_r=0;romote_test_i=0;}
				 romote_test_i+=1;
			  }
	    remote_i_r=remote_i;
		 }
    }
}
