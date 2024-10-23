#ifndef MOTOR_H
#define MOTOR_H

#include "main.h"
#include "usart.h"
#include <stdio.h>

typedef struct MOTOR
{
	uint8_t pwm1;
	uint8_t pwm2;
	uint8_t pwm3;
	uint8_t pwm4;
}MOTOR;


void Motor_Init(void);
void Motor_Driver(void);
void Motor_Control(uint8_t mode,int8_t vel_1,int8_t vel_2,int8_t vel_3,int8_t vel_4);

void move_forward(void);
void move_stop(void);
void move_backward(void);
void turn_left(void);
void turn_right(void);
void move_left(void);
void move_right(void);


#endif
