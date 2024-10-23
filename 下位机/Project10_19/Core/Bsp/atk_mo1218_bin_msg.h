/**
 ****************************************************************************************************
 * @file        atk_mo1218_bin_msg.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MO1218ģ��Binary Message��������
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

#ifndef __ATK_MO1218_BIN_MSG_H
#define __ATK_MO1218_BIN_MSG_H

#include "main.h"

/* �ȴ�ATK-MO1218ģ��Binary Message��Ӧ��ʱʱ�� */
#define ATK_MO1218_BIN_MSG_TIMEOUT                      50

/* ATK-MO1218ģ��Binary Message��Playload������С */
#define ATK_MO1218_BIN_MSG_PLAYLOAD_MAX_LEN             0x0057

/* ATK-MO1218��������ö�� */
typedef enum
{
    ATK_MO1218_RESTART_HOT = 0x00,                      /* ������ */
    ATK_MO1218_RESTART_WARM,                            /* ������ */
    ATK_MO1218_RESTART_COLD,                            /* ������ */
} atk_mo1218_restart_t;

/* ATK-MO1218ģ��ָ�������������ö�� */
typedef enum
{
    ATK_MO1218_FACTORY_RESET_NOREBOOT = 0x00,           /* �ָ��������ú����� */
    ATK_MO1218_FACTORY_RESET_REBOOT,                    /* �ָ��������ú����� */
} atk_mo1218_factory_reset_type_t;

/* ATK-MO1218ģ�鴮�ڲ�����ö�� */
typedef enum
{
    ATK_MO1218_SERIAL_BAUDRATE_4800 = 0x00,             /* 4800bps */
    ATK_MO1218_SERIAL_BAUDRATE_9600,                    /* 9600bps */
    ATK_MO1218_SERIAL_BAUDRATE_19200,                   /* 19200bps */
    ATK_MO1218_SERIAL_BAUDRATE_38400,                   /* 38400bps */
    ATK_MO1218_SERIAL_BAUDRATE_57600,                   /* 57600bps */
    ATK_MO1218_SERIAL_BAUDRATE_115200,                  /* 115200bps */
    ATK_MO1218_SERIAL_BAUDRATE_230400,                  /* 230400bps */
    ATK_MO1218_SERIAL_BAUDRATE_460800,                  /* 460800bps */
    ATK_MO1218_SERIAL_BAUDRATE_921600,                  /* 921600bps */
} atk_mo1218_serial_baudrate_t;

/* ATK-MO1218ģ�����ñ��淽ʽö�� */
typedef enum
{
    ATK_MO1218_SAVE_SRAM = 0x00,                        /* ���浽SRAM�����õ��綪ʧ�� */
    ATK_MO1218_SAVE_SRAM_FLASH,                         /* ���浽SRAM��Flash�����õ��粻��ʧ�� */
    ATK_MO1218_SAVE_TEMP,                               /* ��ʱ�� */
} atk_mo1218_save_type_t;

/* ATK-MO1218ģ���������ö�� */
typedef enum
{
    ATK_MO1218_NOOUTPUT = 0x00,                         /* ����� */
    ATK_MO1218_OUTPUT_NMEA,                             /* ���NMEA��Ϣ */
    ATK_MO1218_OUTPUT_BINARY,                           /* ���Binary��Ϣ */
} atk_mo1218_output_type_t;

/* ATK-MO1218ģ��ϵͳ��Դģʽö�� */
typedef enum
{
    ATK_MO1218_POWER_MODE_NORMAL = 0x00,                /* ����ģʽ */
    ATK_MO1218_POWER_MODE_SAVE,                         /* ʡ��ģʽ */
} atk_mo1218_power_mode_t;

