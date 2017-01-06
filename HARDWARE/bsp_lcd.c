/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              F103VET6
*
* Notes         : LCD display function.
*
* Filename      : bsp_lcd.c
* Version       : V1.00
* Programmer(s) : WEN
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include  "stm32f4xx.h"
#include  "bsp_lcd.h"
#include  "includes.h"

#include "lwip_comm.h" 

/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/

extern  __IO	uint8_t	 BSP_LCD_PAGE_Disp_flag;


int  BSP_LCD_BAT_X[25]      = {0,  0, 10, 20, 30,  0, 10, 20, 30,  0, 10, 20, 30, \
                               0, 10, 20, 30,  0, 10, 20, 30,  0, 10, 20, 30};
int  BSP_LCD_BAT_Y[25]      = {0,  1,  1,  1,  1,  2,  2,  2,  2,  1,  1,  1,  1, \
                               2,  2,  2,  2,  1,  1,  1,  1,  2,  2,  2,  2};
int  BSP_LCD_BAT_NUMBER[25] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  1,  2,  3,  4, \
                               5,  6,  7,  8,  1,  2,  3,  4,  5,  6,  7,  8};

															 
extern __lwip_dev  lwipdev;						//lwip控制结构体 

/*
*********************************************************************************************************
*                                          EXTERN DEFINES
*********************************************************************************************************
*/														 

extern  __IO  int  BSP_BAT_Vol_Buf[25];
extern  __IO  int  BSP_BAT_Res[25];
extern  __IO  int  BSP_BAT_Vol_Buf[25];

extern	__IO	uint32_t  BSP_BAT_Vol_Min_To_Max[24];
extern	__IO	uint8_t	  BSP_BAT_Vol_Min_To_Max_Num[24];


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  BSP_LCD_GPIO_Init(void);
static  void  BSP_LCD_Screen_Init(void);
static  void  BSP_LCD_Write(unsigned char dat_comm, unsigned char content);


/*
*********************************************************************************************************
*                                             BSP_LCD_GPIO_Init()
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
////////////////////////////////////////////
////////////////////////////////////////////


static  void  BSP_LCD_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;                            /* Define a GPIO_InitTypeDef                   */
		
	RCC_AHB1PeriphClockCmd(BSP_LCD_ON_OFF_RCC_AHB1Periph \
		                     |BSP_LCD_RS_RCC_AHB1Periph \
	                       |BSP_LCD_RW_RCC_AHB1Periph \
	                       |BSP_LCD_E_RCC_AHB1Periph \
	                       |BSP_LCD_DB_RCC_AHB1Periph, ENABLE);     /* Open GPIOB,GPIOD peripheral clock           */
  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;               	  /* Initialize general push-pull output mode    */  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;									//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;               /* IO speed 50MHz                              */ 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	
	GPIO_InitStructure.GPIO_Pin = BSP_LCD_ON_OFF_PIN;	                  /* Choose GPIOB IO                             */  
	GPIO_Init(BSP_LCD_ON_OFF_GPIO, &GPIO_InitStructure);	              /* Initialize GPIOB5                           */
  
	GPIO_InitStructure.GPIO_Pin = BSP_LCD_RS_PIN;	               		    /* Choose GPIOB IO                             */  
	GPIO_Init(BSP_LCD_RS_GPIO, &GPIO_InitStructure);	            		  /* Initialize GPIOB5                           */
  
	GPIO_InitStructure.GPIO_Pin = BSP_LCD_RW_PIN;                  		  /* Choose GPIOD IO                             */	
  GPIO_Init(BSP_LCD_RW_GPIO, &GPIO_InitStructure);	              		/* Initialize GPIOD3                           */
  
	GPIO_InitStructure.GPIO_Pin = BSP_LCD_E_PIN;                    		/* Choose GPIOD IO                             */		
  GPIO_Init(BSP_LCD_E_GPIO, &GPIO_InitStructure);                 		/* Initialize GPIOD6                           */			  
  
	GPIO_InitStructure.GPIO_Pin = BSP_LCD_DB0_PIN \
	                             |BSP_LCD_DB1_PIN \
															 |BSP_LCD_DB2_PIN \
															 |BSP_LCD_DB3_PIN \
															 |BSP_LCD_DB4_PIN \
															 |BSP_LCD_DB5_PIN \
															 |BSP_LCD_DB6_PIN \
															 |BSP_LCD_DB7_PIN;
	GPIO_Init(BSP_LCD_DB_GPIO, &GPIO_InitStructure);
	
	BSP_LCD_ON;
}


