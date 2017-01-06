#ifndef __BSP_MB_USART_H
#define	__BSP_MB_USART_H

//#include "stm32f10x.h"
#include  "stm32f4xx.h"	

#define  GE_MB_485_SEND_DATA    GPIO_SetBits(GPIOA,GPIO_Pin_8);
#define  GE_MB_485_REV_DATA     GPIO_ResetBits(GPIOA,GPIO_Pin_8);

#define  GE_MB_485_USART_NUM    USART1
#define  GE_MB_ARM_USART_NUM    USART3


void GE_MB_USART_Init(void);

void BSP_MB_USART_1_Init(void);
void BSP_MB_USART_1_GPIO_Init(void);
void BSP_MB_USART_1_Mode_Init(uint16_t baudrate);
void BSP_MB_USART_1_NVIC_Init(void);

void BSP_MB_USART_2_Init(void);
void BSP_MB_USART_2_GPIO_Init(void);
void BSP_MB_USART_2_Mode_Init(uint16_t baudrate);
void BSP_MB_USART_2_NVIC_Init(void);

#endif

