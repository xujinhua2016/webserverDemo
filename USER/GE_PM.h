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

#define D_VALUE 10		//�����жϣ�����Ҫ��ƽ����ѹֵСD_VALUE��������

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


//��������־����Ҫ�����豸����ʱ��������;��ⲻ����ͬʱ�������ж�
__IO  uint8_t   BAT_Vol_Res_Dect_Flag = 0;		//������Կ��صı�־��
__IO  uint8_t   BSP_BAT_Balan_Flag = 0;				//��ؾ��⿪�صı�־��

__IO	uint8_t	 BSP_BAT_CAN_BAD_Num = 0xFF;	//��ʼ���ã��ĸ��ӻ����ܹ��ҵ���û�жϿ�������


__IO  uint16_t NoConvBatVol[24];		//�����ֵ��δ������׼��ѹת��
__IO  uint16_t NoConvBatRes[24];		//�����ֵ��δ�������ص���ת��

__IO	uint32_t  BSP_BAT_Vol_Min_To_Max[24];		 //the value of battery voltage from minimum to maximum
__IO	uint8_t	  BSP_BAT_Vol_Min_To_Max_Num[24];//the number of battery voltage from minimum to maximum

__IO  uint32_t  BSP_BAT_Res_Min_To_Max[24];		 //the value of battery resistance from minimum to maximum
__IO	uint8_t	  BSP_BAT_Res_Min_To_Max_Num[24];//the number of battery resistance from minimum to maximum


__IO	uint8_t	 BSP_LCD_DISP_DATA_TYPE_Flag = 4;			//����������ʾ��־��1.��ѹ���� 2.�������� 3.��ʾ��ص�ѹ���������ֵ��ƽ��ֵ���������ֵ����ֵ
																										//4��Ĭ�ϳ�ʼֵ����ʾ�����Ļ����¶ȣ������������Լ��Ͽ��ӻ�ҳ�� 6.modbus address ����ģʽ
__IO	uint8_t	 BSP_LCD_PAGE_Disp_flag = 1;					//�����л���ʾ�ı�־��1.��һҳ(1-8)��2.�ڶ�ҳ(9-16)��3.����ҳ(17-24)



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

static void BSP_getVol(void);		//����ֱ��ѭ�������ݼ���ص�ѹ������
static void BSP_getRes(void);		//�������������ǰ��һ��Ҫ�жϵ���Ƿ��������Ĺ�����Χ��
static void BSP_sendBala(void);	//����ָ�������Ҫ���������ж�
static void BSP_getI_T(void);		//��õ������¶�ֵ

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