/*
*********************************************************************************************************
*                                             BSP_LCD_Delay()
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

static void  BSP_LCD_Delay(__IO uint32_t nCount)	 
{
	for(; nCount != 0; nCount--);
}


/*
*********************************************************************************************************
*                                             BSP_LCD_Screen_Init()
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


static  void  BSP_LCD_Screen_Init(void)
{
	BSP_LCD_E_0;
	
  BSP_LCD_Write(BSP_LCD_COM, 0x01);  // 清屏，地址指针指向00H
	
  BSP_LCD_Write(BSP_LCD_COM, 0x06);  // 光标的移动方向 0000 0110
	
  BSP_LCD_Write(BSP_LCD_COM, 0x0C);  // 开显示，关光标 0000 1100
	
  BSP_LCD_Write(BSP_LCD_COM, 0x38);  // 8位接口两行显示模式 0011 1000 
}


/*
*********************************************************************************************************
*                                             BSP_LCD_Write()
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

static  void  BSP_LCD_Write(unsigned char dat_comm, unsigned char content)
{
	BSP_LCD_E_0;
	
	if(dat_comm)			//写数据
	{
		BSP_LCD_RS_1;
    BSP_LCD_RW_0;
	}
	else							//写命令
	{
		BSP_LCD_RS_0;
    BSP_LCD_RW_0;
	}
	//lcd数据引脚DB0-DB7使用，PC6-PC13引脚，所以初始化时候默认相应位为0，0000 0000 0011 1111
	BSP_LCD_DB &= 0x003F;			
  BSP_LCD_DB |= (content<<6);
	 
	/////////
	BSP_LCD_Delay(0x00FF);
	/////////
	
	
  BSP_LCD_E_1;
	//f103设置为0x4FFF
  BSP_LCD_Delay(0x84FF);
  BSP_LCD_E_0;
	//BSP_LCD_Delay(0x00FF);
}


/*
*********************************************************************************************************
*                                               BSP_LCD_Init()
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

void  BSP_LCD_Init(void)
{
	BSP_LCD_GPIO_Init();
	BSP_LCD_Screen_Init();
}


/*
*********************************************************************************************************
*                                        BSP_LCD_Clear(void)
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

void  BSP_LCD_Clear(void)
{
	BSP_LCD_Write(BSP_LCD_COM, 0x01);  // 清屏，地址指针指向00H
}


/*
*********************************************************************************************************
*                                BSP_LCD_Set_X_Y(unsigned char x, unsigned char y)
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

void  BSP_LCD_Set_X_Y(unsigned char x, unsigned char y)
{
	unsigned char address;
	
	if (y == 1)			//显示在第一行
	{
		address = 0x80 + x;
	}
	else						//显示在第二行
	{
		address = 0xC0 + x;
	}
	BSP_LCD_Write(BSP_LCD_COM, address);
}


/*
*********************************************************************************************************
*                        BSP_LCD_Disp_Str_X_Y(unsigned char x, unsigned char y, char *str)
*
* Description : Initialize the Board Support Package (BSP).显示字符串
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

void  BSP_LCD_Disp_Str_X_Y(unsigned char x, unsigned char y, char *str)
{
	BSP_LCD_Set_X_Y(x, y);		//显示的位置
	
	while (*str != '\0')			//字符显示
	{
		BSP_LCD_Write(BSP_LCD_DAT, *str);//写字符串数据
		str++;
  }
}


/*
*********************************************************************************************************
*                    BSP_LCD_Disp_Val_X_Y(unsigned char x, unsigned char y, uint16_t val)
*
* Description : Initialize the Board Support Package (BSP).
*								//在某一个位置上显示数值
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

void  BSP_LCD_Disp_Val_X_Y(unsigned char x, unsigned char y, uint16_t val)
{
	unsigned char ths, hun, deca, unit;
	
	ths = val / 1000;
	val = val % 1000;
	hun = val / 100;
	val = val % 100;
	deca = val / 10;
	unit = val % 10;;
	
	BSP_LCD_Set_X_Y(x, y);
	//数字0在ASCII编码是48，使用十六进制表示为0X30
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + ths);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + hun);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + deca);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + unit);
}


/*
*********************************************************************************************************
* BSP_LCD_Disp_Vol(int bat, int val1, int val2, int val3, int val4, int val5, int val6, int val7, int val8)
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

void  BSP_LCD_Disp_Vol(int bat, int val1, int val2, int val3, int val4, int val5, int val6, int val7, int val8)
{
	unsigned char ths, hun, deca, unit;
	int temp[9], i, j, k;
	
	temp[1] = val1;
	temp[2] = val2;
	temp[3] = val3;
	temp[4] = val4;
	temp[5] = val5;
	temp[6] = val6;
	temp[7] = val7;
	temp[8] = val8;
	
	BSP_LCD_Write(BSP_LCD_COM, 0x80);
	
	for (i=1; i<9; i++)
	{
		ths     = temp[i] / 1000;
		temp[i] = temp[i] % 1000;
		hun     = temp[i] / 100;
		temp[i] = temp[i] % 100;
		deca    = temp[i] / 10;
		unit    = temp[i] % 10;
		
		j = (8 * (bat - 1) + i) / 10;
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + j);
		k = (8 * (bat - 1) + i) % 10;
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + k);
		
		BSP_LCD_Write(BSP_LCD_DAT, *("-"));
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + ths);
		BSP_LCD_Write(BSP_LCD_DAT, *("."));
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + hun);
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + deca);
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + unit);
		BSP_LCD_Write(BSP_LCD_DAT, *("V"));
		BSP_LCD_Write(BSP_LCD_DAT, *(" "));
	}
}


/*
*********************************************************************************************************
* BSP_LCD_Disp_Res(int bat, int val1, int val2, int val3, int val4, int val5, int val6, int val7, int val8)
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

void  BSP_LCD_Disp_Res(int bat, int val1, int val2, int val3, int val4, int val5, int val6, int val7, int val8)
{
	unsigned char hun, deca, unit;
	int temp[9], i, j, k;
	
	temp[1] = val1;
	temp[2] = val2;
	temp[3] = val3;
	temp[4] = val4;
	temp[5] = val5;
	temp[6] = val6;
	temp[7] = val7;
	temp[8] = val8;
	
	BSP_LCD_Write(BSP_LCD_COM, 0x80);
	
	BSP_LCD_Set_X_Y(0,1);
	
	for (i=1; i<9; i++)
	{
		hun     = temp[i] / 100;
		temp[i] = temp[i] % 100;
		deca    = temp[i] / 10;
		unit    = temp[i] % 10;
		
		j = (8 * (bat - 1) + i) / 10;
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + j);
		k = (8 * (bat - 1) + i) % 10;
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + k);
		BSP_LCD_Write(BSP_LCD_DAT, *("-"));
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + hun);
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + deca);
	  BSP_LCD_Write(BSP_LCD_DAT, *("."));
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + unit);
		BSP_LCD_Write(BSP_LCD_DAT, *("m"));
		BSP_LCD_Write(BSP_LCD_DAT, 0xF0 + 4);
		BSP_LCD_Write(BSP_LCD_DAT, *(" "));
	}
}


/*
*********************************************************************************************************
*                                     BSP_LCD_Disp_Bad_Vol(void)
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

void  BSP_LCD_Disp_Bad_Vol(void)
{
	int i;
	
	switch(BSP_LCD_PAGE_Disp_flag)	//根据液晶显示屏切换页标志量进行选择
	{
		case 1:														//擦出电池电压的数据，为做闪烁的显示做准备
			for(i=0;i<8;i++)
		  {
				if(BatVolWarnBit000_015 & (1<<i))
				{
					BSP_LCD_Disp_Str_X_Y(BSP_LCD_BAT_X[i+1],BSP_LCD_BAT_Y[i+1],"          ");
				}
			}
		break;
		
		case 2:														//擦出电池内阻的数据，为做闪烁的显示做准备
			for(i=8;i<16;i++)
		  {
				if(BatVolWarnBit000_015 & (1<<i))
				{
					BSP_LCD_Disp_Str_X_Y(BSP_LCD_BAT_X[i+1],BSP_LCD_BAT_Y[i+1],"          ");
				}
			}
		break;
		
		case 3:
			for(i=0;i<8;i++)
		  {
				if(BatVolWarnBit016_031 & (1<<i))
				{
					BSP_LCD_Disp_Str_X_Y(BSP_LCD_BAT_X[i+1],BSP_LCD_BAT_Y[i+1],"          ");
				}
			}
		break;
		
		default:
			
		break;
	}
}


/*
*********************************************************************************************************
*                                         BSP_LCD_Disp_Bad_Res(void)
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

void  BSP_LCD_Disp_Bad_Res(void)
{
	int i;
	
	switch(BSP_LCD_PAGE_Disp_flag)
	{
		case 1:
			for(i=0;i<8;i++)
		  {
				if(BatResWarnBit000_015 & (1<<i))
				{
					BSP_LCD_Disp_Str_X_Y(BSP_LCD_BAT_X[i+1],BSP_LCD_BAT_Y[i+1],"          ");
				}
			}
		break;
		
		case 2:
			for(i=8;i<16;i++)
		  {
				if(BatResWarnBit000_015 & (1<<i))
				{
					BSP_LCD_Disp_Str_X_Y(BSP_LCD_BAT_X[i+1],BSP_LCD_BAT_Y[i+1],"          ");
				}
			}
		break;
		
		case 3:
			for(i=0;i<8;i++)
		  {
				if(BatResWarnBit016_031 & (1<<i))
				{
					BSP_LCD_Disp_Str_X_Y(BSP_LCD_BAT_X[i+1],BSP_LCD_BAT_Y[i+1],"          ");
				}
			}
		break;
		
		default:
			
		break;
	}
}


/*
*********************************************************************************************************
*                                         BSP_Warn_LED(void)
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

void  BSP_Warn_LED(void)
{
	LED3_OFF;
	LED4_OFF;
	
	OSTimeDlyHMSM(0, 0, 0, 200);
	
	if((BatVolWarnBit000_015 | BatVolWarnBit016_031) != 0)
	{
		LED3_ON;
	}
	
	if((BatResWarnBit000_015 | BatResWarnBit016_031) != 0)
	{
		LED4_ON;
	}
}


/*
*********************************************************************************************************
*                                         BSP_LCD_Disp_CAN_Stat(uint8_t num)
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : num = 0x00.
*
*               1->0x01.   0000 0001
*               2->0x02.   0000 0010
*               3->0x03.   0000 0100
*               4->0x08.   0000 1000
*
*********************************************************************************************************
*/

