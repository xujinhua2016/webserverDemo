/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                      POWER MANAGEMENT PRODUCT
*
* Filename      : bsp_mem.h
* Version       : V1.00
* Programmer(s) : JOSEPH&XU
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*
*********************************************************************************************************
*/

#ifndef __BSP_MEM_H
#define	__BSP_MEM_H


/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include  "stm32f4xx.h"
#include	"stdint.h"


/*
*********************************************************************************************************
*                                          DEFINITIONS
*********************************************************************************************************
*/

#define  GE_BAT_BASE         ((uint32_t)0x20001000)

//#define  BSP_BAT_PARAM_BASE	 ((uint32_t)0x40006C00)


/**************************  Battery voltage register **************************/
extern volatile	uint32_t	GE_BAT_Vol[192];

/********  单体电压警告，使用低16位，电池出现电压警告，相应数据位置1  **********/
extern volatile uint32_t 	BatVolWarnBit000_015;	// #000~#015号电池
extern volatile uint32_t 	BatVolWarnBit016_031;
extern volatile uint32_t 	BatVolWarnBit032_047;
extern volatile uint32_t 	BatVolWarnBit048_063;
extern volatile uint32_t 	BatVolWarnBit064_079;
extern volatile uint32_t 	BatVolWarnBit080_095;
extern volatile uint32_t 	BatVolWarnBit096_111;
extern volatile uint32_t 	BatVolWarnBit112_127;
extern volatile uint32_t 	BatVolWarnBit128_143;
extern volatile uint32_t 	BatVolWarnBit144_159;
extern volatile uint32_t 	BatVolWarnBit160_175;
extern volatile uint32_t	BatVolWarnBit176_191;

/***************  单体电池内阻电池编号#0~#191，基地址0x20001400  ***************/
extern volatile	uint32_t	BatIntRes[192];

/*******  单体电池内阻警告，使用低16位，电池出现电压警告，相应数据位置1  *******/
extern volatile uint32_t	BatResWarnBit000_015;	// #000~#015号电池
extern volatile uint32_t 	BatResWarnBit016_031;
extern volatile uint32_t 	BatResWarnBit032_047;
extern volatile uint32_t 	BatResWarnBit048_063;
extern volatile uint32_t 	BatResWarnBit064_079;
extern volatile uint32_t 	BatResWarnBit080_095;
extern volatile uint32_t 	BatResWarnBit096_111;
extern volatile uint32_t 	BatResWarnBit112_127;
extern volatile uint32_t 	BatResWarnBit128_143;
extern volatile uint32_t 	BatResWarnBit144_159;
extern volatile uint32_t 	BatResWarnBit160_175;
extern volatile uint32_t 	BatResWarnBit176_191;

/*******************************  整组电池参数值  ******************************/
extern volatile	uint32_t	TotalBatVol;      // 整组电压
extern volatile	uint32_t	ChargeCurrentVal; // 充放电电流
extern volatile	uint32_t	FloatCurrentVal;	// 浮充电流
extern volatile	uint32_t	Temperature;	    // 环境温度
extern volatile	uint32_t	HigVolBatNum1;    // 最高电压1电池编号
extern volatile	uint32_t	HigVolBatVal1;    // 最高电压1电池电压
extern volatile	uint32_t	HigVolBatNum2;    // 最高电压2电池编号
extern volatile	uint32_t	HigVolBatVal2;    // 最高电压2电池电压
extern volatile	uint32_t	HigVolBatNum3;	  // 最高电压3电池编号
extern volatile	uint32_t	HigVolBatVal3;	  // 最高电压3电池电压
extern volatile	uint32_t	LowVolBatNum1;	  // 最低电压1电池编号
extern volatile	uint32_t	LowVolBatVal1;	  // 最低电压1电池电压
extern volatile	uint32_t	LowVolBatNum2;	  // 最低电压2电池编号
extern volatile	uint32_t	LowVolBatVal2;	  // 最低电压2电池电压
extern volatile	uint32_t	LowVolBatNum3;	  // 最低电压3电池编号
extern volatile	uint32_t	LowVolBatVal3;	  // 最低电压3电池电压
extern volatile	uint32_t	MaxResBatNum;   	// 最大内阻电池编号
extern volatile	uint32_t	MaxResBatVal;    	// 最大内阻电池阻值
extern volatile	uint32_t	AverageVolVal;	  // 平均电压值
extern volatile	uint32_t	MaxVolDiff;	      // 最大电压差值

extern volatile	uint32_t	ReadTheClkYear;	  // 读取时钟年
extern volatile	uint32_t	ReadTheClkMonth;  // 读取时钟月
extern volatile	uint32_t	ReadTheClkDay;	  // 读取时钟日
extern volatile	uint32_t	ReadTheClkHour;	  // 读取时钟时
extern volatile	uint32_t	ReadTheClkMinu;	  // 读取时钟分
extern volatile	uint32_t	ReadTheClkSeco;  	// 读取时钟秒

extern volatile	uint32_t  GroBatWarn;	      // 整组电池警告

extern volatile	uint32_t	BatSOC;	          // 电池剩余电量SOC

