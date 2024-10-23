/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */

#define TX_BUFFER_MAX 80

extern uint8_t aRx1Buffer;			//接收中断缓冲
extern uint8_t Uart1_Rx_Cnt;		//接收缓冲计数
extern uint8_t aRx3Buffer;			//接收中断缓冲
extern uint8_t Uart3_Rx_Cnt;		//接收缓冲计数

extern uint8_t TX3_BUFFER[TX_BUFFER_MAX];
extern uint8_t RX3_BUFFER[TX_BUFFER_MAX];

#define zhen_head 0x88
#define zhen_tail 0x99

typedef struct send_zhen{
	uint8_t send_buffer[10];
	short earth_temp;
	short earth_wet;
	short env_temp;
	short env_wet;
}Send_Zhen;

typedef struct recv_zhen{
	uint8_t send_buffer[8];
  int8_t vel_x;
	int8_t vel_y;
	uint8_t theta;
	int8_t pos_x;
	int8_t pos_y;
	uint8_t pos_z;
}Recv_Zhen;

extern Send_Zhen send_zhen;
extern Recv_Zhen recv_zhen;
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

