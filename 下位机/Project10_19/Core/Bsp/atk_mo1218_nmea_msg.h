/**
 ****************************************************************************************************
 * @file        atk_mo1218_nmea_msg.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MO1218ģ��NMEA��������
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

#ifndef __ATK_MO1218_NMEA_MSG_H
#define __ATK_MO1218_NMEA_MSG_H

#include "main.h"

/* ATK-MO1218ģ��NMEA��Ϣ����ö�� */
typedef enum
{
    ATK_MO1218_NMEA_MSG_GNGGA = 0x00,               /* $GNGGA */
    ATK_MO1218_NMEA_MSG_GNGLL,                      /* $GNGLL */
    ATK_MO1218_NMEA_MSG_GNGSA,                      /* $GNGSA */
    ATK_MO1218_NMEA_MSG_GPGSA,                      /* $GPGSA */
    ATK_MO1218_NMEA_MSG_BDGSA,                      /* $BDGSA */
    ATK_MO1218_NMEA_MSG_GPGSV,                      /* $GPGSV */
    ATK_MO1218_NMEA_MSG_BDGSV,                      /* $BDGSV */
    ATK_MO1218_NMEA_MSG_GNRMC,                      /* $GNRMC */
    ATK_MO1218_NMEA_MSG_GNVTG,                      /* $GNVTG */
    ATK_MO1218_NMEA_MSG_GNZDA,                      /* $GNZDA */
} atk_mo1218_nmea_msg_t;

/* ATK-MO1218ģ�鱱γ��γö�� */
typedef enum
{
    ATK_MO1218_LATITUDE_NORTH = 0x00,               /* ��γ */
    ATK_MO1218_LATITUDE_SOUTH,                      /* ��γ */
} atk_mo1218_latitude_indicator_t;

/* ATK-MO1218ģ�鶫������ö�� */
typedef enum
{
    ATK_MO1218_LONGITUDE_EAST = 0x00,               /* ���� */
    ATK_MO1218_LONGITUDE_WEST,                      /* ���� */
} atk_mo1218_longitude_indicator_t;

/* ATK-MO1218ģ��GPS����ָ��ö�� */
typedef enum
{
    ATK_MO1218_GPS_UNAVAILABLE = 0x00,              /* ��λ������ */
    ATK_MO1218_GPS_VALID_SPS,                       /* ��λ��Ч��SPSģʽ */
    ATK_MO1218_GPS_VALID_DIFFERENTIAL,              /* ��λ��Ч�����GPSģʽ */
} atk_mo1218_gps_quality_indicator_t;

/* ATK-MO1218ģ��NMEA��Ϣ XXGLL����״̬ö�� */
typedef enum
{
    ATK_MO1218_XXGLL_DATA_VALID = 0x00,             /* XXGLL������Ч */
    ATK_MO1218_XXGLL_DATA_NO_VALID,                 /* XXGLL���ݲ����� */
} atk_mo1218_xxgll_status_t;

/* ATK-MO1218ģ��GPS����������ģʽö�� */
typedef enum
{
    ATK_MO1218_GPS_OPERATING_MANUAL = 0x00,         /* �ֶ���ǿ����2D��3Dģʽ�²��� */
    ATK_MO1218_GPS_OPERATING_AUTOMATIC,             /* �Զ��������Զ��л�2D��3Dģʽ */
} atk_mo1218_gps_operation_mode_t;

/* ATK-MO1218ģ��Ķ�λ���� */
typedef enum
{
    ATK_MO1218_FIX_NOT_AVAILABLE = 0x00,            /* δ��λ */
    ATK_MO1218_FIX_2D,                              /* 2D */
    ATK_MO1218_FIX_3D,                              /* 3D */
} atk_mo1218_fix_type_t;

/* ATK-MO1218ģ�鵼������״̬ö�� */
typedef enum
{
    ATK_MO1218_NAVIGATION_WARNING = 0x00,           /* ����������Ч */
    ATK_MO1218_NAVIGATION_VALID,                    /* ����������Ч */
} atk_mo1218_navigation_data_status_t;

