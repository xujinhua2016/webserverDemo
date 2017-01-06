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
* Filename      : bsp_lcd.h
* Version       : V1.00
* Programmer(s) : WEN
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP_LED present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef __BSP_LCD_H
#define	__BSP_LCD_H


/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

//#include  "stm32f10x.h"
#include  "stm32f4xx.h"


/*
*********************************************************************************************************
*                                          DEFINITIONS
*********************************************************************************************************
*/

/*******************************************************************************************************/
#define  BSP_LCD_ON_OFF_PIN                   GPIO_Pin_9

#define  BSP_LCD_RS_PIN                       GPIO_Pin_8
#define  BSP_LCD_RW_PIN                       GPIO_Pin_7
#define  BSP_LCD_E_PIN                        GPIO_Pin_6

#define  BSP_LCD_DB0_PIN                      GPIO_Pin_6
#define  BSP_LCD_DB1_PIN                      GPIO_Pin_7
#define  BSP_LCD_DB2_PIN                      GPIO_Pin_8
#define  BSP_LCD_DB3_PIN                      GPIO_Pin_9
#define  BSP_LCD_DB4_PIN                      GPIO_Pin_10
#define  BSP_LCD_DB5_PIN                      GPIO_Pin_11
#define  BSP_LCD_DB6_PIN                      GPIO_Pin_12
#define  BSP_LCD_DB7_PIN                      GPIO_Pin_13
/*******************************************************************************************************/
#define  BSP_LCD_ON_OFF_GPIO                  GPIOG

#define  BSP_LCD_RS_GPIO                      GPIOG
#define  BSP_LCD_RW_GPIO                      GPIOG
#define  BSP_LCD_E_GPIO                       GPIOG

#define  BSP_LCD_DB_GPIO                      GPIOC
/*******************************************************************************************************/
#define  BSP_LCD_ON_OFF_RCC_AHB1Periph        RCC_AHB1Periph_GPIOG 

#define  BSP_LCD_RS_RCC_AHB1Periph            RCC_AHB1Periph_GPIOG                
#define  BSP_LCD_RW_RCC_AHB1Periph            RCC_AHB1Periph_GPIOG 
#define  BSP_LCD_E_RCC_AHB1Periph             RCC_AHB1Periph_GPIOG 

#define  BSP_LCD_DB_RCC_AHB1Periph            RCC_AHB1Periph_GPIOC
/*******************************************************************************************************/
#define  BSP_LCD_COM	                        0
#define  BSP_LCD_DAT	                        1
/*******************************************************************************************************/
#define  BSP_LCD_RS_0                         GPIO_ResetBits(BSP_LCD_RS_GPIO, BSP_LCD_RS_PIN)
#define  BSP_LCD_RS_1                         GPIO_SetBits(BSP_LCD_RS_GPIO, BSP_LCD_RS_PIN)
#define  BSP_LCD_RW_0                         GPIO_ResetBits(BSP_LCD_RW_GPIO, BSP_LCD_RW_PIN)
#define  BSP_LCD_RW_1                         GPIO_SetBits(BSP_LCD_RW_GPIO, BSP_LCD_RW_PIN)
#define  BSP_LCD_E_0                          GPIO_ResetBits(BSP_LCD_E_GPIO, BSP_LCD_E_PIN)
#define  BSP_LCD_E_1                          GPIO_SetBits(BSP_LCD_E_GPIO, BSP_LCD_E_PIN)

#define  BSP_LCD_ON                           GPIO_SetBits(BSP_LCD_ON_OFF_GPIO, BSP_LCD_ON_OFF_PIN)
#define  BSP_LCD_OFF                          GPIO_ResetBits(BSP_LCD_ON_OFF_GPIO, BSP_LCD_ON_OFF_PIN)

/*!< GPIO port output data register,        Address offset: 0x14      */
#define  BSP_LCD_DB                           GPIOC->ODR

#define  BSP_LCD_X_Size                       40


/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  BSP_LCD_Init(void);

static  void  BSP_LCD_Delay(__IO uint32_t nCount);
void  BSP_LCD_Clear(void);
void  BSP_LCD_Set_X_Y(unsigned char x, unsigned char y);
void  BSP_LCD_Disp_Str_X_Y(unsigned char x, unsigned char y, char *str);
void  BSP_LCD_Disp_Val_X_Y(unsigned char x, unsigned char y, uint16_t val);

void  BSP_LCD_Disp_Vol(int bat, int val1, int val2, int val3, int val4, int val5, int val6, int val7, int val8);
void  BSP_LCD_Disp_Bad_Vol(void);
void  BSP_LCD_Disp_Res(int bat, int val1, int val2, int val3, int val4, int val5, int val6, int val7, int val8);
void  BSP_LCD_Disp_Bad_Res(void);
void  BSP_Warn_LED(void);

void	BSP_LCD_Disp_CAN_Stat(uint8_t num);
void	BSP_LCD_Disp_Current_Tempe(void);

void	BSP_LCD_Disp_BAT_Info(void);

void	BSP_LCD_Disp_Config_Parameter(void);


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/


#endif                                                          /* End of module include.                               */

