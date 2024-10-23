/**
 ****************************************************************************************************
 * @file        atk_mo1218.c
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

#include "atk_mo1218.h"
#include "usart.h"

/**
 * @brief       ATK-MO1218��ʼ��
 * @param       baudrate: ATK-MO1218 UARTͨѶ������
 * @retval      ATK_MO1218_EOK  : ATK-MO1218��ʼ���ɹ�
 *              ATK_MO1218_ERROR: ATK-MO1218��ʼ��ʧ��
 */
uint8_t atk_mo1218_init(uint32_t baudrate)
{
    uint8_t ret;
    atk_mo1218_sw_version_t version;

    ret = atk_mo1218_get_sw_version(&version);
    if (ret != ATK_MO1218_EOK)
    {
        return ATK_MO1218_ERROR;
    }
    
    return ATK_MO1218_EOK;
}

/**
 * @brief       ��ȡ������ATK-MO1218ģ������
 * @param       utc                  : UTCʱ��
 *              position             : λ����Ϣ����γ������100000����
 *              altitude             : ���θ߶ȣ�����10��������λ����
 *              speed                : �����ٶȣ�����10��������λ��ǧ��/ʱ
 *              fix_info             : ��λ��Ϣ
 *              gps_satellite_info   : �ɼ�GPS������Ϣ
 *              beidou_satellite_info: �ɼ�����������Ϣ
 *              timeout              : �ȴ���ʱʱ�䣬��λ��1����
 * @retval      ATK_MO1218_EOK     : ��ȡ������ATK-MO1218ģ�����ݳɹ�
 *              ATK_MO1218_EINVAL  : ������������
 *              ATK_MO1218_ETIMEOUT: �ȴ���ʱ
 */
