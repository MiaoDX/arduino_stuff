#include "led.h"
	

//��ʼ��PB1Ϊ���.��ʹ��ʱ��	    
//LED IO��ʼ��
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOE_CLK_ENABLE();           //����GPIOBʱ��
	  __HAL_RCC_GPIOF_CLK_ENABLE();
	  __HAL_RCC_GPIOG_CLK_ENABLE();
	
    GPIO_Initure.Pin=GPIO_PIN_7; //PB1,0
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
	
	  GPIO_Initure.Pin=GPIO_PIN_14;
	  HAL_GPIO_Init(GPIOF,&GPIO_Initure);
	
	  GPIO_Initure.Pin=GPIO_PIN_13;
	  HAL_GPIO_Init(GPIOG,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET);	//PB0��1 
    HAL_GPIO_WritePin(GPIOF,GPIO_PIN_14,GPIO_PIN_SET);	//PB1��1 
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_13,GPIO_PIN_SET);	//PB1��1 	
}
