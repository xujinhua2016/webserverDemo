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
* Filename      : bsp_led.c
* Version       : V1.00
* Programmer(s) : Joseph L.Labrosse
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  "bsp_led.h"


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               BSP_KEY_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void BSP_LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(BSP_LED_RCC_AHB1Periph1 \
	                      |BSP_LED_RCC_AHB1Periph2 \
	                      |BSP_LED_RCC_AHB1Periph3 \
	                      |BSP_LED_RCC_AHB1Periph4, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = BSP_LED_PIN1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(BSP_LED_GPIO1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BSP_LED_PIN2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(BSP_LED_GPIO2, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BSP_LED_PIN3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(BSP_LED_GPIO3, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BSP_LED_PIN4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(BSP_LED_GPIO4, &GPIO_InitStructure);
	
//	LED1(LED_OFF);
//	LED2(LED_OFF);
//	LED3(LED_OFF);
//	LED4(LED_OFF);
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
}
/*************************END OF FILE******************************/

