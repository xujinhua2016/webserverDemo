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
* Filename      : bsp_mem.c
* Version       : V1.00
* Programmer(s) : Joseph & Xu
* Notes         : 1  V1.00：变量存储位置已经设置好，备份寄存器可以正常掉电存储数据
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  "bsp_mem.h"


/*
*********************************************************************************************************
*                                            VARIABLES
*********************************************************************************************************
*/

uint8_t BKP_Write_Flag = 0xA4; // 备份寄存器重写标志

/**************************  Battery voltage register **************************/
/***************  单体电池电压电池编号#0~#191，基地址0x20001000  ***************/
volatile uint32_t	  GE_BAT_Vol[192]				  __attribute__((at(GE_BAT_BASE)));

/********  单体电压警告，使用低16位，电池出现电压警告，相应数据位置1  **********/
volatile uint32_t 	BatVolWarnBit000_015		__attribute__((at(GE_BAT_BASE + 0x0300)));	// #000~#015号电池
volatile uint32_t 	BatVolWarnBit016_031		__attribute__((at(GE_BAT_BASE + 0x0304)));

volatile uint32_t 	BatVolWarnBit032_047		__attribute__((at(GE_BAT_BASE + 0x0308)));
volatile uint32_t 	BatVolWarnBit048_063		__attribute__((at(GE_BAT_BASE + 0x030C)));
volatile uint32_t 	BatVolWarnBit064_079		__attribute__((at(GE_BAT_BASE + 0x0310)));
volatile uint32_t 	BatVolWarnBit080_095		__attribute__((at(GE_BAT_BASE + 0x0314)));
volatile uint32_t 	BatVolWarnBit096_111		__attribute__((at(GE_BAT_BASE + 0x0318)));
volatile uint32_t 	BatVolWarnBit112_127		__attribute__((at(GE_BAT_BASE + 0x031C)));
volatile uint32_t 	BatVolWarnBit128_143		__attribute__((at(GE_BAT_BASE + 0x0320)));
volatile uint32_t 	BatVolWarnBit144_159		__attribute__((at(GE_BAT_BASE + 0x0324)));
volatile uint32_t 	BatVolWarnBit160_175		__attribute__((at(GE_BAT_BASE + 0x0328)));
volatile uint32_t	  BatVolWarnBit176_191	  __attribute__((at(GE_BAT_BASE + 0x032C)));

/***************  单体电池内阻电池编号#0~#191，基地址0x20001400  ***************/
volatile	uint32_t	BatIntRes[192]					__attribute__((at(GE_BAT_BASE + 0x0400)));

/*******  单体电池内阻警告，使用低16位，电池出现内阻警告，相应数据位置1  *******/
volatile 	uint32_t	BatResWarnBit000_015		__attribute__((at(GE_BAT_BASE + 0x0700)));	// #000~#015号电池
volatile 	uint32_t 	BatResWarnBit016_031		__attribute__((at(GE_BAT_BASE + 0x0704)));

volatile 	uint32_t 	BatResWarnBit032_047		__attribute__((at(GE_BAT_BASE + 0x0708)));
volatile 	uint32_t 	BatResWarnBit048_063		__attribute__((at(GE_BAT_BASE + 0x070C)));
volatile 	uint32_t 	BatResWarnBit064_079		__attribute__((at(GE_BAT_BASE + 0x0710)));
volatile 	uint32_t 	BatResWarnBit080_095		__attribute__((at(GE_BAT_BASE + 0x0714)));
volatile 	uint32_t 	BatResWarnBit096_111		__attribute__((at(GE_BAT_BASE + 0x0718)));
volatile 	uint32_t 	BatResWarnBit112_127		__attribute__((at(GE_BAT_BASE + 0x071C)));
volatile 	uint32_t 	BatResWarnBit128_143		__attribute__((at(GE_BAT_BASE + 0x0720)));
volatile 	uint32_t 	BatResWarnBit144_159		__attribute__((at(GE_BAT_BASE + 0x0724)));
volatile 	uint32_t 	BatResWarnBit160_175		__attribute__((at(GE_BAT_BASE + 0x0728)));
volatile 	uint32_t 	BatResWarnBit176_191		__attribute__((at(GE_BAT_BASE + 0x072C)));