/* ATK-MO1218ģ��λ�ø���Ƶ��ö�� */
typedef enum
{
    ATK_MO1218_POSITION_RATE_1HZ = 0x00,                /* 1Hz */
    ATK_MO1218_POSITION_RATE_2HZ,                       /* 2Hz */
    ATK_MO1218_POSITION_RATE_4HZ,                       /* 4Hz�����ڲ����������38400bps */
    ATK_MO1218_POSITION_RATE_5HZ,                       /* 5Hz�����ڲ����������38400bps */
    ATK_MO1218_POSITION_RATE_8HZ,                       /* 8Hz�����ڲ����������38400bps */
    ATK_MO1218_POSITION_RATE_10HZ,                      /* 10Hz�����ڲ����������38400bps */
    ATK_MO1218_POSITION_RATE_20HZ,                      /* 20Hz�����ڲ����������115200bps */
    ATK_MO1218_POSITION_RATE_25HZ,                      /* 25Hz�����ڲ����������115200bps */
    ATK_MO1218_POSITION_RATE_40HZ,                      /* 40Hz�����ڲ����������921600bps */
    ATK_MO1218_POSITION_RATE_50HZ,                      /* 50Hz�����ڲ����������921600bps */
} atk_mo1218_position_rate_t;

/* ATK-MO1218ģ��DOPģʽö�� */
typedef enum
{
    ATK_MO1218_DOP_MODE_DISABLE = 0x00,                 /* �ر� */
    ATK_MO1218_DOP_MODE_AUTO,                           /* �Զ� */
    ATK_MO1218_DOP_MODE_PDOP,                           /* ��PDOP */
    ATK_MO1218_DOP_MODE_HDOP,                           /* ��HDOP */
    ATK_MO1218_DOP_MODE_GDOP,                           /* ��GDOP */
} atk_mo1218_dop_mode_t;

/* ATK-MO1218ģ��Elevation��CNRģʽö�� */
typedef enum
{
    ATK_MO1218_ELEVATION_CNR_MODE_DISABLE = 0x00,       /* �ر� */
    ATK_MO1218_ELEVATION_CNR_MODE_BOTH,                 /* Elevation��CNR */
    ATK_MO1218_ELEVATION_CNR_MODE_ELEVATION,            /* ��Elevation */
    ATK_MO1218_ELEVATION_CNR_MODE_CNR,                  /* ��CNR */
} atk_mo1218_elevation_cnr_mode_t;

/* ATK-MO1218ģ��Position Pinning״̬ö�� */
typedef enum
{
    ATK_MO1218_POSITION_PINNING_DEFAULT = 0x00,         /* Ĭ�� */
    ATK_MO1218_POSITION_PINNING_ENABLE,                 /* ʹ�� */
    ATK_MO1218_POSITION_PINNING_DISABLE,                /* �ر� */
} atk_mo1218_position_pinning_t;

/* ATK-MO1218ģ��SBASʹ��״̬ö�� */
typedef enum
{
    ATK_MO1218_SBAS_DISABLE = 0x00,                     /* �ر� */
    ATK_MO1218_SBAS_ENABLE,                             /* ʹ�� */
} atk_mo1218_sbas_enable_t;

/* ATK-MO1218ģ��SBAS���ڵ���ʹ��״̬ö�� */
typedef enum
{
    ATK_MO1218_SBAS_NAV_DISABLE = 0x00,                 /* �ر� */
    ATK_MO1218_SBAS_NAV_ENABLE,                         /* ʹ�� */
} atk_mo1218_sbas_nav_enable_t;

/* ATK-MO1218ģ��SBAS Correctionʹ��״̬ö�� */
typedef enum
{
    ATK_MO1218_SBAS_CORRECTION_DISABLE = 0x00,          /* �ر� */
    ATK_MO1218_SBAS_CORRECTION_ENABLE,                  /* ʹ�� */
} atk_mo1218_sbas_correction_enable_t;

/* ATK-MO1218ģ��SBAS WAASʹ��״̬ö�� */
typedef enum
{
    ATK_MO1218_SBAS_WAAS_DISABLE = 0x00,                /* �ر� */
    ATK_MO1218_SBAS_WAAS_ENABLE,                        /* ʹ�� */
} atk_mo1218_sbas_waas_enable_t;

/* ATK-MO1218ģ��SBAS EGNOSʹ��״̬ö�� */
typedef enum
{
    ATK_MO1218_SBAS_EGNOS_DISABLE = 0x00,               /* �ر� */
    ATK_MO1218_SBAS_EGNOS_ENABLE,                       /* ʹ�� */
} atk_mo1218_sbas_egnos_enable_t;