void	BSP_LCD_Disp_CAN_Stat(uint8_t num)
{
	BSP_LCD_Disp_Str_X_Y(0, 2, "WORK:1--");
	BSP_LCD_Disp_Str_X_Y(13, 2, " 2--");
	BSP_LCD_Disp_Str_X_Y(22, 2, " 3--");
	BSP_LCD_Disp_Str_X_Y(31, 2, " 4--");
	
	//根据传递的参数num的低四位bit0--bit3来判断从机的链接状态
	// Check NO.1 CAN bus.
	if(0x01 & num)
	{
		BSP_LCD_Disp_Str_X_Y(8, 2, " NOK ");
	}
	else
	{
		BSP_LCD_Disp_Str_X_Y(8, 2, " OK  ");
	}
	// Check NO.2 CAN bus.
	if(0x02 & num)
	{
		BSP_LCD_Disp_Str_X_Y(17, 2, " NOK ");
	}
	else
	{
		BSP_LCD_Disp_Str_X_Y(17, 2, " OK  ");
	}
	// Check NO.3 CAN bus.
	if(0x04 & num)
	{
		BSP_LCD_Disp_Str_X_Y(26, 2, " NOK ");
	}
	else
	{
		BSP_LCD_Disp_Str_X_Y(26, 2, " OK  ");
	}
	// Check NO.4 CAN bus.
	if(0x08 & num)
	{
		BSP_LCD_Disp_Str_X_Y(35, 2, " NOK ");
	}
	else
	{
		BSP_LCD_Disp_Str_X_Y(35, 2, " OK  ");
	}
}


