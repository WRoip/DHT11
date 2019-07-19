#include "stm32f4xx.h"
#include "dht.h"
#include "Wait_Clock.h"
#include "usart1.h"


int main(void)
{
	char c[5] = {0};
	Systick_Init();
	DHT11_Init();
	Usart1_Init(115200);
	
	printf("初始化成功！开始监测湿度及温度！\r\n");
	
	while(1){
		if(Read_DHT11(c) == 0){
			printf("当前湿度:%d.%d%%RH   温度:%d.%d℃\r\n\r\n", c[0],c[1],c[2],c[3]);
		}else{
			printf("监测出错！\r\n");
		}
		delay_s(2);
	}
	
}
