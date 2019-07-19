#include "dht.h"
#include "Wait_Clock.h"
#include "usart1.h"

GPIO_InitTypeDef GPIO_InitStruct;

//初始化DHT11
void DHT11_Init()
{
	
	//开启GPIOG时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	//配置GPIOG
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_9;
	GPIO_InitStruct.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_25MHz;
	GPIO_Init(GPIOG, &GPIO_InitStruct);
}

//调用DHT11模块
int Read_DHT11(char *c)
{
	int i = 0;
	//唤醒DHT11
	DHT11_OUT();
	
	SetGPIO(0);
	delay_ms(20);
	SetGPIO(1);
	delay_us(30);
	
	
	//接收DHT响应
	DHT11_IN();
	while(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9) == 1);		//低电平为响应信号开始
	while(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9) == 0){		//判断低电平是否持续80us
		i++;
		if(i > 17)return -1;
		delay_us(5);
	}
	i = 0;
	
	
	if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9) == 1){			//高电平为响应信号的结束
		while(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9) == 1){	//判断高电平是否持续80us + 20~40的延时等待
			i++;
			if(i > 110)return -1;
			delay_us(1);
		}
	}else{
		return -1;
	}
	
	//接收数据
	if(Read_Data(c) != 0){
		return -2;
	}
	
	//检验数据
	if(c[4] == (c[0] + c[1] +c[2] +c[3])){
		return 0;
	}else{
		return -3;
	}
}

//接收DHT11发送的40bit数据
int Read_Data(char *ch)
{
	int i = 0;
	int j = 0;
	int count = 0;
	
	if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9) == 0){
		for(j = 0; j < 5; j++){
			ch[j] = 0;
			for(i = 0; i < 8; i++){
				while(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9) == 0){
					count++;
					if(count > 50)return -1;
					delay_us(1);
				}
				count = 0;
				delay_us(30);
				if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9) == 1){
					ch[j] |= (0x1 << (7 - i));
				}
				while(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9) == 1){
					count++;
					if(count > 70)return -2;
					delay_us(1);
				}
				count = 0;
			}
		}
		return 0;
	}else{
		return -3;
	}
}