/* ATK-MO1218ģ�鶨��ϵͳģʽö�� */
typedef enum
{
    ATK_MO1218_POS_SYS_DATA_NOT_VALID = 0x00,       /* ������Ч */
    ATK_MO1218_POS_SYS_AUTONOMOUS,                  /* ������λ */
    ATK_MO1218_POS_SYS_DIFFERENTIAL,                /* ��� */
    ATK_MO1218_POS_SYS_ESTIMATED,                   /* ���� */
} atk_mo1218_pos_sys_mode_t;

/* ATK-MO1218ģ��UTCʱ��ṹ�� */
typedef struct
{
    uint8_t hour;                                   /* ʱ */
    uint8_t minute;                                 /* �� */
    uint8_t second;                                 /* �� */
    uint16_t millisecond;                           /* ���� */
} atk_mo1218_utc_time_t;

/* ATK-MO1218ģ��UTC���ڽṹ�� */
typedef struct
{
    uint16_t year;                                  /* �� */
    uint8_t month;                                  /* �� */
    uint8_t day;                                    /* �� */
} atk_mo1218_utc_date_t;

/* ATK-MO1218ģ��γ�Ƚṹ�� */
typedef struct
{
    atk_mo1218_latitude_indicator_t indicator;      /* ָʾ��γ����γ */
    uint32_t degree;                                /* γ�ȣ�����100000������λ���� */
} atk_mo1218_latitude_t;

/* ATK-MO1218ģ�龭�Ƚṹ�� */
typedef struct
{
    atk_mo1218_longitude_indicator_t indicator;     /* ָʾ���������� */
    uint32_t degree;                                /* ���ȣ�����100000������λ���� */
} atk_mo1218_longitude_t;

/* ATK-MO1218ģ��������Ϣ */
typedef struct
{
    uint16_t satellite_id;                          /* ����ID */
    uint8_t elevation;                              /* �������ǣ���λ���� */
    uint16_t azimuth;                               /* ���Ƿ�λ�ǣ���λ���� */
    uint8_t snr;                                    /* ����ȣ���λ��dB */
} atk_mo1218_satellite_info_t;

/* ATK-MO1218ģ��NMEA��ϢXXGGA��Ϣ�ṹ�� */
typedef struct
{
    atk_mo1218_utc_time_t utc_time;                 /* UTCʱ�� */
    atk_mo1218_latitude_t latitude;                 /* γ�� */
    atk_mo1218_longitude_t longitude;               /* ���� */
    atk_mo1218_gps_quality_indicator_t gps_quality; /* GPS����ָ�� */
    uint8_t satellite_num;                          /* ʹ�õ��������� */
    uint16_t hdop;                                  /* ˮƽ�������ӣ�����10�� */
    int32_t altitude;                               /* ���θ߶ȣ�����10�� */
    /* ���ˮ׼���� */
    uint16_t dgps_id;                               /* DGPSվID */
} atk_mo1218_nmea_gga_msg_t;

/* ATK-MO1218ģ��NMEA��ϢXXGLL��Ϣ�ṹ�� */
typedef struct
{
    atk_mo1218_latitude_t latitude;                 /* γ�� */
    atk_mo1218_longitude_t longitude;               /* ���� */
    atk_mo1218_utc_time_t utc_time;                 /* UTCʱ�� */
    atk_mo1218_xxgll_status_t status;               /* ����״̬ */
} atk_mo1218_nmea_gll_msg_t;

/* ATK-MO1218ģ��NMEA��ϢXXGSA��Ϣ�ṹ�� */
typedef struct
{
    atk_mo1218_gps_operation_mode_t mode;           /* GPS����������ģʽ */
    atk_mo1218_fix_type_t type;                     /* ��λ���� */
    uint8_t satellite_id[12];                       /* ����ID */
    uint16_t pdop;                                  /* λ�þ������ӣ�����10�� */
    uint16_t hdop;                                  /* ˮƽ�������ӣ�����10�� */
    uint16_t vdop;                                  /* ��ֱ�������ӣ�����10�� */
} atk_mo1218_nmea_gsa_msg_t;

/* ATK-MO1218ģ��NMEA��ϢXXGSV��Ϣ�ṹ�� */
typedef struct
{
    uint8_t satellite_view;                         /* �ɼ��������� */
    atk_mo1218_satellite_info_t satellite_info[12]; /* ������Ϣ */
} atk_mo1218_nmea_gsv_msg_t;

