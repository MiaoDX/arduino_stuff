There are some lesson learnt from the open-sourced codes of the 2016 RM summer camp.

# 2 组

It controlled the motor via three ways, the remote controller, the mouse and the keyboard (see User/main.h). And in `main.c`, there is some code for `/* 位置环PID控制 */`, maybe we can have a look at.

However, in their writeup:

> 首先 NUC 通过外置摄像头利用 ORB_SLAM 进行全场定位，借助 PID
控制器将速度控制量通过串口发给底盘主控，底盘主控通过速度环直接控制电机
转速。通过这样的闭环控制，保证了小车的定位精度，避免了底盘主控位置环的
控制误差较大且容易受轮子打滑影响的问题。

It shocks me a lot... Will have a try on it latter.