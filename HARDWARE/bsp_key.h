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
* Filename      : bsp_key.h
* Version       : V1.00
* Programmer(s) : WEN
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

#ifndef __BSP_KEY_H
#define	__BSP_KEY_H


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

//#include  "stm32f10x.h"
#include  "stm32f4xx.h"


/*
*********************************************************************************************************
*                                          GPIO PIN DEFINITIONS
*********************************************************************************************************
*/

/* -------------------- GPIOB PINS -------------------- */
#define  BSP_KEY_PIN1                         GPIO_Pin_12
#define  BSP_KEY_PIN2                         GPIO_Pin_13
#define  BSP_KEY_PIN3                         GPIO_Pin_14
#define  BSP_KEY_PIN4                         GPIO_Pin_15 // Reserve

#define  BSP_KEY_GPIO1                        GPIOB
#define  BSP_KEY_GPIO2                        GPIOB
#define  BSP_KEY_GPIO3                        GPIOB
#define  BSP_KEY_GPIO4                        GPIOB

#define  BSP_KEY1_RCC_AHB1Periph              RCC_AHB1Periph_GPIOB                
#define  BSP_KEY2_RCC_AHB1Periph              RCC_AHB1Periph_GPIOB 
#define  BSP_KEY3_RCC_AHB1Periph              RCC_AHB1Periph_GPIOB 
#define  BSP_KEY4_RCC_AHB1Periph              RCC_AHB1Periph_GPIOB

#define  KEY_ON	                                 0
#define  KEY_OFF	                               1
#define  KEY_HOLD	                               2



/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  BSP_KEY_Init(void);


/*
*********************************************************************************************************
*                                              KEY SERVICES
*********************************************************************************************************
*/

uint8_t  Key_Scan (GPIO_TypeDef* GPIOx, u16 GPIO_Pin);


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/


#endif                                                          /* End of module include.                               */