/*******************************  整组电池参数值  ******************************/
volatile	uint32_t	TotalBatVol							__attribute__((at(GE_BAT_BASE + 0x0800)));	// 整组电压
volatile	uint32_t	ChargeCurrentVal				__attribute__((at(GE_BAT_BASE + 0x0804)));	// 充放电电流
volatile	uint32_t	FloatCurrentVal					__attribute__((at(GE_BAT_BASE + 0x0808)));	// 浮充电流
volatile	uint32_t	Temperature							__attribute__((at(GE_BAT_BASE + 0x080C)));	// 环境温度
volatile	uint32_t	HigVolBatNum1						__attribute__((at(GE_BAT_BASE + 0x0810)));	// 最高电压1电池编号
volatile	uint32_t	HigVolBatVal1						__attribute__((at(GE_BAT_BASE + 0x0814)));	// 最高电压1电池电压
volatile	uint32_t	HigVolBatNum2						__attribute__((at(GE_BAT_BASE + 0x0818)));	// 最高电压2电池编号
volatile	uint32_t	HigVolBatVal2						__attribute__((at(GE_BAT_BASE + 0x081C)));	// 最高电压2电池电压
volatile	uint32_t	HigVolBatNum3						__attribute__((at(GE_BAT_BASE + 0x0820)));	// 最高电压3电池编号
volatile	uint32_t	HigVolBatVal3						__attribute__((at(GE_BAT_BASE + 0x0824)));	// 最高电压3电池电压
volatile	uint32_t	LowVolBatNum1						__attribute__((at(GE_BAT_BASE + 0x0828)));	// 最低电压1电池编号
volatile	uint32_t	LowVolBatVal1						__attribute__((at(GE_BAT_BASE + 0x082C)));	// 最低电压1电池电压
volatile	uint32_t	LowVolBatNum2						__attribute__((at(GE_BAT_BASE + 0x0830)));	// 最低电压2电池编号
volatile	uint32_t	LowVolBatVal2						__attribute__((at(GE_BAT_BASE + 0x0834)));	// 最低电压2电池电压
volatile	uint32_t	LowVolBatNum3						__attribute__((at(GE_BAT_BASE + 0x0838)));	// 最低电压3电池编号
volatile	uint32_t	LowVolBatVal3						__attribute__((at(GE_BAT_BASE + 0x083C)));	// 最低电压3电池电压
volatile	uint32_t	MaxResBatNum						__attribute__((at(GE_BAT_BASE + 0x0840)));	// 最大内阻电池编号
volatile	uint32_t	MaxResBatVal						__attribute__((at(GE_BAT_BASE + 0x0844)));	// 最大内阻电池阻值
volatile	uint32_t	AverageVolVal						__attribute__((at(GE_BAT_BASE + 0x0848)));	// 平均电压值
volatile	uint32_t	MaxVolDiff							__attribute__((at(GE_BAT_BASE + 0x084C)));	// 最大电压差值

volatile	uint32_t	ReadTheClkYear					__attribute__((at(GE_BAT_BASE + 0x0850)));	// 读取时钟年
volatile	uint32_t	ReadTheClkMonth					__attribute__((at(GE_BAT_BASE + 0x0854)));	// 读取时钟月
volatile	uint32_t	ReadTheClkDay						__attribute__((at(GE_BAT_BASE + 0x0858)));	// 读取时钟日
volatile	uint32_t	ReadTheClkHour					__attribute__((at(GE_BAT_BASE + 0x085C)));	// 读取时钟时
volatile	uint32_t	ReadTheClkMinu					__attribute__((at(GE_BAT_BASE + 0x0860)));	// 读取时钟分
volatile	uint32_t	ReadTheClkSeco					__attribute__((at(GE_BAT_BASE + 0x0864)));	// 读取时钟秒

