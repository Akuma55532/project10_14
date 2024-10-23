#include "servo.h"
#include <math.h>

#include <stdio.h>
#include <math.h>
#include <string.h>
#define M_PI 3.1416

#define LINK12 159.0    // 单位为毫米
#define LINK23 105.0
#define LINK34 75.0
#define DIST_EE 185.0   // 末端执行器与第四个关节之间的距离

#define PI 3.1415926

uint8_t servo_move_flag;

uint8_t arm_x,arm_y,arm_z;

int joint[4];




const char *action_group_x[14] = {//红灯模式下按键的配置
	"#001P0700T1000!#002P1100T1000!#003P1300T1000!\r\n",
	"#001P0800T1000!#002P1200T1000!#003P1300T1000!\r\n",
	"#001P0900T1000!#002P1300T1000!#003P1300T1000!\r\n",
	"#001P1000T1000!#002P1400T1000!#003P1300T1000!\r\n",
	"#001P1100T1000!#002P1500T1000!#003P1300T1000!\r\n",
	"#001P1200T1000!#002P1600T1000!#003P1300T1000!\r\n",
	"#001P1300T1000!#002P1700T1000!#003P1300T1000!\r\n",
	"#001P1400T1000!#002P1800T1000!#003P1300T1000!\r\n",
	"#001P1500T1000!#002P1900T1000!#003P1300T1000!\r\n",
	"#001P1600T1000!#002P2000T1000!#003P1300T1000!\r\n",
	"#001P1700T1000!#002P2100T1000!#003P1300T1000!\r\n",
	"#001P1800T1000!#002P2200T1000!#003P1300T1000!\r\n",
	"#001P1900T1000!#002P2300T1000!#003P1300T1000!\r\n",
	"#001P2000T1000!#002P2400T1000!#003P1300T1000!\r\n",	//--13					  				    				  							
};

const char *action_group_y[9] = {
	"#000P0700T1000!\r\n",
	"#000P0900T1000!\r\n",
	"#000P1100T1000!\r\n",
	"#000P1300T1000!\r\n",
	"#000P1500T1000!\r\n",
	"#000P1700T1000!\r\n",
	"#000P1900T1000!\r\n",
	"#000P2100T1000!\r\n",
	"#000P2300T1000!\r\n", //--8
}; 

void Servo_Init(void)
{
	sprintf((char *)TX3_BUFFER, "#000P1500T1000!#001P1800T1000!#002P2200T1000!#003P1100T1000!#005P1500T1000!\r\n");
	HAL_HalfDuplex_EnableTransmitter(&huart3);
	HAL_UART_Transmit(&huart3,TX3_BUFFER,sizeof(TX3_BUFFER),0xffff);
	HAL_HalfDuplex_EnableReceiver(&huart3);
	arm_x = 11;
	arm_y = 4;
}



