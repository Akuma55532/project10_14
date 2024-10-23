#ifndef SERVO_H
#define SERVO_H

#include "main.h"
#include "usart.h"
#include <stdio.h>


extern uint8_t arm_x,arm_y,arm_z;

extern uint8_t servo_move_flag;

void Servo_Init(void);



void action_arm2(void);


void action_do_x(uint8_t direct);

void action_do_y(uint8_t direct);

void test1_servo(void);

void test2_servo(void);

void test3_servo(void);

#endif

