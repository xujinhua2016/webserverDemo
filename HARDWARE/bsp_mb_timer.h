#ifndef __BSP_MB_TIMER_H
#define	__BSP_MB_TIMER_H

//#include "stm32f10x.h"
#include  "stm32f4xx.h"

void GE_MB_TIMER_Init(void);

void BSP_MB_TIMER_1_Init(void);
void BSP_MB_TIMER_1_Mode_Init(uint16_t usTim1Timerout50us);
void BSP_MB_TIMER_1_NVIC_Init(void);
void BSP_MB_TIMER_1_Enable(void);
void BSP_MB_TIMER_1_Disable(void);

void BSP_MB_TIMER_2_Init(void);
void BSP_MB_TIMER_2_Mode_Init(uint16_t usTim1Timerout50us);
void BSP_MB_TIMER_2_NVIC_Init(void);
void BSP_MB_TIMER_2_Enable(void);
void BSP_MB_TIMER_2_Disable(void);

#endif

