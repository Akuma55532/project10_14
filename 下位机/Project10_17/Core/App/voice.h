#ifndef VOICE_H
#define VOICE_H

#include "main.h"
#include "usart.h"
#include <stdio.h>


void beep_on_times(uint16_t time);

void beep_on(void);
	
void beep_off(void);
	
uint16_t str_contain_str(unsigned char *str, unsigned char *str2);

void parse_cmd(uint8_t *cmd);

void beep_on_times(uint16_t time);
void beep_on(void);
void beep_off(void);


#endif

