/**
 ****************************************************************************************************
 * @file        atk_mo1218_uart.c
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

#include "atk_mo1218_uart.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static struct
{
    uint8_t buf[ATK_MO1218_UART_RX_BUF_SIZE];               /* ֡���ջ��� */
    struct
    {
        uint16_t len    : 15;                               /* ֡���ճ��ȣ�sta[14:0] */
        uint16_t finsh  : 1;                                /* ֡������ɱ�־��sta[15] */
    } sta;                                                  /* ֡״̬��Ϣ */
} g_uart_rx_frame = {0};                                    /* ATK-MO1218 UART����֡������Ϣ�ṹ�� */
static uint8_t g_uart_tx_buf[ATK_MO1218_UART_TX_BUF_SIZE];  /* ATK-MO1218 UART���ͻ��� */

/**
 * @brief       ATK-MO1218 UART��������
 * @param       dat: �����͵�����
 *              len: ���������ݵĳ���
 * @retval      ��
 */
void atk_mo1218_uart_send(uint8_t *dat, uint8_t len)
{
    HAL_UART_Transmit(&huart5, dat, len, HAL_MAX_DELAY);
    printf("send!\r\n");
}

/**
 * @brief       ATK-MO1218 UART printf
 * @param       fmt: ����ӡ������
 * @retval      ��
 */
void atk_mo1218_uart_printf(char *fmt, ...)
{
    va_list ap;
    uint16_t len;
    
    va_start(ap, fmt);
    vsprintf((char *)g_uart_tx_buf, fmt, ap);
    va_end(ap);
    
    len = strlen((const char *)g_uart_tx_buf);
    HAL_UART_Transmit(&huart5, g_uart_tx_buf, len, HAL_MAX_DELAY);
}

/**
 * @brief       ATK-MO1218 UART���¿�ʼ��������
 * @param       ��
 * @retval      ��
 */
void atk_mo1218_uart_rx_restart(void)
{
    g_uart_rx_frame.sta.len     = 0;
    g_uart_rx_frame.sta.finsh   = 0;
}

/**
 * @brief       ��ȡATK-MO1218 UART���յ���һ֡����
 * @param       ��
 * @retval      NULL: δ���յ�һ֡����
 *              ����: ���յ���һ֡����
 */
uint8_t *atk_mo1218_uart_rx_get_frame(void)
{
    if (g_uart_rx_frame.sta.finsh == 1)
    {
        g_uart_rx_frame.buf[g_uart_rx_frame.sta.len] = '\0';
        return g_uart_rx_frame.buf;
    }
    else
    {
        return NULL;
    }
}

/**
 * @brief       ��ȡATK-MO1218 UART���յ���һ֡���ݵĳ���
 * @param       ��
 * @retval      0   : δ���յ�һ֡����
 *              ����: ���յ���һ֡���ݵĳ���
 */
uint16_t atk_mo1218_uart_rx_get_frame_len(void)
{
    if (g_uart_rx_frame.sta.finsh == 1)
    {
        return g_uart_rx_frame.sta.len;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief       ATK-MO1218 UART��ʼ��
 * @param       baudrate: UARTͨѶ������
 * @retval      ��
 */
void atk_mo1218_uart_init(uint32_t baudrate)
{
    huart5.Instance          = ATK_MO1218_UART_INTERFACE;    /* ATK-MO1218 UART */
    huart5.Init.BaudRate     = baudrate;                     /* ������ */
    huart5.Init.WordLength   = UART_WORDLENGTH_8B;           /* ����λ */
    huart5.Init.StopBits     = UART_STOPBITS_1;              /* ֹͣλ */
    huart5.Init.Parity       = UART_PARITY_NONE;             /* У��λ */
    huart5.Init.Mode         = UART_MODE_TX_RX;              /* �շ�ģʽ */
    huart5.Init.HwFlowCtl    = UART_HWCONTROL_NONE;          /* ��Ӳ������ */
    huart5.Init.OverSampling = UART_OVERSAMPLING_16;         /* ������ */
    HAL_UART_Init(&huart5);                                  /* ʹ��ATK-MO1218 UART
                                                                     * HAL_UART_Init()����ú���HAL_UART_MspInit()
                                                                     * �ú����������ļ�usart.c��
                                                                     */
}

/**
 * @brief       ATK-MO1218 UART�жϻص�����
 * @param       ��
 * @retval      ��
 */
void ATK_MO1218_UART_IRQHandler(void)
{
    uint8_t tmp;
    
    if (__HAL_UART_GET_FLAG(&huart5, UART_FLAG_ORE) != RESET)        /* UART���չ��ش����ж� */
    {
        __HAL_UART_CLEAR_OREFLAG(&huart5);                           /* ������չ��ش����жϱ�־ */
        (void)huart5.Instance->SR;                                   /* �ȶ�SR�Ĵ������ٶ�DR�Ĵ��� */
        (void)huart5.Instance->DR;
    }
    
    if (__HAL_UART_GET_FLAG(&huart5, UART_FLAG_RXNE) != RESET)       /* UART�����ж� */
    {
        HAL_UART_Receive(&huart5, &tmp, 1, HAL_MAX_DELAY);           /* UART�������� */
        
        if (g_uart_rx_frame.sta.len < (ATK_MO1218_UART_RX_BUF_SIZE - 1))    /* �ж�UART���ջ����Ƿ����
                                                                             * ����һλ��������'\0'
                                                                             */
        {
            g_uart_rx_frame.buf[g_uart_rx_frame.sta.len] = tmp;             /* �����յ�������д�뻺�� */
            g_uart_rx_frame.sta.len++;                                      /* ���½��յ������ݳ��� */
        }
        else                                                                /* UART���ջ������ */
        {
            g_uart_rx_frame.sta.len = 0;                                    /* ����֮ǰ�յ������� */
            g_uart_rx_frame.buf[g_uart_rx_frame.sta.len] = tmp;             /* �����յ�������д�뻺�� */
            g_uart_rx_frame.sta.len++;                                      /* ���½��յ������ݳ��� */
        }
    }
    
    if (__HAL_UART_GET_FLAG(&huart5, UART_FLAG_IDLE) != RESET)       /* UART���߿����ж� */
    {
        g_uart_rx_frame.sta.finsh = 1;                                      /* ���֡������� */
        
        __HAL_UART_CLEAR_IDLEFLAG(&huart5);                          /* ���UART���߿����ж� */
    }
}