/* ATK-MO1218ģ��NMEA��ϢXXRMC��Ϣ�ṹ�� */
typedef struct
{
    atk_mo1218_utc_time_t utc_time;                 /* UTCʱ�� */
    atk_mo1218_utc_date_t utc_date;                 /* UTC���� */
    atk_mo1218_navigation_data_status_t status;     /* ��������״̬ */
    atk_mo1218_latitude_t latitude;                 /* γ�� */
    atk_mo1218_longitude_t longitude;               /* ���� */
    uint16_t speed_ground;                          /* �����ٶȣ�����10������λ���ڣ�knot�� */
    uint16_t course_ground;                         /* ���溽������10������λ���� */
    atk_mo1218_pos_sys_mode_t position_system_mode; /* ��λϵͳģʽ */
} atk_mo1218_nmea_rmc_msg_t;

/* ATK-MO1218ģ��NMEA��ϢXXVTG��Ϣ�ṹ�� */
typedef struct
{
    uint16_t course_true;                           /* ���溽������10������λ���� */
    uint16_t course_magnetic;                       /* ���溽������10������λ���� */
    uint16_t speed_knots;                           /* �����ٶȣ�����10������λ���ڣ�knot�� */
    uint16_t speed_kph;                             /* �����ٶȣ�����10������λ��ǧ��/ʱ */
    atk_mo1218_pos_sys_mode_t position_system_mode; /* ��λϵͳģʽ */
} atk_mo1218_nmea_vtg_msg_t;

/* ATK-MO1218ģ��NMEA��ϢXXZDA��Ϣ�ṹ�� */
typedef struct
{
    atk_mo1218_utc_time_t utc_time;                 /* UTCʱ�� */
    atk_mo1218_utc_date_t utc_date;                 /* UTC���� */
    int8_t local_zone_hour;                         /* ����ʱ��ʱ����Χ��-13~+13 */
    uint8_t local_zone_minute;                      /* ����ʱ���֣���Χ��0~59 */
} atk_mo1218_nmea_zda_msg_t;

/* �������� */
uint8_t atk_mo1218_get_nmea_msg_from_buf(uint8_t *buf, atk_mo1218_nmea_msg_t nmea, uint8_t msg_index, uint8_t **msg);   /* �����ݻ����л�ȡָ�����ͺ�������NMEA��Ϣ */
uint8_t atk_mo1218_decode_nmea_xxgga(uint8_t *xxgga_msg, atk_mo1218_nmea_gga_msg_t *decode_msg);                        /* ����$XXGGA���͵�NMEA��Ϣ */
uint8_t atk_mo1218_decode_nmea_xxgll(uint8_t *xxgll_msg, atk_mo1218_nmea_gll_msg_t *decode_msg);                        /* ����$XXGLL���͵�NMEA��Ϣ */
uint8_t atk_mo1218_decode_nmea_xxgsa(uint8_t *xxgsa_msg, atk_mo1218_nmea_gsa_msg_t *decode_msg);                        /* ����$XXGSA���͵�NMEA��Ϣ */
uint8_t atk_mo1218_decode_nmea_xxgsv(uint8_t *xxgsv_msg, atk_mo1218_nmea_gsv_msg_t *decode_msg);                        /* ����$XXGSV���͵�NMEA��Ϣ */
uint8_t atk_mo1218_decode_nmea_xxrmc(uint8_t *xxrmc_msg, atk_mo1218_nmea_rmc_msg_t *decode_msg);                        /* ����$XXRMC���͵�NMEA��Ϣ */
uint8_t atk_mo1218_decode_nmea_xxvtg(uint8_t *xxvtg_msg, atk_mo1218_nmea_vtg_msg_t *decode_msg);                        /* ����$XXVTG���͵�NMEA��Ϣ */
uint8_t atk_mo1218_decode_nmea_xxzda(uint8_t *xxzda_msg, atk_mo1218_nmea_zda_msg_t *decode_msg);                        /* ����$XXZDA���͵�NMEA��Ϣ */

#endif
