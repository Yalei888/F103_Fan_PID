#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"

#include "motor.h"
#include "Encoder.h"
#include "Menu.h"

void Motor_Init(void)
{
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
	HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);
	Motor_Load(0);
}

void Motor_Load(int PWM)
{
	if(PWM > 7200)
	{
		PWM = 7200;
	}
	if(PWM < -7200)
	{
		PWM = -7200;
	}
	if(PWM >= 0)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,PWM);
	}
	else if(PWM < 0)
	{
		PWM = -PWM;
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,PWM);
	}
	
}

int16_t Motor_GetSpeed(void)
{
	int16_t cnt = __HAL_TIM_GET_COUNTER(&htim3);
	__HAL_TIM_SET_COUNTER(&htim3,0);
	return cnt;
}