/*
*********************************************************************************************************
*                                        BSP_LCD_Disp_Current_Tempe(void)
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : Current and temperature.
*
*********************************************************************************************************
*/

void	BSP_LCD_Disp_Current_Tempe(void)
{
	uint32_t Temp_Vol;
	uint8_t  hun, deca, unit;
	
	/************** Display current **************/
	BSP_LCD_Disp_Str_X_Y(0, 1, "I: ");
	
	if(ChargeCurrentVal&0x8000)			//电流值首位（符号位）为1表示负值
	{
		BSP_LCD_Disp_Str_X_Y(3, 1, "-");
	}
	else
	{
		BSP_LCD_Disp_Str_X_Y(3, 1, "+");
	}
	
	Temp_Vol = (ChargeCurrentVal & 0x7FFF);	//取数值，去除符号位
	hun      = Temp_Vol / 100;
	Temp_Vol = Temp_Vol % 100;
	deca     = Temp_Vol / 10;
	unit     = Temp_Vol % 10;
	
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + hun);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + deca);
	BSP_LCD_Write(BSP_LCD_DAT, *("."));
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + unit);
	
	BSP_LCD_Disp_Str_X_Y(8, 1, " A   ");
	
	/************** Display temperature **************/
	BSP_LCD_Disp_Str_X_Y(13, 1, "T: ");
	
	if(Temperature&0x8000)
	{
		BSP_LCD_Disp_Str_X_Y(16, 1, "-");
	}
	else
	{
		BSP_LCD_Disp_Str_X_Y(16, 1, "+");
	}
	
	Temp_Vol = (Temperature & 0x7FFF);
	hun      = Temp_Vol / 100;
	Temp_Vol = Temp_Vol % 100;
	deca     = Temp_Vol / 10;
	unit     = Temp_Vol % 10;
	
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + hun);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + deca);
	BSP_LCD_Write(BSP_LCD_DAT, *("."));
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + unit);
	BSP_LCD_Disp_Str_X_Y(21, 1, " ");
	BSP_LCD_Set_X_Y(22,1);
	BSP_LCD_Write(BSP_LCD_DAT, 0xDF);
	BSP_LCD_Disp_Str_X_Y(23, 1, "C                ");
}


