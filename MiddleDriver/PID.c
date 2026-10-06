#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"

#include "PID.h"
#include "motor.h"
#include "Encoder.h"
#include "Menu.h"



volatile int Kp = 170,Ki = 25,Kd = 30;
int error,last_error;
int change_p,change_i,change_d;

int abs(int num)
{
	if(num >= 0)
	{
		return num;
	}
	else
	{
		return -num;
	}
}

int PID_Cal(int tar,int cur)
{
	last_error = error;
	error = tar - cur;
	
	change_p = error;
	change_d = error - last_error;
	change_i += error;
	
	//积分限幅
	if(change_i > MAX_CHANGE_I)
	{
		change_i = MAX_CHANGE_I;
	}
	else if(change_i < -MAX_CHANGE_I)
	{
		change_i = -MAX_CHANGE_I;
	}
	
	int output = Kp * change_p + Ki * change_i + Kd * change_d;
	return output;
}

void PID_Control(int tar_speed,int cur_speed)
{
	int PWM = PID_Cal(tar_speed,cur_speed);
	Motor_Load(PWM);
}
