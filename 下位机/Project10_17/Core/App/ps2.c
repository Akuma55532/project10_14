#include "ps2.h"
#include <string.h>
#include "motor.h"
#include "servo.h"
#include "voice.h"

uint8_t ps2_flag = 0;

uint8_t psx_buf[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};   //存储手柄的数据 



const char *pre_cmd_set_red[PSX_BUTTON_NUM] = {//红灯模式下按键的配置			
	"<PS2_RED01:#005P0600T2000!^#005PDST!>",	//L2						  
	"<PS2_RED02:#005P2400T2000!^#005PDST!>",	//R2						  
	"<PS2_RED03:#004P2400T2000!^#004PDST!>",	//L1						  
	"<PS2_RED04:#004P0600T2000!^#004PDST!>",	//R1			
	"<PS2_RED05:#002P2400T2000!^#002PDST!>",	//RU						  
	"<PS2_RED06:#003P2400T2000!^#003PDST!>",	//RR						  
	"<PS2_RED07:#002P0600T2000!^#002PDST!>",	//RD						  
	"<PS2_RED08:#003P0600T2000!^#003PDST!>",	//RL				
	"<PS2_RED09:$MODE!>",					    //SE    				  
	"<PS2_RED10:>",					            //AL						   
	"<PS2_RED11:>",					            //AR						  
	"<PS2_RED12:$DJR!>",					    //ST		
	"<PS2_RED13:#001P0600T2000!^#001PDST!>",	//LU						  
	"<PS2_RED14:#000P0600T2000!^#000PDST!>",	//LR								  
	"<PS2_RED15:#001P2400T2000!^#001PDST!>",	//LD						  
	"<PS2_RED16:#000P2400T2000!^#000PDST!>",	//LL								
};




void PSX_init(void)
{
	PS2_ATT(1);
	PS2_CMD(1);
	PS2_CLK(1);
}

//int型 取绝对值函数
int abs_int(int int1) {
	if(int1 > 0)return int1;
	return (-int1);
}

unsigned char psx_transfer(unsigned char dat) 
{
	unsigned char rd_data ,wt_data, i;
	wt_data = dat;
	rd_data = 0;
	for(i = 0;i < 8;i++){
		PS2_CMD((wt_data & (0x01 << i)));
		Delay_us(10);
		PS2_CLK(1);
		Delay_us(10);
		PS2_CLK(0);
		Delay_us(10);
		PS2_CLK(1);
		if(PS2_DAT) {
			rd_data |= 0x01<<i;
		}
	}
	return rd_data;
}

void psx_write_read(unsigned char *get_buf) {
	PS2_ATT(0);
	get_buf[0] = psx_transfer(START_CMD);
	get_buf[1] = psx_transfer(ASK_DAT_CMD);
	get_buf[2] = psx_transfer(get_buf[0]);
	get_buf[3] = psx_transfer(get_buf[0]);
	get_buf[4] = psx_transfer(get_buf[0]);
	get_buf[5] = psx_transfer(get_buf[0]);
	get_buf[6] = psx_transfer(get_buf[0]);
	get_buf[7] = psx_transfer(get_buf[0]);
	get_buf[8] = psx_transfer(get_buf[0]);	
	PS2_ATT(1);
	
	return;
}

//处理手柄上的按钮
void loop_ps2_button(void) {
	static unsigned char psx_button_bak[2] = {0};

	//对比两次获取的按键值是否相同 ，相同就不处理，不相同则处理
	if((psx_button_bak[0] == psx_buf[3])
	&& (psx_button_bak[1] == psx_buf[4])) {				
	} else {		
		//处理buf3和buf4两个字节，这两个字节存储这手柄16个按键的状态
		parse_psx_buf(psx_buf+3, psx_buf[1]);
		psx_button_bak[0] = psx_buf[3];
		psx_button_bak[1] = psx_buf[4];
	}
	return;
}	

//处理手柄按键字符，buf为字符数组，mode是指模式 主要是红灯和绿灯模式
void parse_psx_buf(unsigned char *buf, unsigned char mode) {
	uint8_t i = 0;
	static uint16_t bak=0xffff, temp, temp2;
	temp = (buf[0]<<8) + buf[1]; // 首先计算当前按键状态 temp
	
	if(bak != temp) {  //检查当前状态 (temp) 是否与上次状态 (bak) 相同
		temp2 = temp;
		temp &= bak;    
		for(i=0;i<16;i++) {     //16个按键一次轮询
			if((1<<i) & temp) {
			} else {
				if((1<<i) & bak) {	// 如果不同，则进一步检查哪些按键的状态发生了变化。				
					if(mode == PS2_LED_RED){
						switch(i)
						{
							case 4:
								break;
							case 5:
								break;
							case 6:
								break;
							case 7:
								break;
							case 9:
								move_left();
								break;
							case 10:
								move_right();
								break;
							case 11:
								beep_on_times(100);
								move_stop();
								Servo_Init();  //机械臂复位
								break;
							case 12:
								Servo_Init();
								break;
						}		
					bak = 0xffff;
				} else {				
					if(mode == PS2_LED_RED){
						if (i == 9 || i == 10) {   //摇杆抬起时停止
							move_stop();	
						}									
					}										
					}	
				}
			}
		}
		bak = temp2;
	}	
	return;
}

/*************************************************************
功能介绍：发送串口指令控制电机转动
函数参数：左前轮速度，右前轮速度，左后轮速度，右后轮速度，范围：-1000~1000， 负值反转，正值正转
返回值：  无  
*************************************************************/
void car_run(int speedlq, int speedrq, int speedlh, int speedrh) {	
	sprintf((char *)TX3_BUFFER, "{#006P%04dT0000!#007P%04dT0000!#008P%04dT0000!#009P%04dT0000!}", (int)(1500+speedlq), (int)(1500-speedrq), (int)(1500+speedlh), (int)(1500-speedrh));
	HAL_HalfDuplex_EnableTransmitter(&huart3);
	HAL_UART_Transmit(&huart3,TX3_BUFFER,sizeof(TX3_BUFFER),0xffff);
	HAL_HalfDuplex_EnableReceiver(&huart3);
}

//处理小车电机摇杆控制
void loop_ps2_car(void) {
	static int ps2_left, ps2_right, ps2_left_temp, ps2_right_temp;
	
	if(psx_buf[1] != PS2_LED_RED)return;
	
	if(abs_int(127 - psx_buf[8]) < 5 )psx_buf[8] = 127;
	if(abs_int(127 - psx_buf[6]) < 5 )psx_buf[6] = 127;
	
	//总线马达设置	
	ps2_left = (127 - psx_buf[8]) * 4;
	ps2_right = (127 - psx_buf[5]) * 2;
	
	if(abs_int(ps2_left) < 100)ps2_left = 0;
	if(ps2_left > 1000)ps2_left = 1000;
	if(ps2_left < -1000)ps2_left = -1000;
	
	if(abs_int(ps2_right) < 100)ps2_right = 0;
	if(ps2_right > 1000)ps2_right = 1000;
	if(ps2_right < -1000)ps2_right = -1000;

	if(ps2_left != ps2_left_temp || ps2_right != ps2_right_temp) 
	{
		car_run(ps2_left-ps2_right, ps2_left+ps2_right, ps2_left-ps2_right, ps2_left+ps2_right);
		ps2_left_temp = ps2_left;
		ps2_right_temp = ps2_right;
	}
}
