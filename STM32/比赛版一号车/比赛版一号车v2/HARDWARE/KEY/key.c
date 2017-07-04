#include "key.h"
#include "delay.h"


//������ʼ������
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOD_CLK_ENABLE();           //����GPIOAʱ��
  
    GPIO_Initure.Pin=GPIO_PIN_10;           //PC13
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);

}

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��WKUP���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>WK_UP!!
u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;     //�����ɿ���־
    if(mode==1)key_up=1;    //֧������
    if(key_up&&(KEY0==0))
    {
        delay_ms(10);
        key_up=0;
        if(KEY0==0)       return KEY0_PRES;			
    }else if(KEY0==1)key_up=1;
    return 0;   //�ް�������
}