void action_arm2(void)
{
	static uint8_t arm_x_last = 11;
	static uint8_t arm_y_last = 4;
	if(arm_x != arm_x_last)
	{
		switch(arm_x)
		{
			case 0:
				sprintf((char *)TX3_BUFFER, "#001P0700T1000!#002P1100T1000!\r\n");
				break;
			case 1:
				sprintf((char *)TX3_BUFFER, "#001P0800T1000!#002P1200T1000!\r\n");
				break;
			case 2:
				sprintf((char *)TX3_BUFFER, "#001P0900T1000!#002P1300T1000!\r\n");
				break;
			case 3:
				sprintf((char *)TX3_BUFFER, "#001P1000T1000!#002P1400T1000!\r\n");
				break;
			case 4:
				sprintf((char *)TX3_BUFFER, "#001P1100T1000!#002P1500T1000!\r\n");
				break;
			case 5:
				sprintf((char *)TX3_BUFFER, "#001P1200T1000!#002P1600T1000!\r\n");
				break;
			case 6:
				sprintf((char *)TX3_BUFFER, "#001P1300T1000!#002P1700T1000!\r\n");
				break;
			case 7:
				sprintf((char *)TX3_BUFFER, "#001P1400T1000!#002P1800T1000!\r\n");
				break;
			case 8:
				sprintf((char *)TX3_BUFFER, "#001P1500T1000!#002P1900T1000!\r\n");
				break;
			case 9:
				sprintf((char *)TX3_BUFFER, "#001P1600T1000!#002P2000T1000!\r\n");
				break;
			case 10:
				sprintf((char *)TX3_BUFFER, "#001P1700T1000!#002P2100T1000!#003P1200T1000!\r\n");
				break;
			case 11:
				sprintf((char *)TX3_BUFFER, "#001P1800T1000!#002P2200T1000!#003P1100T1000!\r\n");
				break;
			case 12:
				sprintf((char *)TX3_BUFFER, "#001P1900T1000!#002P2300T1000!\r\n");
				break;
			case 13:
				sprintf((char *)TX3_BUFFER, "#001P2000T1000!#002P2400T1000!\r\n");
				break;
		}
		HAL_HalfDuplex_EnableTransmitter(&huart3);
		HAL_UART_Transmit(&huart3,TX3_BUFFER,sizeof(TX3_BUFFER),0xffff);
		HAL_HalfDuplex_EnableReceiver(&huart3);
	}
	if(arm_y != arm_y_last)
	{
		switch(arm_y)
		{
			case 0:sprintf((char *)TX3_BUFFER, "#000P0700T1000!\r\n");
				break;
			case 1:sprintf((char *)TX3_BUFFER, "#000P0900T1000!\r\n");
				break;
			case 2:sprintf((char *)TX3_BUFFER, "#000P1100T1000!\r\n");
				break;
			case 3:sprintf((char *)TX3_BUFFER, "#000P1300T1000!\r\n");
				break;
			case 4:sprintf((char *)TX3_BUFFER, "#000P1500T1000!\r\n");
				break;
			case 5:sprintf((char *)TX3_BUFFER, "#000P1700T1000!\r\n");
				break;
			case 6:sprintf((char *)TX3_BUFFER, "#000P1900T1000!\r\n");
				break;
			case 7:sprintf((char *)TX3_BUFFER, "#000P2100T1000!\r\n");
				break;
			case 8:sprintf((char *)TX3_BUFFER, "#000P2300T1000!\r\n");
				break;
		}
		HAL_HalfDuplex_EnableTransmitter(&huart3);
		HAL_UART_Transmit(&huart3,TX3_BUFFER,sizeof(TX3_BUFFER),0xffff);
		HAL_HalfDuplex_EnableReceiver(&huart3);
	}
	arm_x_last = arm_x;
	arm_y_last = arm_y;
	
}



void action_do_x(uint8_t direct)
{
	if(direct == 0)//up
	{
		joint[1] += 100;
		joint[2] -= 100;
		if(joint[1] > 2500)
		{
			joint[1] = 2500;
		}
		if(joint[2] < 500)
		{
			joint[2] = 500;
		}
	}
	else //down
	{
		joint[1] -= 100;
		joint[2] += 100;
		if(joint[1] < 500)
		{
			joint[1] = 500;
		}
		if(joint[2] > 2500)
		{
			joint[2] = 2500;
		}
	}
}

void action_do_y(uint8_t direct)
{
	if(direct == 0)//up
	{
		joint[0] += 200;
		if(joint[0] > 2500)
		{
			joint[0] = 2500;
		}
	}
	else //down
	{
		joint[0] -= 200;
		if(joint[0] < 500)
		{
			joint[0] = 500;
		}
	}
}

















void test1_servo(void)
{
	sprintf((char *)TX3_BUFFER,"#000P2000T1000!\r\n");
	HAL_HalfDuplex_EnableTransmitter(&huart3);
	HAL_UART_Transmit(&huart3,TX3_BUFFER,sizeof(TX3_BUFFER),0xffff);
	HAL_HalfDuplex_EnableReceiver(&huart3);
}
void test2_servo(void)
{
	sprintf((char *)TX3_BUFFER,"#000P1500T1000!\r\n");
	HAL_HalfDuplex_EnableTransmitter(&huart3);
	HAL_UART_Transmit(&huart3,TX3_BUFFER,sizeof(TX3_BUFFER),0xffff);
	HAL_HalfDuplex_EnableReceiver(&huart3);
}
void test3_servo(void)
{
	sprintf((char *)TX3_BUFFER,"#000P2000T1000!\r\n");
	HAL_HalfDuplex_EnableTransmitter(&huart3);
	HAL_UART_Transmit(&huart3,TX3_BUFFER,sizeof(TX3_BUFFER),0xffff);
	HAL_HalfDuplex_EnableReceiver(&huart3);
}

