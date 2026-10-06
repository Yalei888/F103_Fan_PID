#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"

#include "SG09.h"
#include "Encoder.h"
#include "Menu.h"
#include "motor.h"
#include "PID.h"


int16_t ccr = 75;


void SG09_Init(void)
{
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,ccr);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim4);
}

void SG09_Start(void)
{
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
}

void SG09_Stop(void)
{
	HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_4);
}

int cnt1 = 0,cnt2 = 0,cnt3 = 0,cnt4 = 0;
volatile uint8_t direState,KeyState,OLEDState;
volatile int16_t act_speed,cur_speed;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/*	20ms定时器中断 	*/
	if(htim == &htim2)
	{
		/*	摇头控制   */
		if(isRound)
		{
			cnt1++;
			if(cnt1 == 5)
			{
				cnt1 = 0;
				if(ccr == 115)
				{
					direState = 1;
				}
				if(ccr == 35)
				{
					direState = 0;
				}
				
				if(direState == 0)
				{
					ccr+=1;
				}
				else if(direState == 1)
				{
					ccr-=1;
				}
				
				ccr = ccr > 125 ? 125 : ccr;
				ccr = ccr < 25 ? 25 : ccr;
				__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,ccr);
			}
		}
		/* 	按键检测 	*/
		if(cnt2 == 1)
		{ 
			cnt2 = 0;
			if(HAL_GPIO_ReadPin(Key_GPIO_Port,Key_Pin)==GPIO_PIN_RESET)
			{
				KeyState = 1;
			}
		}
		if(HAL_GPIO_ReadPin(Key_GPIO_Port,Key_Pin)==GPIO_PIN_SET)
		{
			cnt2++;
		}
		
		/*  屏幕刷新   */
		cnt3++;
		if(cnt3 == 5)
		{
			cnt3 = 0;
			OLEDState = 1;
		}
		
		/*	编码器测速  */
		cur_speed = Motor_GetSpeed();
		/*	PID控制		*/
		if(Level == 1)
		{
			act_speed = LEVEL1_SPEED;
			PID_Control(LEVEL1_SPEED,cur_speed);
		}
		else if(Level == 2)
		{
			act_speed = LEVEL2_SPEED;
			PID_Control(LEVEL2_SPEED,cur_speed);
		}
		else if(Level == 3)
		{
			act_speed = LEVEL3_SPEED;
			PID_Control(LEVEL3_SPEED,cur_speed);
		}
		else
		{
			act_speed = 0;
			Motor_Load(0);
		}
		
	}
	/*	50ms定时器中断 	*/
	if(htim == &htim4)
	{
		/*	串口打印   */
		cnt4++;
		if(cnt4 == 1)
		{
			cnt4 = 0;
			uart_printf_isr("%d,%d,%d,%d,%d,%d,%d\r\n",Kp,Ki,Kd,Level,isRound,act_speed,cur_speed);
		}
	}
}