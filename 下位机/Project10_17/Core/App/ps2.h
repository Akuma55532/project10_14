#ifndef PS2_H
#define PS2_H

#include "main.h"
#include "gpio.h"
#include <stdio.h>


extern uint8_t psx_buf[9];
extern uint8_t ps2_flag;


#define START_CMD				0x01
#define ASK_DAT_CMD			0x42

#define PS2_LED_RED  		0x73		//PS2手柄红灯模式
#define PS2_LED_GRN  		0x41		//PS2手柄绿灯模式
#define PSX_BUTTON_NUM 	16			//手柄按键数目
#define PS2_MAX_LEN 		64			//手柄命令最大字节数

#define PS2_DAT	    HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)
#define PS2_CMD(x) {x? HAL_GPIO_WritePin(GPIOA,GPIO_PIN_14,GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOA,GPIO_PIN_14,GPIO_PIN_RESET);}
#define PS2_ATT(x) {x? HAL_GPIO_WritePin(GPIOA,GPIO_PIN_13,GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOA,GPIO_PIN_13,GPIO_PIN_RESET);}
#define PS2_CLK(x) {x? HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);}

void PSX_init(void);
unsigned char psx_transfer(unsigned char dat);
void psx_write_read(unsigned char *get_buf);
void loop_ps2_button(void);
void parse_psx_buf(unsigned char *buf, unsigned char mode);
void loop_ps2_car(void);
#endif