volatile	uint32_t	GroBatWarn							__attribute__((at(GE_BAT_BASE + 0x0868)));	// 整组电池警告

volatile	uint32_t	BatSOC									__attribute__((at(GE_BAT_BASE + 0x086C)));	// 电池剩余电量SOC

/*****************************  整组电池备份参数值  ****************************/
// 基地址：0x40024000 偏移地址：0x0004
//volatile	uint32_t	SlaveAddress           0x0000; //__attribute__((at(BKPSRAM_BASE + 0x0000))); // 从机地址
//volatile	uint32_t	ConnectLineRes          //__attribute__((at(BKPSRAM_BASE + 0x0004))); // 连接线电阻
//volatile	uint32_t	TransBaudRate           //__attribute__((at(BKPSRAM_BASE + 0x0008)));	// 传输波特率
//volatile	uint32_t	TotalNumBat             //__attribute__((at(BKPSRAM_BASE + 0x000C)));	// 电池总数
//volatile	uint32_t	MaxWarnRes              //__attribute__((at(BKPSRAM_BASE + 0x0010))); // 最高告警内阻
//volatile	uint32_t	HigWarnVolSinBat        //__attribute__((at(BKPSRAM_BASE + 0x0014))); // 最高告警单电池电压
//volatile	uint32_t	LowWarnVolSinBat        //__attribute__((at(BKPSRAM_BASE + 0x0018)));	// 最低告警单电池电压
//volatile	uint32_t	MaxWarnVolDiff          //__attribute__((at(BKPSRAM_BASE + 0x001C)));	// 最大告警电压差
//volatile	uint32_t	MaxWarnChargeCurVal     //__attribute__((at(BKPSRAM_BASE + 0x0020)));	// 最大告警充电电流
//volatile	uint32_t	MaxWarnDischargeCurVal  //__attribute__((at(BKPSRAM_BASE + 0x0024))); // 最大告警放电电流
//volatile	uint32_t	HigWarnTemperature      //__attribute__((at(BKPSRAM_BASE + 0x0028)));	// 最高告警环境温度
//volatile	uint32_t	LowWarnTemperature      //__attribute__((at(BKPSRAM_BASE + 0x002C)));	// 最低告警环境温度
//volatile	uint32_t	RTC_Year                //__attribute__((at(BKPSRAM_BASE + 0x0030)));	// RTC年
//volatile	uint32_t	RTC_Moth                //__attribute__((at(BKPSRAM_BASE + 0x0034)));	// RTC月
//volatile	uint32_t	RTC_Day                 //__attribute__((at(BKPSRAM_BASE + 0x0038)));	// RTC日
//volatile	uint32_t	RTC_Hour                //__attribute__((at(BKPSRAM_BASE + 0x003C)));	// RTC时
//volatile	uint32_t	RTC_Min                 //__attribute__((at(BKPSRAM_BASE + 0x0040)));	// RTC分
//volatile	uint32_t	RTC_Sec                 //__attribute__((at(BKPSRAM_BASE + 0x0044)));	// RTC秒

//volatile	uint32_t	BatCapacity             //__attribute__((at(BKPSRAM_BASE + 0x0048)));	// 电池容量
//volatile	uint32_t	BKPBatSOC               //__attribute__((at(BKPSRAM_BASE + 0x004C)));	// 电池SOC

//volatile	uint32_t	BKP_Flag                //__attribute__((at(BKPSRAM_BASE + 0x0050)));	// 备份寄存器重写标志寄存器


/////////////////////////////



void  BSP_BAT_DATA_MEM_Init(void)
{
	BSP_BatVol_Init();
	BSP_BatVolWarnBit_Init();
	
	BSP_BatIntRes_Init();
	BSP_BatResWarnBit_Init();
	
	BSP_BatParamConf_Init();
	
	BSP_BKPSRAM_Init();
}


void	BSP_BatVol_Init(void)
{
	int i;
	
	for (i=0;i<192;i++)
	{
		GE_BAT_Vol[i] = 18;
	}
}



