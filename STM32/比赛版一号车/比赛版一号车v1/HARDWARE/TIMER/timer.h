#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"


extern u8 Emit_en;
extern u8 pwm_gate;
extern u8 pwm_gate2;

extern float remote_x,remote_y,remote_rot,remote_p;

//extern u16 yun_pitch_pos;
extern float yun_pitch_sv;
extern float yun_pitch_pv;
extern float yun_yaw_sv;
extern float yun_yaw_pv;
extern s16 moto_201_pv;
extern s16 moto_202_pv;
extern s16 moto_203_pv;
extern s16 moto_204_pv;
extern float moto_201_sv;
extern float moto_202_sv;
extern float moto_203_sv;
extern float moto_204_sv;

//extern s16 pid_out;
extern s16 moto_pitch_out;
extern s16 moto_yaw_out;
extern s16 moto_201_out;
extern s16 moto_202_out;
extern s16 moto_203_out;
extern s16 moto_204_out;

extern TIM_HandleTypeDef TIM3_Handler;      //¶¨Ê±Æ÷¾ä±ú 
extern TIM_HandleTypeDef TIM4_Handler;
extern TIM_HandleTypeDef TIM5_Handler;

void TIM3_Init(u16 arr,u16 psc);
void TIM4_Init(u16 arr,u16 psc);
void TIM5_Init(u16 arr,u16 psc);

#endif

