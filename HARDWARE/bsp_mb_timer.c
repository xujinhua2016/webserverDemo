#include "bsp_mb_timer.h"
#include "bsp_led.h"

extern uint8_t MB_TimeExpireT3_5;


void GE_MB_TIMER_Init(void)
{
	BSP_MB_TIMER_1_Init();
	//BSP_MB_TIMER_2_Init();
}


void BSP_MB_TIMER_1_Init(void)
{
	BSP_MB_TIMER_1_Mode_Init(35);
	BSP_MB_TIMER_1_NVIC_Init();
}


void BSP_MB_TIMER_1_Mode_Init(uint16_t usTim1Timerout50us)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
  uint16_t PrescalerValue = 0;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  
  //定时器时间基配置说明
  //HCLK为72MHz，APB1经过2分频为36MHz
  //TIM4的时钟倍频后为72MHz（硬件自动倍频,达到最大）
  //TIM4的分频系数为3599，时间基频率为72 / (1 + Prescaler) = 20KHz,基准为50us
  //TIM最大计数值为usTim1Timerout50u
  PrescalerValue = (uint16_t) (SystemCoreClock / 20000) - 1; 
	
  TIM_TimeBaseStructure.TIM_Period = (uint16_t) usTim1Timerout50us - 1;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  TIM_ARRPreloadConfig(TIM3, ENABLE);
  
  TIM_Cmd(TIM3, DISABLE);
}


void BSP_MB_TIMER_1_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
  TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
}

void BSP_MB_TIMER_1_Enable(void)
{
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  TIM_SetCounter(TIM3,0x0000);
  TIM_Cmd(TIM3, ENABLE);
}


void BSP_MB_TIMER_1_Disable(void)
{
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
  TIM_SetCounter(TIM3,0x0000);
  TIM_Cmd(TIM3, DISABLE);
}


//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
//  {
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
//		BSP_MB_TIMER_1_Disable();
//		
//		MB_TimeExpireT3_5 = 1;
//  }
//}


void BSP_MB_TIMER_2_Init(void)
{
	BSP_MB_TIMER_2_Mode_Init(35);
	BSP_MB_TIMER_2_NVIC_Init();
}


void BSP_MB_TIMER_2_Mode_Init(uint16_t usTim1Timerout50us)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
  uint16_t PrescalerValue = 0;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  
  //定时器时间基配置说明
  //HCLK为72MHz，APB1经过2分频为36MHz
  //TIM4的时钟倍频后为72MHz（硬件自动倍频,达到最大）
  //TIM4的分频系数为3599，时间基频率为72 / (1 + Prescaler) = 20KHz,基准为50us
  //TIM最大计数值为usTim1Timerout50u
  PrescalerValue = (uint16_t) (SystemCoreClock / 20000) - 1; 
	
  TIM_TimeBaseStructure.TIM_Period = (uint16_t) usTim1Timerout50us - 1;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  TIM_ARRPreloadConfig(TIM4, ENABLE);
  
  TIM_Cmd(TIM4, DISABLE);
}


void BSP_MB_TIMER_2_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
  TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
}

void BSP_MB_TIMER_2_Enable(void)
{
  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  TIM_SetCounter(TIM4,0x0000);
  TIM_Cmd(TIM4, ENABLE);
}


void BSP_MB_TIMER_2_Disable(void)
{
  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
  TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
  TIM_SetCounter(TIM4,0x0000);
  TIM_Cmd(TIM4, DISABLE);
}


void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
  }
}

