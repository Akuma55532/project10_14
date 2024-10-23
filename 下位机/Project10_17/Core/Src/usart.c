/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <string.h>
#include "motor.h"
#include "servo.h"
#include "tim.h"

#include "ps2.h"

uint8_t aRx1Buffer;			//接收中断缓冲
uint8_t Uart1_Rx_Cnt = 0;		//接收缓冲计数
uint8_t aRx3Buffer;			//接收中断缓冲
uint8_t Uart3_Rx_Cnt = 0;		//接收缓冲计数

char RxBuffer[80];   //接收数据

uint8_t TX3_BUFFER[TX_BUFFER_MAX];
uint8_t RX3_BUFFER[TX_BUFFER_MAX];


Send_Zhen send_zhen;
Recv_Zhen recv_zhen;
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_HalfDuplex_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */


 
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
   */
	if(huart->Instance == USART1)
	{
		if(Uart1_Rx_Cnt >= 255)  //溢出判断
		{
			Uart1_Rx_Cnt = 0;
			memset(RxBuffer,0x00,sizeof(RxBuffer));
			HAL_UART_Transmit(&huart1, (uint8_t *)"数据溢出", 10,0xFFFF); 	
		}
		else
		{
			RxBuffer[Uart1_Rx_Cnt++] = aRx1Buffer;   //接收数据转存
			if((RxBuffer[Uart1_Rx_Cnt-1] == 0x0A)&&(RxBuffer[Uart1_Rx_Cnt-2] == 0x0D)) //判断结束�?
			{
				if((RxBuffer[0] == 'W') && (Uart1_Rx_Cnt == 3)&& (ps2_flag == 0))
				{
					move_forward();
				}
				else if((RxBuffer[0] == 'S') && (Uart1_Rx_Cnt == 3)&& (ps2_flag == 0))
				{
					move_stop();
				}
				else if((RxBuffer[0] == 'X') && (Uart1_Rx_Cnt == 3)&& (ps2_flag == 0))
				{
					move_backward();
				}
				else if((RxBuffer[0] == 'A') && (Uart1_Rx_Cnt == 3)&& (ps2_flag == 0))
				{
					move_left();
				}
				else if((RxBuffer[0] == 'D') && (Uart1_Rx_Cnt == 3)&& (ps2_flag == 0))
				{
					move_right();
				}
				else if((RxBuffer[0] == 'Q') && (Uart1_Rx_Cnt == 3)&& (ps2_flag == 0))
				{
					turn_left();
				}
				else if((RxBuffer[0] == 'E') && (Uart1_Rx_Cnt == 3)&& (ps2_flag == 0))
				{
					turn_right();
				}
				if((RxBuffer[0] == 'I') && (Uart1_Rx_Cnt == 3))
				{
					if (arm_x < 13)
					{
						arm_x++;
						action_arm2();
					}
					servo_move_flag = 1;
				}
				else if((RxBuffer[0] == 'K') && (Uart1_Rx_Cnt == 3))
				{
					if (arm_x > 0)
					{
						arm_x--;
						action_arm2();
					}
					servo_move_flag = 1;
				}
				else if((RxBuffer[0] == 'J') && (Uart1_Rx_Cnt == 3))
				{
					if (arm_y < 8)
					{
						arm_y++;
						action_arm2();
					}
					servo_move_flag = 1;
				}
				else if((RxBuffer[0] == 'L') && (Uart1_Rx_Cnt == 3))
				{
					if (arm_y > 0)
					{
						arm_y--;
						action_arm2();
					}
					servo_move_flag = 1;
				}
				else if((RxBuffer[0] == 'C') && (Uart1_Rx_Cnt == 3))
				{
					__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2,1100);
				}
				else if((RxBuffer[0] == 'V') && (Uart1_Rx_Cnt == 3))
				{
					__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3,1000);
				}
				else if((RxBuffer[0] == 'B') && (Uart1_Rx_Cnt == 3))
				{
					__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4,1100);
				}
				
				if((RxBuffer[0] == 'Z') && (Uart1_Rx_Cnt == 3))
				{
					if(ps2_flag == 1)
						ps2_flag = 0;
					else
						ps2_flag = 1;
				}
				Uart1_Rx_Cnt = 0;
				memset(RxBuffer,0x00,sizeof(RxBuffer)); //清空数组
			}
		}
		HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRx1Buffer, 1);   //再开启接收中�?
	}
}



/* USER CODE END 1 */
