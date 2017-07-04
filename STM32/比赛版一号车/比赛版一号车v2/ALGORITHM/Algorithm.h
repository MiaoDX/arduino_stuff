#ifndef __Algorithm_H
#define __Algorithm_H
#include "sys.h"

#define Pos_Standard 3130  //宿舍云台电机值
#define Pos_Stan_Pitch 7777
#define Pos_Stan_Yaw   1510


extern float Pout;
extern float Iout;
extern float Dout;
extern float OUT;

extern float Pout_Pitch;
extern float Iout_Pitch;
extern float Dout_Pitch;
extern float OUT_Pitch;

extern float Pout_Yaw;
extern float Iout_Yaw;
extern float Dout_Yaw;
extern float OUT_Yaw;

extern float Yaw_E_k;
extern float Yaw_E_sum;
extern float Yaw_E_k_1;


float romote_ang_cal(float x,float y);
s16 Position_Cal(s16 pos_can,u8 turn,u16 pos_standard);
float PID_Pitch(float sv_p,float pv_p);
float PID_Yaw(float sv_y,float pv_y);
float PID_Bottom_moto1(float sv1,float pv1);
float PID_Bottom_moto2(float sv2,float pv2);
float PID_Bottom_moto3(float sv3,float pv3);
float PID_Bottom_moto4(float sv4,float pv4);

float remote_angle_cal(float remote_get);


#endif
