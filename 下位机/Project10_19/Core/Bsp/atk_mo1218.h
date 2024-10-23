/**
 ****************************************************************************************************
 * @file        atk_mo1218.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MO1218ģ����������
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

#ifndef __ATK_MO1218_H
#define __ATK_MO1218_H

#include "main.h"
#include "atk_mo1218_bin_msg.h"
#include "atk_mo1218_nmea_msg.h"
#include "atk_mo1218_uart.h"

/* ATK-MO1218ģ��UTC�ṹ�� */
typedef struct
{
    uint16_t year;                                  /* �� */
    uint8_t month;                                  /* �� */
    uint8_t day;                                    /* �� */
    uint8_t hour;                                   /* ʱ */
    uint8_t minute;                                 /* �� */
    uint8_t second;                                 /* �� */
    uint16_t millisecond;                           /* ���� */
} atk_mo1218_time_t;

/* ATK-MO1218ģ��λ�ýṹ�� */
typedef struct
{
    atk_mo1218_latitude_t latitude;                 /* γ����Ϣ */
    atk_mo1218_longitude_t longitude;               /* ������Ϣ */
} atk_mo1218_position_t;

/* ATK-MO1218ģ�鶨λ��Ϣ�ṹ�� */
typedef struct
{
    atk_mo1218_gps_quality_indicator_t quality;     /* ��λ���� */
    uint8_t satellite_num;                          /* ���ڶ�λ���������� */
    atk_mo1218_fix_type_t type;                     /* ��λ���� */
    uint16_t satellite_id[12];                      /* ���ڶ�λ�����Ǳ�� */
    uint16_t pdop;                                  /* λ�þ������ӣ�����10���� */
    uint16_t hdop;                                  /* ˮƽ�������ӣ�����10���� */
    uint16_t vdop;                                  /* ��ֱ�������ӣ�����10���� */
} atk_mo1218_fix_info_t;

/* ATK-MO1218ģ��ɼ�������Ϣ�ṹ�� */
typedef struct
{
    uint8_t satellite_num;                          /* �ɼ��������� */
    atk_mo1218_satellite_info_t satellite_info[12]; /* �ɼ�������Ϣ */
} atk_mo1218_visible_satellite_info_t;

/* ������� */
#define ATK_MO1218_EOK      0                       /* û�д��� */
#define ATK_MO1218_ERROR    1                       /* ���� */
#define ATK_MO1218_ETIMEOUT 2                       /* ��ʱ���� */
#define ATK_MO1218_EINVAL   3                       /* �������� */

/* �������� */
uint8_t atk_mo1218_init(uint32_t baudrate);                                                                                                                                                                                                                                                     /* ATK-MO1218��ʼ�� */
uint8_t atk_mo1218_update(atk_mo1218_time_t *utc, atk_mo1218_position_t *position, int16_t *altitude, uint16_t *speed, atk_mo1218_fix_info_t *fix_info, atk_mo1218_visible_satellite_info_t *gps_satellite_info, atk_mo1218_visible_satellite_info_t *beidou_satellite_info, uint32_t timeout); /* ��ȡ������ATK-MO1218ģ������ */

#endif