/* ATK-MO1218ģ��SBAS MSASʹ��״̬ö�� */
typedef enum
{
    ATK_MO1218_SBAS_MSAS_DISABLE = 0x00,                /* �ر� */
    ATK_MO1218_SBAS_MSAS_ENABLE,                        /* ʹ�� */
} atk_mo1218_sbas_msas_enable_t;

/* ATK-MO1218ģ��QZSSʹ��״̬ö�� */
typedef enum
{
    ATK_MO1218_QZSS_DISABLE = 0x00,                     /* �ر� */
    ATK_MO1218_QZSS_ENABLE,                             /* ʹ�� */
} atk_mo1218_qzss_enable_t;

/* ATK-MO1218ģ��SAEEʹ��״̬ö�� */
typedef enum
{
    ATK_MO1218_SAEE_DEFAULT = 0x00,                     /* Ĭ�� */
    ATK_MO1218_SAEE_ENABLE,                             /* ʹ�� */
    ATK_MO1218_SAEE_DISABLE,                            /* �ر� */
} atk_mo1218_saee_enable_t;

/* ATK-MO1218ģ����������ö�� */
typedef enum
{
    ATK_MO1218_BOOT_FROM_FLASH_OK = 0x00,               /* ��Flash�����ɹ� */
    ATK_MO1218_BOOT_FROM_ROM,                           /* ��ROM��������Ϊ��Flash����ʧ�� */
} atk_mo1218_boot_fail_t;

/* ATK-MO1218ģ����ż��״̬ö�� */
typedef enum
{
    ATK_MO1218_INTERENCE_DETECTION_DISABLE = 0x00,      /* �ر� */
    ATK_MO1218_INTERENCE_DETECTION_ENABLE,              /* ʹ�� */
} atk_mo1218_interence_detection_enable_t;

/* ATK-MO1218ģ�����״̬ö�� */
typedef enum
{
    ATK_MO1218_INTERENCE_UNKNOWN = 0x00,                /* δ֪ */
    ATK_MO1218_INTERENCE_NO,                            /* �޸��� */
    ATK_MO1218_INTERENCE_LITE,                          /* ��΢���� */
    ATK_MO1218_INTERENCE_CRITICAL,                      /* ���ظ��� */
} atk_mo1218_interence_status_t;

/* ATK-MO1218ģ�鵼��ģʽö�� */
typedef enum
{
    ATK_MO1218_NAVIGATION_MODE_AUTO = 0x00,             /* �Զ�ģʽ */
    ATK_MO1218_NAVIGATION_MODE_PRDESTRIAN,              /* ����ģʽ */
    ATK_MO1218_NAVIGATION_MODE_CAR,                     /* ����ģʽ */
    ATK_MO1218_NAVIGATION_MODE_MARINE,                  /* ����ģʽ */
    ATK_MO1218_NAVIGATION_MODE_BALLOON,                 /* ����ģʽ */
    ATK_MO1218_NAVIGATION_MODE_AIRBORNE,                /* ����ģʽ */
} atk_mo1218_navigation_mode_t;

/* ATK-MO1218ģ��GNSS����ö�� */
typedef enum
{
    ATK_MO1218_GNSS_GPS = 0x00,                         /* GPS */
    ATK_MO1218_GNSS_BEIDOU,                             /* ���� */
    ATK_MO1218_GNSS_GPS_BEIDOU,                         /* GPS�ͱ��� */
} atk_mo1218_gnss_for_navigation_t;

/* ATK-MO1218����汾�ṹ�嶨�� */
typedef struct
{
    struct
    {
        uint8_t x1;
        uint8_t y1;
        uint8_t z1;
    } kernel;                                           /* Kernel Version: x1.y1.z1 */
    struct
    {
        uint8_t x1;
        uint8_t y1;
        uint8_t z1;
    } odm;                                              /* ODM Version: x1.y1.z1 */
    struct
    {
        uint8_t yy;
        uint8_t mm;
        uint8_t dd;
    } revision;                                         /* Revision: (2000+yy).mm.dd */
} atk_mo1218_sw_version_t;

/* ATK-MO1218ģ��GPS�������ݽṹ�� */
typedef struct
{
    uint16_t sv_id;                                     /* ����ID */
    uint8_t subframe0[28];                              /* ����������֡0 */
    uint8_t subframe1[28];                              /* ����������֡1 */
    uint8_t subframe2[28];                              /* ����������֡2 */
} atk_mo1218_gps_ephemeris_data_t;