void	BSP_BatVolWarnBit_Init(void)
{
	BatVolWarnBit000_015 = 0;
	BatVolWarnBit016_031 = 0;
	
	BatVolWarnBit032_047 = 0;
	BatVolWarnBit048_063 = 0;
	BatVolWarnBit064_079 = 0;
	BatVolWarnBit080_095 = 0;
	BatVolWarnBit096_111 = 0;
	BatVolWarnBit112_127 = 0;
	BatVolWarnBit128_143 = 0;
	BatVolWarnBit144_159 = 0;
	BatVolWarnBit160_175 = 0;
	BatVolWarnBit176_191 = 0;
}


void	BSP_BatIntRes_Init(void)
{
	int i;
	
	for(i=0;i<192;i++)
	{
		BatIntRes[i] = 0;
	}
}


void	BSP_BatResWarnBit_Init(void)
{
	BatResWarnBit000_015 = 0;
	BatResWarnBit016_031 = 0;
	
	BatResWarnBit032_047 = 0;
	BatResWarnBit048_063 = 0;
	BatResWarnBit064_079 = 0;
	BatResWarnBit080_095 = 0;
	BatResWarnBit096_111 = 0;
	BatResWarnBit112_127 = 0;
	BatResWarnBit128_143 = 0;
	BatResWarnBit144_159 = 0;
	BatResWarnBit160_175 = 0;
	BatResWarnBit176_191 = 0;
}


void	BSP_BatParamConf_Init(void)
{
	TotalBatVol      = 0; // 整组电压
	ChargeCurrentVal = 0; // 充放电电流
	FloatCurrentVal  = 0; // 浮充电流
	Temperature      = 0; // 环境温度
  
	HigVolBatNum1		 = 0;	// 最高电压1电池编号
  HigVolBatVal1		 = 0;	// 最高电压1电池电压
  HigVolBatNum2		 = 0; // 最高电压2电池编号
  HigVolBatVal2		 = 0; // 最高电压2电池电压
  HigVolBatNum3		 = 0; // 最高电压3电池编号
  HigVolBatVal3		 = 0;	// 最高电压3电池电压
  LowVolBatNum1		 = 0;	// 最低电压1电池编号
  LowVolBatVal1		 = 0;	// 最低电压1电池电压
  LowVolBatNum2		 = 0;	// 最低电压2电池编号
  LowVolBatVal2		 = 0;	// 最低电压2电池电压
  LowVolBatNum3		 = 0;	// 最低电压3电池编号
  LowVolBatVal3		 = 0;	// 最低电压3电池电压
	
	MaxResBatNum     = 0; // 最大内阻电池编号
	MaxResBatVal     = 0; // 最大内阻电池阻值
	
	AverageVolVal    = 0; // 平均电压值
	MaxVolDiff       = 0; // 最大电压差值
	
	ReadTheClkYear   = 0; // 读取时钟年
	ReadTheClkMonth  = 0; // 读取时钟月
	ReadTheClkDay    = 0; // 读取时钟日
	ReadTheClkHour   = 0; // 读取时钟时
	ReadTheClkMinu   = 0; // 读取时钟分
	ReadTheClkSeco   = 0; // 读取时钟秒
	
	GroBatWarn       = 0; // 整组电池警告
	
	BatSOC           = 0; // 电池剩余电量SOC
}


void	BSP_BKPSRAM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	/* 使能 BKPRAM 时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM,ENABLE);
	/* 启用备份SRAM的低功耗稳压器，在VBAT模式仍保持其内*/
	PWR_BackupRegulatorCmd(ENABLE);
	/* 等待直到备份SRAM的低功耗稳压器就绪 */
	while(PWR_GetFlagStatus(PWR_FLAG_BRR) == RESET)
	{
	}
