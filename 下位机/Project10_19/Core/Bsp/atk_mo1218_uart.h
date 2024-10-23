/**
 ****************************************************************************************************
 * @file        atk_mo1218_uart.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MO1218ģ��UART�ӿ���������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� MiniSTM32 V4������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#ifndef __ATK_MO1218_UART_H
#define __ATK_MO1218_UART_H

#include "main.h"
#include "usart.h"

/* ���Ŷ��� */
#define ATK_MO1218_UART_TX_GPIO_PORT            GPIOC
#define ATK_MO1218_UART_TX_GPIO_PIN             GPIO_PIN_12
#define ATK_MO1218_UART_TX_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
                                                
#define ATK_MO1218_UART_RX_GPIO_PORT            GPIOD
#define ATK_MO1218_UART_RX_GPIO_PIN             GPIO_PIN_2
#define ATK_MO1218_UART_RX_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)
                                                
#define ATK_MO1218_UART_INTERFACE               UART5
#define ATK_MO1218_UART_IRQn                    UART5_IRQn
#define ATK_MO1218_UART_IRQHandler              UART5_IRQHandler
#define ATK_MO1218_UART_CLK_ENABLE()            do{ __HAL_RCC_UART5_CLK_ENABLE(); }while(0)

/* UART�շ������С */
#define ATK_MO1218_UART_RX_BUF_SIZE             2048
#define ATK_MO1218_UART_TX_BUF_SIZE             64

/* �������� */
void atk_mo1218_uart_send(uint8_t *dat, uint8_t len);   /* ATK-MO1218 UART�������� */
void atk_mo1218_uart_printf(char *fmt, ...);            /* ATK-MO1218 UART printf */
void atk_mo1218_uart_rx_restart(void);                  /* ATK-MO1218 UART���¿�ʼ�������� */
uint8_t *atk_mo1218_uart_rx_get_frame(void);            /* ��ȡATK-MO1218 UART���յ���һ֡���� */
uint16_t atk_mo1218_uart_rx_get_frame_len(void);        /* ��ȡATK-MO1218 UART���յ���һ֡���ݵĳ��� */
void atk_mo1218_uart_init(uint32_t baudrate);           /* ATK-MO1218 UART��ʼ�� */

#endif