/* ATK-MO1218ģ��Position pinning�����ṹ�� */
typedef struct
{
    uint16_t pinning_speed;                             /* ��λ������/ʱ */
    uint16_t pinning_cnt;                               /* ��λ���� */
    uint16_t unpinning_speed;                           /* ��λ������/ʱ */
    uint16_t unpinning_cnt;                             /* ��λ���� */
    uint16_t unpinning_distance;                        /* ��λ���� */
} atk_mo1218_position_pinning_parameter_t;

/* ATK-MO1218ģ��Position pinning״̬���ݽṹ�� */
typedef struct
{
    atk_mo1218_position_pinning_t status;               /* ʹ��λ */
    atk_mo1218_position_pinning_parameter_t parameter;  /* ���� */
} atk_mo1218_position_pinning_status_t;

/* ATK-MO1218ģ��SBAS�����ṹ�� */
typedef struct
{
    atk_mo1218_sbas_enable_t enable;                    /* SBASʹ��λ */
    atk_mo1218_sbas_nav_enable_t navigation;            /* ����Navigationʹ��λ */
    uint8_t ranging_ura_mask;                           /* UAR���� */
    atk_mo1218_sbas_correction_enable_t correction;     /* Correctionʹ��λ */
    uint8_t num_tracking_channel;                       /* ͨ������ */
    atk_mo1218_sbas_waas_enable_t waas;                 /* WAASʹ��λ */
    atk_mo1218_sbas_egnos_enable_t egnos;               /* EGNOSʹ��λ */
    atk_mo1218_sbas_msas_enable_t msas;                 /* MSAMʹ��λ */
} atk_mo1218_sbas_parameter_t;

/* ATK-MO1218ģ��QZSS�����ṹ�� */
typedef struct
{
    atk_mo1218_qzss_enable_t enable;                    /* QZSSʹ��λ */
    uint8_t num_tracking_channel;                       /* ͨ������ */
} atk_mo1218_qzss_parameter_t;

/* ATK-MO1218ģ��SAEE�����ṹ�� */
typedef struct
{
    atk_mo1218_saee_enable_t enable;                    /* SAEEʹ��λ */
} atk_mo1218_saee_parameter_t;

/* ATK-MO1218ģ������״̬��Ϣ�ṹ�� */
typedef struct
{
    atk_mo1218_boot_fail_t fail_over;                   /* �������� */
} atk_mo1218_boot_status_t;

/* ATK-MO1218ģ����ż��״̬�ṹ�� */
typedef struct
{
    atk_mo1218_interence_detection_enable_t enable;     /* ���ż��ʹ��״̬ */
    atk_mo1218_interence_status_t interence_status;     /* ���ż���� */
} atk_mo1218_interence_detection_status_t;

