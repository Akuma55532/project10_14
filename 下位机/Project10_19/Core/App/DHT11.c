//
// Created by Wangxuan on 2024/10/19.
//
#include "DHT11.h"
/*
 * DHT11引脚：输入/输出模式配置函数
 * Mode = 0/INPUT  时 输入模式
 * Mode = 1/OUTPUT 时 输出模式
 */
void DHT11_PIN_Mode(int Mode)
{
    if(Mode)
    {
        GPIO_InitTypeDef GPIO_InitStruct = {0};							// 定义GPIO_InitTypeDef结构体
        GPIO_InitStruct.Pin = DHT11_Pin;                    // 引脚选择
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;         // 引脚模式：输出模式
        GPIO_InitStruct.Pull = GPIO_NOPULL;                 // 配置内部上拉
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;       // 引脚速率：高速
        HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
    }
    else
    {
        GPIO_InitTypeDef GPIO_InitStruct = {0};						// 定义GPIO_InitTypeDef结构体
        GPIO_InitStruct.Pin = DHT11_Pin;				    				// 引脚选择
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;							// 引脚模式：输入模式
        GPIO_InitStruct.Pull = GPIO_NOPULL;									// 配置内部上拉
        HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
    }
}

/*
 *DHT11起始函数
 *根据DHT11时序图，主机要要发送起始信号，需要将总线电平拉低（18~30ms）
 */
void DHT11_Start(void)
{
    DHT11_PIN_Mode(OUTPUT);
    DHT11_IO_SET;    				// 先让总线处于高电平状态
    HAL_Delay(1);
    DHT11_IO_RESET;  				// 拉低总线20ms，表示主机发送起始信号
    HAL_Delay(20);
    DHT11_IO_SET;    				// 将总线拉高等待传感器响应
    DHT11_Delay_us(30);
}

/**
  * DHT11响应检测函数
  * 返回1：未检测到DHT11的存在
  * 返回0：存在出现由高到低的变化即可
  */
uint8_t DHT11_Check(void)
{
    uint8_t retry = 0;
    DHT11_PIN_Mode(INPUT);              //将引脚切换为输入模式
    while(!DHT11_IO_Read && retry<100)  //单片机发送起始信号后，DHT11会将总线拉低83微妙
    {
        retry++;
        DHT11_Delay_us(1);
    }
    if(retry >= 100)return 1;
    else retry = 0;

    while(DHT11_IO_Read && retry<100)  //DHT11拉低后会再次拉高87微妙
    {
        retry++;
        DHT11_Delay_us(1);
    }
    if(retry >= 100) return 1;
    return 0;
}


/**
  * 从DHT11读取一个位
  * 返回值：1/0
  */
uint8_t DHT11_Read_Bit(void)
{
    DHT11_PIN_Mode(INPUT);
    while(!DHT11_IO_Read);
    DHT11_Delay_us(40);
    if(DHT11_IO_Read)
    {
        while(DHT11_IO_Read);
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
  *  读取一个字节数据 1byte / 8bit
  *  返回值是一个字节的数据
  */
uint8_t DHT11_Read_Byte(void)
{
    uint8_t i,buf = 0;                             //  暂时存储数据

    for(i=0; i<8 ;i++)
    {
        buf <<= 1;
        if(DHT11_Read_Bit())                        //  1byte -> 8bit
        {
            buf |= 1;                              //  0000 0001
        }
    }
    return buf;
}

/**
  * 读取温湿度传感器数据 5byte / 40bit
  * 使用方法：创建两个float变量，将变量地址传入函数
  * 注意：两次使用该函数的间隔需要大于2秒，否则会导致数据测量不准确
  */
uint8_t data[5] = {0};

uint8_t DHT11_READ_DATA(float *temp, float *humi)
{
    uint8_t i;
    DHT11_Start();                                 //  主机发送启动信号

    if(!DHT11_Check())                             //  如果DHT11应答
    {
        for(i=0; i<5; i++)
        {
            data[i] = DHT11_Read_Byte();             //  读取 5byte
        }
        if(data[0] + data[1] + data[2] + data[3] == data[4])
        {
            *humi = data[0] + 0.1*data[1];
            *temp = data[2] + 0.1*data[3];
            return 1;                                //  数据校验通过
        }
        else return 0;                              //  数据校验失败
    }
    else return 2;                                 //  如果DHT11不应答
}

/**
  * 微妙延时函数
  * 全系列通用，只需要将宏定义CPU_FREQUENCY_MHZ根据时钟主频修改即可。
  * 系统滴答定时器是HAL库初始化的，且必须有HAL库初始化。
  */
#define CPU_FREQUENCY_MHZ   (int)(HAL_RCC_GetHCLKFreq()/1000000)		// 自动获取STM32时钟主频

void DHT11_Delay_us(__IO uint32_t delay)
{
    int last, curr, val;
    int temp;

    while (delay != 0)
    {
        temp = delay > 900 ? 900 : delay;
        last = SysTick->VAL;
        curr = last - CPU_FREQUENCY_MHZ * temp;
        if (curr >= 0)
        {
            do
            {
                val = SysTick->VAL;
            }
            while ((val < last) && (val >= curr));
        }
        else
        {
            curr += CPU_FREQUENCY_MHZ * 1000;
            do
            {
                val = SysTick->VAL;
            }
            while ((val <= last) || (val > curr));
        }
        delay -= temp;
    }
}

