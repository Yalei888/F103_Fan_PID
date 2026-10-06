#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

#include "Encoder.h"


volatile int32_t encoder_pos = 0;
int32_t last_pos,current_pos;
int Encoder_State;

void Encoder_GetDelta(void)
{
	current_pos = encoder_pos;
		
	if(current_pos > last_pos)
	{
		Encoder_State = 1;
	}
	
	else if(current_pos < last_pos)
	{
		Encoder_State = -1;
	}
	else
	{
		Encoder_State = 0;
	}
	last_pos = current_pos;
}

void HAL_SYSTICK_Callback(void)
{
	uint8_t pin_a = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
	uint8_t pin_b = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);

	
	static uint8_t stage = 0;
	static int8_t direction = 0;
	
	if (stage == 0)
	{
			if (pin_a == 0 && pin_b == 1)
			{
					stage = 1;          
					direction = 1;
			}
			else if (pin_a == 1 && pin_b == 0)
			{
					stage = 2;          
					direction = -1;
			}
	}
	else if (stage == 1 || stage == 2)
	{
			if (pin_a == 1 && pin_b == 1)
			{
					stage = 0;          
					direction = 0;
			}
			else if (pin_a == 0 && pin_b == 0)
			{
					stage = 3;        
			}
	}
	else if (stage == 3)
	{
			if (pin_a == 1 && pin_b == 1)
			{
					if (direction == 1) {
							encoder_pos++; 
					} else if (direction == -1) {
							encoder_pos--;  
					}
					stage = 0;         
					direction = 0;
			}
	}
}
//void Encoder_GetKey(void)
//{
//	if(HAL_GPIO_ReadPin(Key_GPIO_Port,Key_Pin)==GPIO_PIN_RESET)
//	{
//		Key_State = 1;
//		HAL_Delay(20);
//		while(HAL_GPIO_ReadPin(Key_GPIO_Port,Key_Pin)==GPIO_PIN_RESET);
//	}
//	else
//	{
//		Key_State = 0;
//	}
//}

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	if(GPIO_Pin == GPIO_PIN_0)
//	{
//		static uint32_t last_time = 0;
//		uint32_t current_time = HAL_GetTick();
//		if(current_time - last_time > 50)
//		{
//			for (volatile int i = 0; i < 50; i++); 
//			if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
//			{
//				 encoder_pos += (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_SET) ? 1 : -1;
//			}
//			last_time = current_time;
//		}
//		
//	}
//}

