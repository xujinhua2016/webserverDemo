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
* Filename      : bsp_key.c
* Version       : V1.00
* Programmer(s) : ENUT
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  "bsp_key.h" 
#include  "ucos_ii.h"


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

static void BSP_KEY_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(BSP_KEY1_RCC_AHB1Periph \
	                      |BSP_KEY2_RCC_AHB1Periph \
	                      |BSP_KEY3_RCC_AHB1Periph \
	                      |BSP_KEY4_RCC_AHB1Periph, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/*!< GPIO Input Mode */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//иою╜
	
	GPIO_InitStructure.GPIO_Pin = BSP_KEY_PIN1; 
	GPIO_Init(BSP_KEY_GPIO1,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BSP_KEY_PIN2;
	GPIO_Init(BSP_KEY_GPIO2,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BSP_KEY_PIN3;
	GPIO_Init(BSP_KEY_GPIO3,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BSP_KEY_PIN4;
	GPIO_Init(BSP_KEY_GPIO4,&GPIO_InitStructure);
}


/*
*********************************************************************************************************
*                                  Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin)
*
* Description : TOGGLE any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       1    TOGGLE user LED1  on the board
*                       2    TOGGLE user LED2  on the board
*                       3    TOGGLE user LED3  on the board
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

uint8_t  Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin)
{
	uint16_t i = 0;
	
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==KEY_ON )                 /* Detection button               */
		{
			OSTimeDlyHMSM(0, 0, 0, 10);		                                 /* Disappears shakes              */
			
			if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==KEY_ON )
				{
					while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==KEY_ON)  		 /* Waiting for the button release */
					{
						OSTimeDlyHMSM(0, 0, 0, 10);
						
						i++;
						
						if(i > 100)
						{
							return 	KEY_HOLD;
						}
					}
					return 	KEY_ON;
        }
				else
				{
					return KEY_OFF;
				}
    }
		else
		{
			return KEY_OFF;
		}
}


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

void BSP_KEY_Init(void)
{
	BSP_KEY_GPIO_Config();
}
/*************************END OF FILE******************************/

