#include "Algorithm.h"
#include "usart.h"
#include "math.h"

float ang;
s16 pos;
s16 pos_dif;


float Kp_201=9.5;          float Ki_201=0.025;          float Kd_201=1;
float Kp_202=9.5;          float Ki_202=0.025;          float Kd_202=1;
float Kp_203=9.5;          float Ki_203=0.025;          float Kd_203=1;
float Kp_204=9.5;          float Ki_204=0.025;          float Kd_204=1;
float Kp_p=3.1  ;          float Ki_p=0  ;              float Kd_p=320;
float Kp_y=3.1  ;          float Ki_y=0  ;              float Kd_y=370;

float Yaw_E_k=0;
float Yaw_E_sum=0;
float Yaw_E_k_1=0;


 float Pout_Pitch;
 float Iout_Pitch;
 float Dout_Pitch;
 float OUT_Pitch;
 
 float Pout_Yaw;
 float Iout_Yaw;
 float Dout_Yaw;
 float OUT_Yaw;
 
 float Pout;
 float Iout;
 float Dout;
 float OUT;

float romote_ang_cal(float x,float y)
{
 u8 xiangxian=0;
 double z;
 if((x<-5)&&(y>0))       {x=-x; xiangxian=4;}
 else if((x>5)&&(y<=0))  {y=-y; xiangxian=2;}
 else if((x<-5)&&(y<=0)) { x=-x; y=-y; xiangxian=3;}
 else if((x>-5)&&(x<5)&&(y<0)) { y=-y; xiangxian=10; }
 //else if((x=0)&&(y>0)) xiangxian=11;
 else xiangxian=1;
 z = y/x;
 if(xiangxian==4) ang = 3.14-atan(z);
 else if(xiangxian==2) ang = 6.28-atan(z);
 else if(xiangxian==3) ang = 3.14+atan(z);
 else if(xiangxian==10) ang = 4.71;
 //else if(xiangxian==11) ang = 90;
 else ang = atan(z);
 return ang;
}



//
//输入 0-8191
//turn  1 顺时针转为正方向   0 逆时针转为负方向
 
s16 Position_Cal(s16 pos_can,u8 turn,u16 pos_standard)  
{
 pos_dif=pos_can-pos_standard;
 if(pos_dif>4096) pos=pos_can-pos_standard-8191;
 else if((pos_dif<=4096)&&(pos_dif>=-4096)) pos=pos_can-pos_standard;   
 else pos=pos_can-pos_standard+8191;
 if(turn) pos=-pos;
 return pos;
}


 
float PID_Pitch(float sv_p,float pv_p)  //pid计算
{
 //float Pout_Pitch;
 //float Iout_Pitch;
 //float Dout_Pitch;
 //float OUT_Pitch;
 float static Pitch_E_k=0;
 float static Pitch_E_sum=0;
 float static Pitch_E_k_1=0;
 Pitch_E_k=sv_p-pv_p;
 if((Pitch_E_k<500)&&(Pitch_E_k>-500)) Pitch_E_sum+=Pitch_E_k;
 Pout_Pitch=Kp_p*Pitch_E_k;
 //if(Pout_Pitch> 1000) Pout_Pitch= 1000;
 //if(Pout_Pitch<-1000) Pout_Pitch=-1000;
 Iout_Pitch=Ki_p*Pitch_E_sum;
 //if(Iout_Pitch> 3000) Iout_Pitch= 3000;
 //if(Iout_Pitch<-3000) Iout_Pitch=-3000;
 Dout_Pitch=Kd_p*(Pitch_E_k-Pitch_E_k_1);
 OUT_Pitch=Pout_Pitch+Iout_Pitch+Dout_Pitch;
	
 Pitch_E_k_1=Pitch_E_k;
 if(OUT_Pitch> 2500) OUT_Pitch= 2500;
 if(OUT_Pitch<-2500) OUT_Pitch=-2500;
 return OUT_Pitch;
}

float PID_Yaw(float sv_y,float pv_y)  //pid计算
{
 //float Pout_Yaw;
 //float Iout_Yaw;
 //float Dout_Yaw;
 //float OUT_Yaw;
 //float static Yaw_E_k=0;
 //float static Yaw_E_sum=0;
 //float static Yaw_E_k_1=0;
 Yaw_E_k=sv_y-pv_y;
 //if((Yaw_E_k<250)&&(Yaw_E_k>-250)) Yaw_E_sum+=Yaw_E_k;   //积分的问题，过32000溢出，数据类型问题，云台先不开积分
 //if(Yaw_E_sum>32000)  Yaw_E_sum=32000;
 //if(Yaw_E_sum<-32000) Yaw_E_sum=-32000;
 Pout_Yaw=Kp_y*Yaw_E_k;
 Iout_Yaw=Ki_y*Yaw_E_sum;
 Dout_Yaw=Kd_y*(Yaw_E_k-Yaw_E_k_1);
 OUT_Yaw=Pout_Yaw+Iout_Yaw+Dout_Yaw;
	
 Yaw_E_k_1=Yaw_E_k;
 if(OUT_Yaw> 2500) OUT_Yaw= 2500;
 if(OUT_Yaw<-2500) OUT_Yaw=-2500;
 return OUT_Yaw;
}

