#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

#include "OLED.h"
#include "Menu.h"
#include "Encoder.h"
#include "SG09.h"
#include "PID.h"


volatile int Level;
volatile int isRound;


void First_Set_UI(void)
{
	OLED_Clear();
	OLED_ShowString(0,0,"Level:",OLED_8X16);
	OLED_ShowNum(55,0,0,1,OLED_8X16);
	OLED_ShowNum(71,0,1,1,OLED_8X16);
	OLED_ShowNum(87,0,2,1,OLED_8X16);
	OLED_ShowNum(103,0,3,1,OLED_8X16);
	
	OLED_ShowString(0,15,"Round:",OLED_8X16);
	OLED_ShowString(55,15,"ON",OLED_8X16);
	OLED_ShowString(79,15,"OFF",OLED_8X16);
	
	OLED_Printf(0,31,OLED_8X16,"P=%-3d",Kp);
	OLED_Printf(44,31,OLED_8X16,"I=%-3d",Ki);
	OLED_Printf(88,31,OLED_8X16,"D=%-3d",Kd);
	
	OLED_Printf(0,47,OLED_8X16,"Tar:%3d",act_speed);
	OLED_Printf(62,47,OLED_8X16,"Cur:%3d",cur_speed);
	
	switch(Level)
	{
		case 0:
			OLED_DrawLine(55,15,63,15);
			break;
		case 1:
			OLED_DrawLine(71,15,79,15);
			break;
		case 2:
			OLED_DrawLine(87,15,95,15);
			break;
		case 3:
			OLED_DrawLine(103,15,111,15);
			break;
	}
	
	switch(isRound)
	{
		case 0:
			OLED_DrawLine(79,30,103,30);
			break;
		case 1:
			OLED_DrawLine(55,30,71,30);
			break;
	}
}
void First_Set(void)
{
	int SetFlag = 1;
	while(1)
	{		
		Encoder_GetDelta();
		if(Encoder_State == 1)
		{
			if(SetFlag < 5)
			{
				SetFlag++;
			}
		}
		else if(Encoder_State == -1)
		{
			if(SetFlag > 1)
			{
				SetFlag--;
			}
		}
		//按键按下
		if(KeyState == 1)
		{
			KeyState = 0;
			if(SetFlag == 1)
			{
				Level_Set();
			}
			else if(SetFlag == 2)
			{
				Round_Set();
			}
			else if(SetFlag == 3 || SetFlag == 4 || SetFlag == 5)
			{
				PID_Set(SetFlag);
			}
		}
		//光标显示
		if(OLEDState == 1)
		{
			OLEDState = 0;
			switch(SetFlag)
			{
				case 1:
					First_Set_UI();
					OLED_ReverseArea(0,0,48,16);
					break;
				case 2:
					First_Set_UI();
					OLED_ReverseArea(0,15,48,16);
					break;
				case 3:
					First_Set_UI();
					OLED_ReverseArea(0,31,16,16);
					break;
				case 4:
					First_Set_UI();
					OLED_ReverseArea(44,31,16,16);
					break;
				case 5:
					First_Set_UI();
					OLED_ReverseArea(88,31,16,16);
					break;
			}
			OLED_Update();
		}
	}
}


int Level_Set(void)
{
	int Level_SetFlag = 1;
	while(1)
	{
		Encoder_GetDelta();
		if(Encoder_State == 1)
		{
			if(Level_SetFlag < 4)
			{
				Level_SetFlag++;
			}
		}
		else if(Encoder_State == -1)
		{
			if(Level_SetFlag > 1)
			{
				Level_SetFlag--;
			}
		}
		//按键按下
		if(KeyState == 1)
		{
			KeyState = 0;
			if(Level_SetFlag == 1)
			{
				//0档
				Level = 0;
				return 0;
			}
			else if(Level_SetFlag == 2)
			{
				//1档
				Level = 1;
				return 0;
			}
			else if(Level_SetFlag == 3)
			{
				//2档
				Level = 2;
				return 0;
			}
			else if(Level_SetFlag == 4)
			{
				//3档
				Level = 3;
				return 0;
			}
		}
		//光标显示
		if(OLEDState == 1)
		{
			OLEDState = 0;
			switch(Level_SetFlag)
			{
				case 1:
					First_Set_UI();
					OLED_ReverseArea(55,0,8,16);
					break;
				case 2:
					First_Set_UI();
					OLED_ReverseArea(71,0,8,16);
					break;
				case 3:
					First_Set_UI();
					OLED_ReverseArea(87,0,8,16);
					break;
				case 4:
					First_Set_UI();
					OLED_ReverseArea(103,0,8,16);
					break;
			}
			OLED_Update();
		}
	}
}

int Round_Set(void)
{
	OLED_Clear();
	int Round_SetFlag = 2;
	while(1)
	{
		Encoder_GetDelta();
		if(Encoder_State == 1)
		{
			if(Round_SetFlag < 2)
			{
				Round_SetFlag++;
			}
		}
		else if(Encoder_State == -1)
		{
			if(Round_SetFlag > 1)
			{
				Round_SetFlag--;
			}
		}
		//按键按下
		if(KeyState == 1)
		{
			KeyState = 0;
			if(Round_SetFlag == 1)
			{
				//开启摇头
				isRound = 1;
				return 0;
			}
			else if(Round_SetFlag == 2)
			{
				//关闭摇头
				isRound = 0;
				return 0;
			}
		}
		//光标显示
		if(OLEDState == 1)
		{
			OLEDState = 0;
			switch(Round_SetFlag)
			{
				case 1:
					First_Set_UI();
					OLED_ReverseArea(55,15,16,16);
					break;
				case 2:
					First_Set_UI();
					OLED_ReverseArea(79,15,24,16);
					break;
			}
			OLED_Update();
		}
	}
}

int PID_Set(int SetFlag)
{
	if(SetFlag == 3)
	{
		OLED_Clear();
		while(1)
		{
			Encoder_GetDelta();
			if(Encoder_State == 1)
			{
				Kp+=5;
			}
			else if(Encoder_State == -1)
			{
				Kp-=5;
			}
			//按键按下
			if(KeyState == 1)
			{
				KeyState = 0;
				return 0;
			}
			//光标显示
			if(OLEDState == 1)
			{
				OLEDState = 0;
				First_Set_UI();
				OLED_ReverseArea(16,31,24,16);
				OLED_Update();
			}
		}
	}
	if(SetFlag == 4)
	{
		OLED_Clear();
		while(1)
		{
			Encoder_GetDelta();
			if(Encoder_State == 1)
			{
				Ki+=5;
			}
			else if(Encoder_State == -1)
			{
				Ki-=5;
			}
			//按键按下
			if(KeyState == 1)
			{
				KeyState = 0;
				return 0;
			}
			//光标显示
			if(OLEDState == 1)
			{
				OLEDState = 0;
				First_Set_UI();
				OLED_ReverseArea(60,31,24,16);
				OLED_Update();
			}
		}
	}
	if(SetFlag == 5)
	{
		OLED_Clear();
		while(1)
		{
			Encoder_GetDelta();
			if(Encoder_State == 1)
			{
				Kd+=5;
			}
			else if(Encoder_State == -1)
			{
				Kd-=5;
			}
			//按键按下
			if(KeyState == 1)
			{
				KeyState = 0;
				return 0;
			}
			//光标显示
			if(OLEDState == 1)
			{
				OLEDState = 0;
				First_Set_UI();
				OLED_ReverseArea(104,31,24,16);
				OLED_Update();
			}
		}
	}
}