//	
//	if(BKP_Flag != BKP_Write_Flag)
//	{
//		SlaveAddress           = 1;     // 从机地址
//		ConnectLineRes         = 1;     // 连接线电阻
//		TransBaudRate          = 19200; // 传输波特率
//		TotalNumBat            = 24;    // 电池总数
//		MaxWarnRes             = 15;    // 最高告警内阻
//		HigWarnVolSinBat       = 2300;  // 最高告警单电池电压
//		LowWarnVolSinBat       = 1800;  // 最低告警单电池电压
//		MaxWarnVolDiff         = 1800;  // 最大告警电压差
//		MaxWarnChargeCurVal    = 500;	  // 最大告警充电电流
//		MaxWarnDischargeCurVal = 200;   // 最大告警放电电流
//		HigWarnTemperature     = 33468; // 最高告警环境温度
//		LowWarnTemperature     = 400;   // 最低告警环境温度
//		
//		RTC_Year               = 0;     // RTC年
//		RTC_Moth               = 0;     // RTC月
//		RTC_Day                = 0;     // RTC日
//		RTC_Hour               = 0;     // RTC时
//		RTC_Min                = 0;     // RTC分
//		RTC_Sec                = 0;     // RTC秒
//		
//		BatCapacity            = 0;     // 电池容量
//		BKPBatSOC              = 0;     // 电池SOC
//		
//		BKP_Flag = BKP_Write_Flag;      // 写标志位
//	}

	
	//为了避免重复初始化，所以在此处设置一个标志量来作为判断
	if(BKP_ReadBackupRegister(BKP_Flag) != BKP_Write_Flag)
	{
		BKP_WriteBackupRegister(SlaveAddress,             1); //从机地址
		BKP_WriteBackupRegister(ConnectLineRes,           1); //连接线电阻
		BKP_WriteBackupRegister(TransBaudRate,        19200); //传输波特率
		BKP_WriteBackupRegister(TotalNumBat,             24); //电池总数
		BKP_WriteBackupRegister(MaxWarnRes,              15); //最高告警内阻
		BKP_WriteBackupRegister(HigWarnVolSinBat,      2300); //最高告警单电池电压
		BKP_WriteBackupRegister(LowWarnVolSinBat,      1800); //最低告警单电池电压
		BKP_WriteBackupRegister(MaxWarnVolDiff,        1800); //最大告警电压差
		BKP_WriteBackupRegister(MaxWarnChargeCurVal,    500); //最大告警充电电流
		BKP_WriteBackupRegister(MaxWarnDischargeCurVal, 200);	//最大告警放电电流
		BKP_WriteBackupRegister(HigWarnTemperature,   33468); //最高告警环境温度
		BKP_WriteBackupRegister(LowWarnTemperature,     400); //最低告警环境温度
		
		BKP_WriteBackupRegister(RTC_Year,               0); // RTC年
		BKP_WriteBackupRegister(RTC_Moth,               0); // RTC月
		BKP_WriteBackupRegister(RTC_Day,                0); // RTC日
		BKP_WriteBackupRegister(RTC_Hour,               0); // RTC时
		BKP_WriteBackupRegister(RTC_Min,                0); // RTC分
		BKP_WriteBackupRegister(RTC_Sec,                0); // RTC秒
		
		BKP_WriteBackupRegister(BatCapacity,            0); //电池容量
		BKP_WriteBackupRegister(BKPBatSOC,              0); //电池SOC
		
		BKP_WriteBackupRegister(BKP_Flag, BKP_Write_Flag);
	}
}

void BKP_WriteBackupRegister(uint16_t BKP_DR, uint16_t Data)
{
//  *(__IO uint32_t *) BKP_DR = Data;
	__IO uint32_t tmp = 0;	
	tmp = (uint32_t)BKPSRAM_BASE;
	tmp += BKP_DR;

  *(__IO uint32_t *) tmp = Data;
	//BKP_DR = Data;
}

uint16_t BKP_ReadBackupRegister(uint16_t BKP_DR)
{
  __IO uint32_t tmp = 0;

  /* Check the parameters */
  //assert_param(IS_BKP_DR(BKP_DR));

  tmp = (uint32_t)BKPSRAM_BASE; 
  tmp += BKP_DR;

  return (*(__IO uint16_t *) tmp);
	//	return BKP_DR;
}


/*************************END OF FILE******************************/

