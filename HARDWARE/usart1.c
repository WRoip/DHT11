#include "usart1.h"

//半主机模式
#pragma import(__use_no_semihosting) 
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;       
void _sys_exit(int x) 
{ 
	x = x; 
}

//初始化Usart1串口
void Usart1_Init(uint32_t BaudRate)
{
	
	GPIO_InitTypeDef				GPIO_InitStruct;
	USART_InitTypeDef				USART_InitStruct;
	NVIC_InitTypeDef				NVIC_InitStruct;
	
	//开启USART1，GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	//GPIOA9,10端口复用USART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	//初始化GPIOA9,10端口
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//初始化USART1
	USART_InitStruct.USART_BaudRate							= BaudRate;
	USART_InitStruct.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;//关闭硬件流控;
	USART_InitStruct.USART_Mode									= USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity								= USART_Parity_No;
	USART_InitStruct.USART_StopBits							= USART_StopBits_1;
	USART_InitStruct.USART_WordLength						= USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);
	
	//设置USART1中断事件
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//开启中断
	NVIC_InitStruct.NVIC_IRQChannel										= USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd								= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority				= 0;
	NVIC_Init(&NVIC_InitStruct);
	
	//开启USART1
	USART_Cmd(USART1, ENABLE);
}

//中断触发函数
void USART1_IRQHandler(void)
{
	uint8_t *str;
	uint8_t c[10];
	int i = 0;
	str = &c[0];

	if((USART_GetFlagStatus(USART1, USART_FLAG_RXNE)) != 0){
		for(i = 0; i < 10; i++)str[i] = 0;
		Receive_String(str, USART1);
		Send_String(str, USART1);
		
	}
}

//发送字符串
void Send_String(uint8_t *str , USART_TypeDef* USARTx)
{
	while(((USARTx->SR) & (0x1 << 7)) == 0);
	printf("%s",str);
}

//接收字符串
void Receive_String(uint8_t *str, USART_TypeDef* USARTx)
{
	int i = 0;
	while(((USARTx->SR) & (0x1 << 5)) != 0){
		str[i++] = (uint8_t)USARTx->DR;
	}
}

//重写fputc
int fputc(int c, FILE *f)
{
		while(((USART1->SR) & (0x1 << 7)) == 0);
		USART1->DR = c;
	return c;
}