/*
*********************************************************************************************************
*                                        BSP_LCD_Disp_BAT_Info(void)
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : Display bettery information.
*
*********************************************************************************************************
*/

void	BSP_LCD_Disp_BAT_Info(void)
{
	int i;
	uint32_t Temp_Vol;
	uint8_t ths, hun, deca, unit;
	
	/************** Display max voltage **************/
	BSP_LCD_Disp_Str_X_Y(0, 1, "Max:");
	
	for (i=0;i<3;i++)			//取出最大的三个电压值，作为显示输出
	{
		Temp_Vol = BSP_BAT_Vol_Min_To_Max[i];
		
		ths      = Temp_Vol / 1000;
		Temp_Vol = Temp_Vol % 1000;
		hun      = Temp_Vol / 100;
		Temp_Vol = Temp_Vol % 100;
		deca     = Temp_Vol / 10;
		unit     = Temp_Vol % 10;
		
		BSP_LCD_Set_X_Y(4+9*i, 1);
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + (BSP_BAT_Vol_Min_To_Max_Num[i]+1) / 10);//显示电池编号
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + (BSP_BAT_Vol_Min_To_Max_Num[i]+1) % 10);
		BSP_LCD_Write(BSP_LCD_DAT, *("-"));
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + ths);		//显示电池电压数值
		BSP_LCD_Write(BSP_LCD_DAT, *("."));
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + hun);
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + deca);
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + unit);
		BSP_LCD_Write(BSP_LCD_DAT, *(" "));
	}
	
	/************* Display average voltage *************/
	BSP_LCD_Disp_Str_X_Y(31, 1, "Avg:");				//显示电压的平均值
	
	Temp_Vol = AverageVolVal;
	
	ths      = Temp_Vol / 1000;
	Temp_Vol = Temp_Vol % 1000;
	hun      = Temp_Vol / 100;
	Temp_Vol = Temp_Vol % 100;
	deca     = Temp_Vol / 10;
	unit     = Temp_Vol % 10;
	
	BSP_LCD_Set_X_Y(35, 1);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + ths);
	BSP_LCD_Write(BSP_LCD_DAT, *("."));
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + hun);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + deca);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + unit);
	
	/************** Display min voltage **************/
	BSP_LCD_Disp_Str_X_Y(0, 2, "Min:");
	
	for (i=0;i<3;i++)				//取出三个最小的电压值作为显示输出
	{
		Temp_Vol = BSP_BAT_Vol_Min_To_Max[23-i];
		
		ths      = Temp_Vol / 1000;
		Temp_Vol = Temp_Vol % 1000;
		hun      = Temp_Vol / 100;
		Temp_Vol = Temp_Vol % 100;
		deca     = Temp_Vol / 10;
		unit     = Temp_Vol % 10;
		
		BSP_LCD_Set_X_Y(4+9*i, 2);
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + (BSP_BAT_Vol_Min_To_Max_Num[23-i]+1) / 10);
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + (BSP_BAT_Vol_Min_To_Max_Num[23-i]+1) % 10);
		BSP_LCD_Write(BSP_LCD_DAT, *("-"));
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + ths);
		BSP_LCD_Write(BSP_LCD_DAT, *("."));
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + hun);
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + deca);
		BSP_LCD_Write(BSP_LCD_DAT, 0x30 + unit);
		BSP_LCD_Write(BSP_LCD_DAT, *(" "));
	}
	
	/************** Display vpp voltage **************/
	BSP_LCD_Disp_Str_X_Y(31, 2, "Vpp:");			//显示电压的峰值，最大与最小的差值
	
	Temp_Vol = MaxVolDiff;
	
	ths      = Temp_Vol / 1000;
	Temp_Vol = Temp_Vol % 1000;
	hun      = Temp_Vol / 100;
	Temp_Vol = Temp_Vol % 100;
	deca     = Temp_Vol / 10;
	unit     = Temp_Vol % 10;
	
	BSP_LCD_Set_X_Y(35, 2);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + ths);
	BSP_LCD_Write(BSP_LCD_DAT, *("."));
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + hun);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + deca);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + unit);
}

