#ifndef _WAIT_CLOCK_H_
#define _WAIT_CLOCK_H_

#include "stm32f4xx.h"

void Systick_Init(void);
void delay_us(u32 nus);
void delay_ms(u32 nms);
void delay_s(u32 ns);

#endif
