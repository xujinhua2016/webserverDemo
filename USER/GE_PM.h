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
* Filename      : bsp_can.h
* Version       : V1.00
* Programmer(s) : ENUT
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

#ifndef __GE_PM_H
#define	__GE_PM_H


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

#include  "stm32f4xx.h"


/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#define D_VALUE 10		//均衡判断，必须要比平均电压值小D_VALUE才做均衡

#define REF_RES 35
#define REF_DAC1V 1131

#define SLAVE_VREF1  2498
#define SLAVE_VREF2	 2498
#define SLAVE_VREF3  2499
#define SLAVE_VREF4	 2497

__IO  uint32_t  BSP_CAN_A_ID		= 0x0100;			//slave 1 CAN_ID
__IO  uint32_t  BSP_CAN_B_ID		= 0x0155;			//slave 2 CAN_ID
__IO  uint32_t  BSP_CAN_C_ID		= 0x01AA;			//slave 3 CAN_ID
__IO  uint32_t  BSP_CAN_D_ID		= 0x01FF;			//slave 4 CAN_ID
__IO  uint32_t  BSP_CAN_E_ID		= 0x5111;			//current and temperature CAN_ID


__IO  uint8_t  BSP_CAN_TimOut	= 0;


//这两个标志量主要用于设备工作时，内阻检测和均衡不可以同时开启的判断
__IO  uint8_t   BAT_Vol_Res_Dect_Flag = 0;		//内阻测试开关的标志量
__IO  uint8_t   BSP_BAT_Balan_Flag = 0;				//电池均衡开关的标志量

__IO	uint8_t	 BSP_BAT_CAN_BAD_Num = 0xFF;	//初始设置，四个从机都能够找到，没有断开的现象


__IO  uint16_t NoConvBatVol[24];		//保存的值，未经过基准电压转换
__IO  uint16_t NoConvBatRes[24];		//保存的值，未经过板载电阻转换

__IO	uint32_t  BSP_BAT_Vol_Min_To_Max[24];		 //the value of battery voltage from minimum to maximum
__IO	uint8_t	  BSP_BAT_Vol_Min_To_Max_Num[24];//the number of battery voltage from minimum to maximum

__IO  uint32_t  BSP_BAT_Res_Min_To_Max[24];		 //the value of battery resistance from minimum to maximum
__IO	uint8_t	  BSP_BAT_Res_Min_To_Max_Num[24];//the number of battery resistance from minimum to maximum


__IO	uint8_t	 BSP_LCD_DISP_DATA_TYPE_Flag = 4;			//各种数据显示标志，1.电压数据 2.内阻数据 3.显示电池电压的三个最高值，平均值，三个最低值，峰值
																										//4（默认初始值）显示电池组的环境温度，环境电流，以及断开从机页面 6.modbus address 配置模式
__IO	uint8_t	 BSP_LCD_PAGE_Disp_flag = 1;					//界面切换显示的标志，1.第一页(1-8)，2.第二页(9-16)，3.第三页(17-24)



/*
*********************************************************************************************************
*                                          EXTERN DEFINES
*********************************************************************************************************
*/

extern  CanTxMsg  TxMessage;			//the structure CAN transmit data
extern  CanRxMsg  RxMessage;			//the structure CAN receive data
 
extern  __IO  uint8_t  BSP_CAN_Rx_Flag;			//CAN receive data flag 


extern __IO uint8_t BSP_LCD_ON_TIME;


/*
*********************************************************************************************************
*                                         EXTERN FUNCTION PROTOTYPES
*********************************************************************************************************
*/

extern  void BSP_CAN_SetTxMsg(uint32_t ID, uint8_t cmd);

/*
*********************************************************************************************************
*                                     LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static uint8_t BSP_findRecDataCmd(uint32_t ID,uint8_t cmd);
static void  BSP_CAN_Detec(void);

/**********get the slave battery voltage 1-24**********/
static void BSP_getBatV(int num);
/****get the slave battery internal resistance 1-24****/
static void BSP_getBatR(int num);
/*********send the slave balance command 1-24*********/
static void BSP_sendBatB(int num);

static void BSP_getVol(void);		//可以直接循环，传递检测电池电压的命令
static void BSP_getRes(void);		//传输电池内阻测量前，一定要判断电池是否处于正常的工作范围内
static void BSP_sendBala(void);	//均衡指令开启，需要进行条件判断
static void BSP_getI_T(void);		//获得电流与温度值

static void  BSP_Voltage_Proc(void);
static void  BSP_Res_Proc(void);
static void  BSP_I_T_Proc(void);


void  BSP_Vol_Res_Bala(void);
void BSP_Key_Proc(void);
void BSP_LCD_Proc(void);
/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/


#endif                            /* End of module include.*/