//显示配置参数界面，主要针对modbus address的设置
void	BSP_LCD_Disp_Config_Parameter(void)
{
	uint32_t Temp;
	uint8_t  ten_thou, thou, hun, deca, unit;
	
	// Display Modbus address
	BSP_LCD_Disp_Str_X_Y(0, 1, "MODBUS ADDRESS: ");
	BSP_LCD_Disp_Str_X_Y(21, 1, "IP: ");
	BSP_LCD_Disp_Str_X_Y(28, 1, ".");
	BSP_LCD_Disp_Str_X_Y(32, 1, ".");
	BSP_LCD_Disp_Str_X_Y(36, 1, ".");
	
	
//	
//	BSP_LCD_Disp_Val_X_Y(24, 1, lwipdev.ip[0]);
//	BSP_LCD_Disp_Val_X_Y(28, 1, lwipdev.ip[1]);
//	BSP_LCD_Disp_Val_X_Y(32, 1, lwipdev.ip[2]);
//	BSP_LCD_Disp_Val_X_Y(36, 1, lwipdev.ip[3]);
	

	BSP_LCD_Set_X_Y(16, 1);
	Temp = BKP_ReadBackupRegister(SlaveAddress);
	hun  = Temp / 100;
	Temp = Temp % 100;
	deca = Temp / 10;
	unit = Temp % 10;
	
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + hun);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + deca);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + unit);

	
	BSP_LCD_Set_X_Y(25, 1);
	Temp = lwipdev.ip[0];
	hun  = Temp / 100;
	Temp = Temp % 100;
	deca = Temp / 10;
	unit = Temp % 10;
	
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + hun);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + deca);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + unit);
	
	BSP_LCD_Set_X_Y(29, 1);
	Temp = lwipdev.ip[1];
	hun  = Temp / 100;
	Temp = Temp % 100;
	deca = Temp / 10;
	unit = Temp % 10;
	
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + hun);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + deca);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + unit);
	
	BSP_LCD_Set_X_Y(33, 1);
	Temp = lwipdev.ip[2];
	hun  = Temp / 100;
	Temp = Temp % 100;
	deca = Temp / 10;
	unit = Temp % 10;
	
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + hun);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + deca);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + unit);
	
	BSP_LCD_Set_X_Y(37, 1);
	Temp = lwipdev.ip[3];
	hun  = Temp / 100;
	Temp = Temp % 100;
	deca = Temp / 10;
	unit = Temp % 10;
	
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + hun);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + deca);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + unit);
	
	
	// Display Modbus baudrate
	BSP_LCD_Disp_Str_X_Y(0, 2, "MODBUS BAUDSRATE: ");
	BSP_LCD_Set_X_Y(18, 2);
	
	Temp = BKP_ReadBackupRegister(TransBaudRate);
	ten_thou = Temp / 10000;
	Temp %= 10000;
	thou = Temp / 1000;
	Temp %= 1000;
	hun  = Temp / 100;
	Temp %= 100;
	deca = Temp / 10;
	unit = Temp % 10;
	
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + ten_thou);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + thou);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + hun);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + deca);
	BSP_LCD_Write(BSP_LCD_DAT, 0x30 + unit);
}

