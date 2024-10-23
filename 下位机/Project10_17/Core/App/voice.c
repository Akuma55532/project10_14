#include "voice.h"
#include "servo.h"
#include "motor.h"

uint8_t voice_flag;


void beep_on_times(uint16_t time)
{
	HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_SET);
	HAL_Delay(time);
	HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_SET);
}

void beep_on(void)
{
	HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_SET);
}
	
void beep_off(void)
{
	HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_SET);
}



uint16_t str_contain_str(unsigned char *str, unsigned char *str2) {
	unsigned char *str_temp, *str_temp2;
	str_temp = str;
	str_temp2 = str2;
	while(*str_temp) {
		if(*str_temp == *str_temp2) {
			while(*str_temp2) {
				if(*str_temp++ != *str_temp2++) {
					str_temp = str_temp - (str_temp2-str2) + 1;
					str_temp2 = str2;
					break;
				}	
			}
			if(!*str_temp2) {
				return (str_temp-str);
			}
			
		} else {
			str_temp++;
		}
	}
	return 0;
}






	