uint8_t atk_mo1218_update(atk_mo1218_time_t *utc, atk_mo1218_position_t *position, int16_t *altitude, uint16_t *speed, atk_mo1218_fix_info_t *fix_info, atk_mo1218_visible_satellite_info_t *gps_satellite_info, atk_mo1218_visible_satellite_info_t *beidou_satellite_info, uint32_t timeout)
{
    uint8_t ret;
    uint8_t *buf;
    uint8_t *nmea;
    struct
    {
        atk_mo1218_nmea_gga_msg_t msg;
        uint8_t done;
    } gngga;
    struct
    {
        atk_mo1218_nmea_gsa_msg_t msg;
        uint8_t done;
    } gngsa;
    struct
    {
        atk_mo1218_nmea_gsv_msg_t msg;
        uint8_t done;
    } gpgsv;
    struct
    {
        atk_mo1218_nmea_gsv_msg_t msg;
        uint8_t done;
    } bdgsv;
    struct
    {
        atk_mo1218_nmea_rmc_msg_t msg;
        uint8_t done;
    } gnrmc;
    struct
    {
        atk_mo1218_nmea_vtg_msg_t msg;
        uint8_t done;
    } gnvtg;
    uint8_t satellite_index;
    
    if ((utc == NULL) && (position == NULL) && (altitude == NULL) && (speed == NULL) && (fix_info == NULL) && (gps_satellite_info == NULL) && (beidou_satellite_info == NULL))
    {
        return ATK_MO1218_EINVAL;
    }
    
    gngga.done = 0;
    gngsa.done = 0;
    gpgsv.done = 0;
    bdgsv.done = 0;
    gnrmc.done = 0;
    gnvtg.done = 0;
    
    atk_mo1218_uart_rx_restart();
    while (timeout > 0)
    {
        buf = atk_mo1218_uart_rx_get_frame();
        if (buf != NULL)
        {
            /* GNGGA */
            if (((altitude != NULL) || (fix_info != NULL)) && (gngga.done == 0))
            {
                ret = atk_mo1218_get_nmea_msg_from_buf(buf, ATK_MO1218_NMEA_MSG_GNGGA, 0, &nmea);
                if (ret == ATK_MO1218_EOK)
                {
                    ret = atk_mo1218_decode_nmea_xxgga(nmea, &gngga.msg);
                    if (ret == ATK_MO1218_EOK)
                    {
                        gngga.done = ~0;
                        if (altitude != NULL)
                        {
                            *altitude = gngga.msg.altitude;
                        }
                        if (fix_info != NULL)
                        {
                            fix_info->quality = gngga.msg.gps_quality;
                            fix_info->satellite_num = gngga.msg.satellite_num;
                        }
                    }
                }
            }
            else
            {
                gngga.done = ~0;
            }
            
            /* GNGSA */
            if ((fix_info != NULL) && (gngsa.done == 0))
            {
                ret = atk_mo1218_get_nmea_msg_from_buf(buf, ATK_MO1218_NMEA_MSG_GNGSA, 0, &nmea);
                if (ret == ATK_MO1218_EOK)
                {
                    ret = atk_mo1218_decode_nmea_xxgsa(nmea, &gngsa.msg);
                    if (ret == ATK_MO1218_EOK)
                    {
                        gngsa.done = ~0;
                        fix_info->type = gngsa.msg.type;
                        for (satellite_index=0; satellite_index<12; satellite_index++)
                        {
                            fix_info->satellite_id[satellite_index] = gngsa.msg.satellite_id[satellite_index];
                        }
                        fix_info->pdop = gngsa.msg.pdop;
                        fix_info->hdop = gngsa.msg.hdop;
                        fix_info->vdop = gngsa.msg.vdop;
                    }
                }
            }
            else
            {
                gngsa.done = ~0;
            }
            
            /* GPGSV */
            if ((gps_satellite_info != NULL) && (gpgsv.done == 0))
            {
                ret = atk_mo1218_get_nmea_msg_from_buf(buf, ATK_MO1218_NMEA_MSG_GPGSV, 0, &nmea);
                if (ret == ATK_MO1218_EOK)
                {
                    ret = atk_mo1218_decode_nmea_xxgsv(nmea, &gpgsv.msg);
                    if (ret == ATK_MO1218_EOK)
                    {
                        gpgsv.done = ~0;
                        gps_satellite_info->satellite_num = gpgsv.msg.satellite_view;
                        for (satellite_index=0; satellite_index<gpgsv.msg.satellite_view; satellite_index++)
                        {
                            gps_satellite_info->satellite_info[satellite_index].satellite_id = gpgsv.msg.satellite_info[satellite_index].satellite_id;
                            gps_satellite_info->satellite_info[satellite_index].elevation = gpgsv.msg.satellite_info[satellite_index].elevation;
                            gps_satellite_info->satellite_info[satellite_index].azimuth = gpgsv.msg.satellite_info[satellite_index].azimuth;
                            gps_satellite_info->satellite_info[satellite_index].snr = gpgsv.msg.satellite_info[satellite_index].snr;
                        }
                    }
                }
            }
            else
            {
                gpgsv.done = ~0;
            }
            
            /* BDGSV */
            if ((beidou_satellite_info != NULL) && (bdgsv.done == 0))
            {
                ret = atk_mo1218_get_nmea_msg_from_buf(buf, ATK_MO1218_NMEA_MSG_BDGSV, 0, &nmea);
                if (ret == ATK_MO1218_EOK)
                {
                    ret = atk_mo1218_decode_nmea_xxgsv(nmea, &bdgsv.msg);
                    if (ret == ATK_MO1218_EOK)
                    {
                        bdgsv.done = ~0;
                        beidou_satellite_info->satellite_num = bdgsv.msg.satellite_view;
                        for (satellite_index=0; satellite_index<bdgsv.msg.satellite_view; satellite_index++)
                        {
                            beidou_satellite_info->satellite_info[satellite_index].satellite_id = bdgsv.msg.satellite_info[satellite_index].satellite_id;
                            beidou_satellite_info->satellite_info[satellite_index].elevation = bdgsv.msg.satellite_info[satellite_index].elevation;
                            beidou_satellite_info->satellite_info[satellite_index].azimuth = bdgsv.msg.satellite_info[satellite_index].azimuth;
                            beidou_satellite_info->satellite_info[satellite_index].snr = bdgsv.msg.satellite_info[satellite_index].snr;
                        }
                    }
                }
            }
            else
            {
                bdgsv.done = ~0;
            }
            
            /* GNRMC */
            if (((utc != NULL) || (position != NULL)) && (gnrmc.done == 0))
            {
                ret = atk_mo1218_get_nmea_msg_from_buf(buf, ATK_MO1218_NMEA_MSG_GNRMC, 0, &nmea);
                if (ret == ATK_MO1218_EOK)
                {
                    ret = atk_mo1218_decode_nmea_xxrmc(nmea, &gnrmc.msg);
                    if (ret == ATK_MO1218_EOK)
                    {
                        gnrmc.done = ~0;
                        if (utc != NULL)
                        {
                            utc->year = gnrmc.msg.utc_date.year;
                            utc->month = gnrmc.msg.utc_date.month;
                            utc->day = gnrmc.msg.utc_date.day;
                            utc->hour = gnrmc.msg.utc_time.hour;
                            utc->minute = gnrmc.msg.utc_time.minute;
                            utc->second = gnrmc.msg.utc_time.second;
                            utc->millisecond = gnrmc.msg.utc_time.millisecond;
                        }
                        if (position != NULL)
                        {
                            position->latitude.degree = gnrmc.msg.latitude.degree;
                            position->latitude.indicator = gnrmc.msg.latitude.indicator;
                            position->longitude.degree = gnrmc.msg.longitude.degree;
                            position->longitude.indicator = gnrmc.msg.longitude.indicator;
                        }
                    }
                }
            }
            else
            {
                gnrmc.done = ~0;
            }
            
            /* GNVTG */
            if ((speed != NULL) && (gnvtg.done == 0))
            {
                ret = atk_mo1218_get_nmea_msg_from_buf(buf, ATK_MO1218_NMEA_MSG_GNVTG, 0, &nmea);
                if (ret == ATK_MO1218_EOK)
                {
                    gnvtg.done = ~0;
                    ret = atk_mo1218_decode_nmea_xxvtg(nmea, &gnvtg.msg);
                    if (ret == ATK_MO1218_EOK)
                    {
                        *speed = gnvtg.msg.speed_kph;
                    }
                }
            }
            else
            {
                gnvtg.done = ~0;
            }
        }
        
        if ((gngga.done != 0) && (gngsa.done != 0) && (gpgsv.done != 0) && (bdgsv.done != 0) && (gnrmc.done != 0) && (gnvtg.done != 0))
        {
            return ATK_MO1218_EOK;
        }
        
        timeout--;
        HAL_Delay(1);
    }
    
    return ATK_MO1218_ETIMEOUT;
}