/*********************  参数配置寄存器BKP2，考虑掉电保持   *********************/
//extern volatile	uint32_t	SlaveAddress;           // 从机地址
//extern volatile	uint32_t	ConnectLineRes;         // 连接线电阻
//extern volatile	uint32_t	TransBaudRate;          // 传输波特率
//extern volatile	uint32_t	TotalNumBat;            // 电池总数
//extern volatile	uint32_t	MaxWarnRes;             // 最高告警内阻
//extern volatile	uint32_t	HigWarnVolSinBat;       // 最高告警单电池电压
//extern volatile	uint32_t	LowWarnVolSinBat;	      // 最低告警单电池电压
//extern volatile	uint32_t	MaxWarnVolDiff;       	// 最大告警电压差
//extern volatile	uint32_t	MaxWarnChargeCurVal;	  // 最大告警充电电流
//extern volatile	uint32_t	MaxWarnDischargeCurVal; // 最大告警放电电流
//extern volatile	uint32_t	HigWarnTemperature;	    // 最高告警环境温度
//extern volatile	uint32_t	LowWarnTemperature;    	// 最低告警环境温度
//extern volatile	uint32_t	RTC_Year;             	// RTC年
//extern volatile	uint32_t	RTC_Moth;	             	// RTC月
//extern volatile	uint32_t	RTC_Day;	             	// RTC日
//extern volatile	uint32_t	RTC_Hour;	             	// RTC时
//extern volatile	uint32_t	RTC_Min;	             	// RTC分
//extern volatile	uint32_t	RTC_Sec;             		// RTC秒

//extern volatile	uint32_t	BatCapacity;	        	// 电池容量
//extern volatile	uint32_t	BKPBatSOC;	          	// 电池SOC


#define	SlaveAddress           0x0000//__attribute__((at(BKPSRAM_BASE + 0x0000))); // 从机地址
#define	ConnectLineRes         0x0004//__attribute__((at(BKPSRAM_BASE + 0x0004))); // 连接线电阻
#define	TransBaudRate          0x0008 //__attribute__((at(BKPSRAM_BASE + 0x0008)));	// 传输波特率
#define	TotalNumBat            0x000C //__attribute__((at(BKPSRAM_BASE + 0x000C)));	// 电池总数
#define	MaxWarnRes             0x0010 //__attribute__((at(BKPSRAM_BASE + 0x0010))); // 最高告警内阻
#define	HigWarnVolSinBat       0x0014 //__attribute__((at(BKPSRAM_BASE + 0x0014))); // 最高告警单电池电压
#define	LowWarnVolSinBat       0x0018 //__attribute__((at(BKPSRAM_BASE + 0x0018)));	// 最低告警单电池电压
#define	MaxWarnVolDiff         0x001C //__attribute__((at(BKPSRAM_BASE + 0x001C)));	// 最大告警电压差
#define	MaxWarnChargeCurVal    0x0020 //__attribute__((at(BKPSRAM_BASE + 0x0020)));	// 最大告警充电电流
#define	MaxWarnDischargeCurVal 0x0024 //__attribute__((at(BKPSRAM_BASE + 0x0024))); // 最大告警放电电流
#define	HigWarnTemperature     0x0028 //__attribute__((at(BKPSRAM_BASE + 0x0028)));	// 最高告警环境温度
#define	LowWarnTemperature     0x002C //__attribute__((at(BKPSRAM_BASE + 0x002C)));	// 最低告警环境温度
#define	RTC_Year               0x0030 //__attribute__((at(BKPSRAM_BASE + 0x0030)));	// RTC年
#define	RTC_Moth               0x0034 //__attribute__((at(BKPSRAM_BASE + 0x0034)));	// RTC月
#define	RTC_Day                0x0038 //__attribute__((at(BKPSRAM_BASE + 0x0038)));	// RTC日
#define	RTC_Hour               0x003C //__attribute__((at(BKPSRAM_BASE + 0x003C)));	// RTC时
#define	RTC_Min                0x0040 //__attribute__((at(BKPSRAM_BASE + 0x0040)));	// RTC分
#define	RTC_Sec                0x0044 //__attribute__((at(BKPSRAM_BASE + 0x0044)));	// RTC秒

#define	BatCapacity            0x0048 //__attribute__((at(BKPSRAM_BASE + 0x0048)));	// 电池容量
#define	BKPBatSOC              0x004C //__attribute__((at(BKPSRAM_BASE + 0x004C)));	// 电池SOC

#define	BKP_Flag               0x0050 //__attribute__((at(BKPSRAM_BASE + 0x0050)));	// 备份寄存器重写标志寄存器




/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void	BSP_BAT_DATA_MEM_Init (void);

void	BSP_BatVol_Init       (void);
void	BSP_BatVolWarnBit_Init(void);
void	BSP_BatIntRes_Init    (void);
void	BSP_BatResWarnBit_Init(void);
void	BSP_BatParamConf_Init (void);
void	BSP_BKPSRAM_Init      (void);

void BKP_WriteBackupRegister(uint16_t BKP_DR, uint16_t Data);
uint16_t BKP_ReadBackupRegister(uint16_t BKP_DR);
	
/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/


#endif  

