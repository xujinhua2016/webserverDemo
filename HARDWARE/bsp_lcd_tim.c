/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                              Fendou
*                                        Evaluation Board
*
* Filename      : bsp_tim.c
* Version       : V1.00
* Programmer(s) : ENUT
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  "bsp_lcd.h"
#include  "bsp_lcd_tim.h"

__IO uint8_t BSP_LCD_ON_TIME = 0;

/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          BSP_CHARGE_GPIO_Config()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : 
*
*********************************************************************************************************
*/

void BSP_TIM2_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	/*
		Tout=((arr+1)*(psc+1))/Ft us.
		���ڲ�ʱ��ͼ֪����APB1��ʱ�ӷ�Ƶ����Ϊ1��ʱ��TIM2-7,TIM12-14��ʱ��Ϊ
		APB1��ʱ�ӣ�42M��,�����APB1��ʱ�ӷ�Ƶ����Ϊ1����ô��ʱ����ΪAPB1������
	*/
	TIM_TimeBaseStructure.TIM_Period = 9999; 											//�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 8399; 								  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���ϼ���ģʽ
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	TIM2_NVIC_Init();

	TIM_Cmd(TIM2,ENABLE);
}


/*
*********************************************************************************************************
*                                          TIM3_NVIC_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : 
*
*********************************************************************************************************
*/

void TIM2_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}


/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM3_IRQHandler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET )
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		
		BSP_LCD_ON_TIME = BSP_LCD_ON_TIME + 1;	// When BSP_LCD_ON_TIME = 30(30s), close LCD screen. 
		
		if(BSP_LCD_ON_TIME == 120)	// When BSP_LCD_ON_TIME = 30(30s), close LCD screen. 
		{
			TIM_Cmd(TIM2, DISABLE);	// Disable TIM3, waiting for key press to enable TIM3.
			
			BSP_LCD_OFF;	// LCD screen ON.
			BSP_LCD_ON_TIME = 0;	// Clear.
		}
	}
}


/*************************END OF FILE******************************/

