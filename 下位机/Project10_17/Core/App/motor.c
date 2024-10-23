#include "motor.h"
#include <math.h>

#define pi 3.1415926

MOTOR MyMotor;

void Motor_Init(void)
{
	sprintf((char *)TX3_BUFFER,"{#006P1500T1000!#007P1500T1000!#008P1500T1000!#009P1500T1000!}");
	HAL_HalfDuplex_EnableTransmitter(&huart3);
	HAL_UART_Transmit(&huart3,TX3_BUFFER,sizeof(TX3_BUFFER),0xffff);
	HAL_HalfDuplex_EnableReceiver(&huart3);
}

void Motor_Driver(void)
{
	sprintf((char *)TX3_BUFFER,"{#006P%03u0T1000!#007P%03u0T1000!#008P%03u0T1000!#009P%03u0T1000!}",MyMotor.pwm1,MyMotor.pwm2,MyMotor.pwm3,MyMotor.pwm4);
	HAL_HalfDuplex_EnableTransmitter(&huart3);
	HAL_UART_Transmit(&huart3,TX3_BUFFER,sizeof(TX3_BUFFER),0xffff);
	HAL_HalfDuplex_EnableReceiver(&huart3);
}

void Motor_Driver2(void)
{
	sprintf((char *)TX3_BUFFER,"{#006P%03u0T0000!#007P%03u0T0000!#008P%03u0T0000!#009P%03u0T0000!}",MyMotor.pwm1,MyMotor.pwm2,MyMotor.pwm3,MyMotor.pwm4);
	HAL_HalfDuplex_EnableTransmitter(&huart3);
	HAL_UART_Transmit(&huart3,TX3_BUFFER,sizeof(TX3_BUFFER),0xffff);
	HAL_HalfDuplex_EnableReceiver(&huart3);
}

void Motor_Control(uint8_t mode,int8_t vel_1,int8_t vel_2,int8_t vel_3,int8_t vel_4)
{
	MyMotor.pwm1 = (vel_1+15) * 10;
	MyMotor.pwm2 = (15-vel_2) * 10;
	MyMotor.pwm3 = (vel_3+15) * 10;
	MyMotor.pwm4 = (15-vel_4) * 10;
	if (mode == 1)
	{
		Motor_Driver();
	}
	if (mode == 2)
	{
		Motor_Driver2();
	}
}

void move_forward(void)
{
	Motor_Control(2,5,5,5,5);
}

void move_stop(void)
{
	Motor_Control(2,0,0,0,0);
}


void move_backward(void)
{
	Motor_Control(2,-5,-5,-5,-5);
}

void turn_left(void)
{
	Motor_Control(2,-5,5,-5,5);
}

void turn_right(void)
{
	Motor_Control(2,5,-5,5,-5);
}
void move_left(void)
{
	Motor_Control(2,-5,5,7,-7);
}
void move_right(void)
{
	Motor_Control(2,5,-5,-7,7);
}
