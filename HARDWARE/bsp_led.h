/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                        Evaluation Board
*
* Filename      : bsp_led.h
* Version       : V1.00
* Programmer(s) : JOSEPH
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __BSP_LED_H
#define	__BSP_LED_H


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include "stm32f4xx.h"


/*
*********************************************************************************************************
*                                          GPIO PIN DEFINITIONS
*********************************************************************************************************
*/

                          /* -------------------- GPIO PINS -------------------- */
#define  BSP_LED_PIN1                         GPIO_Pin_2
#define  BSP_LED_PIN2                         GPIO_Pin_3
#define  BSP_LED_PIN3                         GPIO_Pin_4
#define  BSP_LED_PIN4                         GPIO_Pin_5

#define  BSP_LED_GPIO1                        GPIOE
#define  BSP_LED_GPIO2                        GPIOE
#define  BSP_LED_GPIO3                        GPIOE
#define  BSP_LED_GPIO4                        GPIOE

#define  BSP_LED_RCC_AHB1Periph1              RCC_AHB1Periph_GPIOE
#define  BSP_LED_RCC_AHB1Periph2              RCC_AHB1Periph_GPIOE
#define  BSP_LED_RCC_AHB1Periph3              RCC_AHB1Periph_GPIOE
#define  BSP_LED_RCC_AHB1Periph4              RCC_AHB1Periph_GPIOE

#define  LED_ON	                                 0
#define  LED_OFF	                               1

#define LED1(a)	if (a)	\
					GPIO_SetBits(BSP_LED_GPIO1,BSP_LED_PIN1);\
					else		\
					GPIO_ResetBits(BSP_LED_GPIO1,BSP_LED_PIN1)

#define LED2(a)	if (a)	\
					GPIO_SetBits(BSP_LED_GPIO2,BSP_LED_PIN2);\
					else		\
					GPIO_ResetBits(BSP_LED_GPIO2,BSP_LED_PIN2)

#define LED3(a)	if (a)	\
					GPIO_SetBits(BSP_LED_GPIO3,BSP_LED_PIN3);\
					else		\
					GPIO_ResetBits(BSP_LED_GPIO3,BSP_LED_PIN3)
					
#define LED4(a)	if (a)	\
					GPIO_SetBits(BSP_LED_GPIO4,BSP_LED_PIN4);\
					else		\
					GPIO_ResetBits(BSP_LED_GPIO4,BSP_LED_PIN4)

#define	Hi(p,i)				{p->BSRRL =i;}				
#define Lo(p,i)				{p->BSRRH	=i;}			
#define Toggle(p,i)		{p->ODR ^=i;}			

#define LED1_TOGGLE		Toggle(BSP_LED_GPIO1,BSP_LED_PIN1)
#define LED1_ON	  		Hi(BSP_LED_GPIO1,BSP_LED_PIN1)
#define LED1_OFF			Lo(BSP_LED_GPIO1,BSP_LED_PIN1)

#define LED2_TOGGLE		Toggle(BSP_LED_GPIO2,BSP_LED_PIN2)
#define LED2_ON 			Hi(BSP_LED_GPIO2,BSP_LED_PIN2)
#define LED2_OFF			Lo(BSP_LED_GPIO2,BSP_LED_PIN2)

#define LED3_TOGGLE		Toggle(BSP_LED_GPIO3,BSP_LED_PIN3)
#define LED3_ON 			Hi(BSP_LED_GPIO3,BSP_LED_PIN3)
#define LED3_OFF			Lo(BSP_LED_GPIO3,BSP_LED_PIN3)

#define LED4_TOGGLE		Toggle(BSP_LED_GPIO4,BSP_LED_PIN4)
#define LED4_ON 			Hi(BSP_LED_GPIO4,BSP_LED_PIN4)
#define LED4_OFF			Lo(BSP_LED_GPIO4,BSP_LED_PIN4)


/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  BSP_LED_Init(void);


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/


#endif                                                          /* End of module include.                               */