float PID_Bottom_moto1(float sv1,float pv1)  //pid计算
{
 //float Pout;
 //float Iout;
 //float Dout;
 //float OUT;
 float static m1_E_k=0;
 float static m1_E_sum=0;
 float static m1_E_k_1=0;
 m1_E_k=sv1-pv1;
 if((m1_E_k<900)&&(m1_E_k>-900)&&(!(pv1==-3))) m1_E_sum+=m1_E_k;
 Pout=Kp_201*m1_E_k;
 Iout=Ki_201*m1_E_sum;
 Dout=Kd_201*(m1_E_k-m1_E_k_1);
 OUT=Pout+Iout+Dout;
 m1_E_k_1=m1_E_k;
 if(OUT> 5000) OUT= 5000;
 if(OUT<-5000) OUT=-5000;
 return OUT;
}

float PID_Bottom_moto2(float sv2,float pv2)  //pid计算
{
 float Pout2;
 float Iout2;
 float Dout2;
 float OUT2;
 float static m2_E_k=0;
 float static m2_E_sum=0;
 float static m2_E_k_1=0;
 m2_E_k=sv2-pv2;
 if((m2_E_k<900)&&(m2_E_k>-900)&&(!(pv2==-3))) m2_E_sum+=m2_E_k;
 Pout2=Kp_202*m2_E_k;
 Iout2=Ki_202*m2_E_sum;
 Dout2=Kd_202*(m2_E_k-m2_E_k_1);
 OUT2=Pout2+Iout2+Dout2;
 m2_E_k_1=m2_E_k;
 if(OUT2> 5000) OUT2= 5000;
 if(OUT2<-5000) OUT2=-5000;
 return OUT2;
}

float PID_Bottom_moto3(float sv3,float pv3)  //pid计算
{
 float Pout3;
 float Iout3;
 float Dout3;
 float OUT3;
 float static m3_E_k=0;
 float static m3_E_sum=0;
 float static m3_E_k_1=0;
 m3_E_k=sv3-pv3;
 if((m3_E_k<900)&&(m3_E_k>-900)&&(!(pv3==-3))) m3_E_sum+=m3_E_k;
 Pout3=Kp_203*m3_E_k;
 Iout3=Ki_203*m3_E_sum;
 Dout3=Kd_203*(m3_E_k-m3_E_k_1);
 OUT3=Pout3+Iout3+Dout3;
 m3_E_k_1=m3_E_k;
 if(OUT3> 5000) OUT3= 5000;
 if(OUT3<-5000) OUT3=-5000;
 return OUT3;
}

float PID_Bottom_moto4(float sv4,float pv4)  //pid计算
{
 float Pout4;
 float Iout4;
 float Dout4;
 float OUT4;
 float static m4_E_k=0;
 float static m4_E_sum=0;
 float static m4_E_k_1=0;
 m4_E_k=sv4-pv4;
 if((m4_E_k<900)&&(m4_E_k>-900)&&(!(pv4==-3))) m4_E_sum+=m4_E_k;
 Pout4=Kp_204*m4_E_k;
 Iout4=Ki_204*m4_E_sum;
 Dout4=Kd_204*(m4_E_k-m4_E_k_1);
 OUT4=Pout4+Iout4+Dout4;
 m4_E_k_1=m4_E_k;
 if(OUT4> 6000) OUT4= 6000;
 if(OUT4<-6000) OUT4=-6000;
 return OUT4;
}

float remote_angle_cal(float remote_get)  //remote_get  -600到600
{
 static float remote_get_r;
 static float remote_out;   //最大值
 u8 remote_en;
 static u8 remote_en_r;
 if(remote_en_r==1) remote_out=0;
 if(remote_get_r>15)
  {
	 if(remote_get>remote_out)  remote_out=remote_get;
	 else if(remote_get<15) 
	  //{remote_out=0;
		 remote_en=1;
	  //}
	}
 if(remote_get_r<-15)
  {
	 if(remote_get<remote_out)  remote_out=remote_get;
	 else if(remote_get>-15) 
	 //{remote_out=0;
		 remote_en=1;
		//}
	}
 remote_get_r=remote_get;
 remote_en_r=remote_en;
 if(remote_en==1) return remote_out;
 else return 0;
}