/* �������� */
uint8_t atk_mo1218_send_bin_msg(uint8_t *playload, uint16_t pl, uint16_t timeout);                                                                                                                                                      /* ��ATK-MO1218����Binary Message */
uint8_t atk_mo1218_restart(atk_mo1218_restart_t restart);                                                                                                                                                                               /* ATK-MO1218ģ��ϵͳ���� */
uint8_t atk_mo1218_get_sw_version(atk_mo1218_sw_version_t *version);                                                                                                                                                                    /* ��ȡATK-MO1218ģ������汾 */
uint8_t atk_mo1218_get_sw_crc(uint16_t *crc);                                                                                                                                                                                           /* ��ȡATK-MO1218ģ�����CRCֵ */
uint8_t atk_mo1218_factory_reset(atk_mo1218_factory_reset_type_t type);                                                                                                                                                                 /* ATK-MO1218ģ��ָ��������� */
uint8_t atk_mo1218_config_serial(atk_mo1218_serial_baudrate_t baudrate, atk_mo1218_save_type_t save_type);                                                                                                                              /* ����ATK-MO1218ģ�鴮�� */
uint8_t atk_mo1218_config_nmea_msg(uint8_t gga, uint8_t gsa, uint8_t gsv, uint8_t gll, uint8_t rmc, uint8_t vtg, uint8_t zda, atk_mo1218_save_type_t save_type);                                                                        /* ����ATK-MO1218ģ��NMEA�����Ϣ��� */
uint8_t atk_mo1218_config_output_type(atk_mo1218_output_type_t output_type, atk_mo1218_save_type_t save_type);                                                                                                                          /* ����ATK-MO1218ģ�������Ϣ���� */
uint8_t atk_mo1218_config_power_mode(atk_mo1218_power_mode_t mode, atk_mo1218_save_type_t save_type);                                                                                                                                   /* ����ATK-MO1218ģ���Դģʽ */
uint8_t atk_mo1218_config_position_rate(atk_mo1218_position_rate_t rate, atk_mo1218_save_type_t save_type);                                                                                                                             /* ����ATK-MO1218ģ��λ�ø���Ƶ�� */
uint8_t atk_mo1218_get_position_rate(atk_mo1218_position_rate_t *rate);                                                                                                                                                                 /* ��ȡATK-MO1218ģ��λ�ø���Ƶ�� */
uint8_t atk_mo1218_config_navigation_interval(uint8_t interval, atk_mo1218_save_type_t save_type);                                                                                                                                      /* ����ATK-MO1218ģ�鵼��������Ϣ��� */
uint8_t atk_mo1218_get_power_mode(atk_mo1218_power_mode_t *mode);                                                                                                                                                                       /* ��ȡATK-MO1218ģ���Դģʽ */
uint8_t atk_mo1218_config_dop_mask(atk_mo1218_dop_mode_t mode, uint16_t pdop_val, uint16_t hdop_val, uint16_t gdop_val, atk_mo1218_save_type_t save_type);                                                                              /* ����ATK-MO1218ģ��DOP���� */
uint8_t atk_mo1218_config_evelation_cnr_mask(atk_mo1218_elevation_cnr_mode_t mode, uint8_t elevation_mask, uint8_t cnr_mask, atk_mo1218_save_type_t save_type);                                                                         /* ����ATK-MO1218ģ��Elevation��CNR���� */
uint8_t atk_mo1218_get_datum(uint16_t *datum_index);                                                                                                                                                                                    /* ��ȡATK-MO1218ģ��Datum */
uint8_t atk_mo1218_get_dop_mask(atk_mo1218_dop_mode_t *mode, uint16_t *pdop_val, uint16_t *hdop_val, uint16_t *gdop_val);                                                                                                               /* ��ȡATK-MO1218ģ��DOP���� */
uint8_t atk_mo1218_get_evelation_cnr_mask(atk_mo1218_elevation_cnr_mode_t *mode, uint8_t *elevation_mask, uint8_t *cnr_mask);                                                                                                           /* ��ȡATK-MO1218ģ��Elevation��CNR���� */
uint8_t atk_mo1218_get_gps_ephemeris(uint8_t sv, atk_mo1218_gps_ephemeris_data_t *data);                                                                                                                                                /* ��ȡATK-MO1218ģ��GPS�������� */
uint8_t atk_mo1218_config_position_pinning(atk_mo1218_position_pinning_t status, atk_mo1218_save_type_t save_type);                                                                                                                     /* ����ATK-MO1218ģ��Position Pinning */
uint8_t atk_mo1218_get_position_pinning_status(atk_mo1218_position_pinning_status_t *status);                                                                                                                                           /* ��ȡATK-MO1218ģ��Position Pinning״̬ */
uint8_t atk_mo1218_config_position_pinning_parameters(atk_mo1218_position_pinning_parameter_t *parameter, atk_mo1218_save_type_t save_type);                                                                                            /* ����ATK-MO1218ģ��Position Pinning���� */
uint8_t atk_mo1218_set_gps_ephemeris(atk_mo1218_gps_ephemeris_data_t *data);                                                                                                                                                            /* ����ATK-MO1218ģ��GPS�������� */
uint8_t atk_mo1218_config_1pps_cable_delay(int32_t cable_delay, atk_mo1218_save_type_t save_type);                                                                                                                                      /* ����ATK-MO1218ģ��1PPS�ĵ�����ʱ */
uint8_t atk_mo1218_get_1pps_cable_delay(int32_t *cable_delay);                                                                                                                                                                          /* ��ȡATK-MO1218ģ��1PPS�ĵ�����ʱ */
uint8_t atk_mo1218_config_sbas(atk_mo1218_sbas_parameter_t *parameter, atk_mo1218_save_type_t save_type);                                                                                                                               /* ����ATK-MO1218ģ��SBAS */
uint8_t atk_mo1218_get_sbas_status(atk_mo1218_sbas_parameter_t *parameter);                                                                                                                                                             /* ��ȡATK-MO1218ģ��SBAS���ò��� */
uint8_t atk_mo1218_config_qzss(atk_mo1218_qzss_parameter_t *parameter, atk_mo1218_save_type_t save_type);                                                                                                                               /* ����ATK-MO1218ģ��QZSS */
uint8_t atk_mo1218_get_qzss_status(atk_mo1218_qzss_parameter_t *parameter);                                                                                                                                                             /* ��ȡATK-MO1218ģ��QZSS���ò��� */
uint8_t atk_mo1218_config_saee(atk_mo1218_saee_parameter_t *parameter, atk_mo1218_save_type_t save_type);                                                                                                                               /* ����ATK-MO1218ģ��SAEE */
uint8_t atk_mo1218_get_saee_status(atk_mo1218_saee_parameter_t *parameter);                                                                                                                                                             /* ��ȡATK-MO1218ģ��SAEE���ò��� */
uint8_t atk_mo1218_get_boot_status(atk_mo1218_boot_status_t *status);                                                                                                                                                                   /* ��ȡATK-MO1218ģ������״̬��Ϣ */
uint8_t atk_mo1218_config_ext_nmea_msg(uint8_t gga, uint8_t gsa, uint8_t gsv, uint8_t gll, uint8_t rmc, uint8_t vtg, uint8_t zda, uint8_t gns, uint8_t gbs, uint8_t grs, uint8_t dtm, uint8_t gst, atk_mo1218_save_type_t save_type);   /* ����ATK-MO1218ģ����չNMEA�����Ϣ��� */
uint8_t atk_mo1218_get_ext_nmea_msg(uint8_t *gga, uint8_t *gsa, uint8_t *gsv, uint8_t *gll, uint8_t *rmc, uint8_t *vtg, uint8_t *zda, uint8_t *gns, uint8_t *gbs, uint8_t *grs, uint8_t *dtm, uint8_t *gst);                            /* ��ȡATK-MO1218ģ����չNMEA�����Ϣ��� */
uint8_t atk_mo1218_config_interference_detection(atk_mo1218_interence_detection_enable_t enable, atk_mo1218_save_type_t save_type);                                                                                                     /* ����ATK-MO1218ģ����ż�� */
uint8_t atk_mo1218_get_interence_detection_status(atk_mo1218_interence_detection_status_t *status);                                                                                                                                     /* ��ȡATK-MO1218ģ����ż��״̬ */
uint8_t atk_mo1218_config_navigation_mode(atk_mo1218_navigation_mode_t mode, atk_mo1218_save_type_t save_type);                                                                                                                         /* ����ATK-MO1218ģ�鵼��ģʽ */
uint8_t atk_mo1218_get_navigation_mode(atk_mo1218_navigation_mode_t *mode);                                                                                                                                                             /* ��ȡATK-MO1218ģ�鵼��ģʽ */
uint8_t atk_mo1218_config_gnss_for_navigation(atk_mo1218_gnss_for_navigation_t gnss, atk_mo1218_save_type_t save_type);                                                                                                                 /* ����ATK-MO1218ģ�����ڵ�����GNSS */
uint8_t atk_mo1218_get_gnss_for_navigation(atk_mo1218_gnss_for_navigation_t *gnss);                                                                                                                                                     /* ��ȡATK-MO1218ģ�����ڵ�����GNSS */
uint8_t atk_mo1218_config_1pps_pulse_width(uint32_t pulse_width, atk_mo1218_save_type_t save_type);                                                                                                                                     /* ����ATK-MO1218ģ��1PPS�������� */
uint8_t atk_mo1218_get_1pps_pulse_width(uint32_t *pulse_width);                                                                                                                                                                         /* ��ȡATK-MO1218ģ��1PPS�������� */

#endif
