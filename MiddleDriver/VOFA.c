#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"

#include "VOFA.h"
#include "Encoder.h"
#include "Menu.h"
#include "PID.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char rx_buf[64];
uint8_t rx_idx = 0;
uint8_t rx_byte = 0;

/**********  		VOFA上位机数据解析		***********/
void VOFA_data_parse(char *cmd)
{
	if(cmd[1] != '=')
	{
		return;
	}
	
	int val = strtol(&cmd[2], NULL,10);
	
	switch(cmd[0])
	{
		case 'P':
			Kp = val;
			break;
		case 'I':
			Ki = val;
			break;
		case 'D':
			Kd = val;
			break;
		case 'L':
			Level = val;
			break;
		case 'R':
			isRound = val;
			break;
		default:
			break;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
//		switch(rx_data)
//		{
//			case 0x00:
//				Level = 0;
//				uart_printf_isr("Level:0\r\n");
//				break;
//			case 0x01:
//				Level = 1;
//				uart_printf_isr("Level:1\r\n");
//				break;
//			case 0x02:
//				Level = 2;
//				uart_printf_isr("Level:2\r\n");
//				break;
//			case 0x03:
//				Level = 3;
//				uart_printf_isr("Level:3\r\n");
//				break;
//			case 0x10:
//				isRound = 0;
//				uart_printf_isr("Round OFF!\r\n");
//				break;
//			case 0x11:
//				isRound = 1;
//				uart_printf_isr("Round ON!\r\n");
//				break;
//		}
		if(rx_byte == '\n' || rx_byte == '\r')		//截断
		{
			if(rx_idx > 0)
			{
				rx_buf[rx_idx] = '\0';	//转换为字符串
				VOFA_data_parse(rx_buf);
				rx_idx = 0;
			}
		}
		else
		{
			if(rx_idx < 63)
			{
				rx_buf[rx_idx++] = (char)rx_byte;
			}
			else
			{
				rx_idx = 0;
			}
		}
		
		HAL_UART_Receive_IT(&huart1,&rx_byte,1);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		__HAL_UART_CLEAR_OREFLAG(huart);
		huart->ErrorCode = HAL_UART_ERROR_NONE;
		HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
	}
}


