#ifndef _USART1_H_
#define _USART1_H_

/************************* 头文件 **************************/
#include "stm32f4xx.h"
#include "stdio.h"

/************************* 宏定义 **************************/

/************************ 函数声明 *************************/
 /*
  * 功  能：Usart1初始化
  * 参  数：void
  * 返回值：void
  *
 */
void Usart1_Init(uint32_t n);

 /*
  * 功  能：接收字符串
  * 参  数：uint8_t *str，USART_TypeDef* USARTx --》端口
  * 返回值：void
  *
 */
void Receive_String(uint8_t *str, USART_TypeDef* USARTx);

 /*
  * 功  能：发送字符串
  * 参  数：uint8_t *str，USART_TypeDef* USARTx --》端口
  * 返回值：void
  *
 */
void Send_String(uint8_t *str, USART_TypeDef* USARTx);

#endif
