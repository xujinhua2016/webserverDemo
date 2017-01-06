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
* Notes         : 1  V1.00�������洢λ���Ѿ����úã����ݼĴ���������������洢����
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

uint8_t BKP_Write_Flag = 0xA4; // ���ݼĴ�����д��־

/**************************  Battery voltage register **************************/
/***************  �����ص�ѹ��ر��#0~#191������ַ0x20001000  ***************/
volatile uint32_t	  GE_BAT_Vol[192]				  __attribute__((at(GE_BAT_BASE)));

/********  �����ѹ���棬ʹ�õ�16λ����س��ֵ�ѹ���棬��Ӧ����λ��1  **********/
volatile uint32_t 	BatVolWarnBit000_015		__attribute__((at(GE_BAT_BASE + 0x0300)));	// #000~#015�ŵ��
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

/***************  �����������ر��#0~#191������ַ0x20001400  ***************/
volatile	uint32_t	BatIntRes[192]					__attribute__((at(GE_BAT_BASE + 0x0400)));

/*******  ���������辯�棬ʹ�õ�16λ����س������辯�棬��Ӧ����λ��1  *******/
volatile 	uint32_t	BatResWarnBit000_015		__attribute__((at(GE_BAT_BASE + 0x0700)));	// #000~#015�ŵ��
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

/*******************************  �����ز���ֵ  ******************************/
volatile	uint32_t	TotalBatVol							__attribute__((at(GE_BAT_BASE + 0x0800)));	// �����ѹ
volatile	uint32_t	ChargeCurrentVal				__attribute__((at(GE_BAT_BASE + 0x0804)));	// ��ŵ����
volatile	uint32_t	FloatCurrentVal					__attribute__((at(GE_BAT_BASE + 0x0808)));	// �������
volatile	uint32_t	Temperature							__attribute__((at(GE_BAT_BASE + 0x080C)));	// �����¶�
volatile	uint32_t	HigVolBatNum1						__attribute__((at(GE_BAT_BASE + 0x0810)));	// ��ߵ�ѹ1��ر��
volatile	uint32_t	HigVolBatVal1						__attribute__((at(GE_BAT_BASE + 0x0814)));	// ��ߵ�ѹ1��ص�ѹ
volatile	uint32_t	HigVolBatNum2						__attribute__((at(GE_BAT_BASE + 0x0818)));	// ��ߵ�ѹ2��ر��
volatile	uint32_t	HigVolBatVal2						__attribute__((at(GE_BAT_BASE + 0x081C)));	// ��ߵ�ѹ2��ص�ѹ
volatile	uint32_t	HigVolBatNum3						__attribute__((at(GE_BAT_BASE + 0x0820)));	// ��ߵ�ѹ3��ر��
volatile	uint32_t	HigVolBatVal3						__attribute__((at(GE_BAT_BASE + 0x0824)));	// ��ߵ�ѹ3��ص�ѹ
volatile	uint32_t	LowVolBatNum1						__attribute__((at(GE_BAT_BASE + 0x0828)));	// ��͵�ѹ1��ر��
volatile	uint32_t	LowVolBatVal1						__attribute__((at(GE_BAT_BASE + 0x082C)));	// ��͵�ѹ1��ص�ѹ
volatile	uint32_t	LowVolBatNum2						__attribute__((at(GE_BAT_BASE + 0x0830)));	// ��͵�ѹ2��ر��
volatile	uint32_t	LowVolBatVal2						__attribute__((at(GE_BAT_BASE + 0x0834)));	// ��͵�ѹ2��ص�ѹ
volatile	uint32_t	LowVolBatNum3						__attribute__((at(GE_BAT_BASE + 0x0838)));	// ��͵�ѹ3��ر��
volatile	uint32_t	LowVolBatVal3						__attribute__((at(GE_BAT_BASE + 0x083C)));	// ��͵�ѹ3��ص�ѹ
volatile	uint32_t	MaxResBatNum						__attribute__((at(GE_BAT_BASE + 0x0840)));	// ��������ر��
volatile	uint32_t	MaxResBatVal						__attribute__((at(GE_BAT_BASE + 0x0844)));	// �����������ֵ
volatile	uint32_t	AverageVolVal						__attribute__((at(GE_BAT_BASE + 0x0848)));	// ƽ����ѹֵ
volatile	uint32_t	MaxVolDiff							__attribute__((at(GE_BAT_BASE + 0x084C)));	// ����ѹ��ֵ

volatile	uint32_t	ReadTheClkYear					__attribute__((at(GE_BAT_BASE + 0x0850)));	// ��ȡʱ����
volatile	uint32_t	ReadTheClkMonth					__attribute__((at(GE_BAT_BASE + 0x0854)));	// ��ȡʱ����
volatile	uint32_t	ReadTheClkDay						__attribute__((at(GE_BAT_BASE + 0x0858)));	// ��ȡʱ����
volatile	uint32_t	ReadTheClkHour					__attribute__((at(GE_BAT_BASE + 0x085C)));	// ��ȡʱ��ʱ
volatile	uint32_t	ReadTheClkMinu					__attribute__((at(GE_BAT_BASE + 0x0860)));	// ��ȡʱ�ӷ�
volatile	uint32_t	ReadTheClkSeco					__attribute__((at(GE_BAT_BASE + 0x0864)));	// ��ȡʱ����

volatile	uint32_t	GroBatWarn							__attribute__((at(GE_BAT_BASE + 0x0868)));	// �����ؾ���

volatile	uint32_t	BatSOC									__attribute__((at(GE_BAT_BASE + 0x086C)));	// ���ʣ�����SOC

/*****************************  �����ر��ݲ���ֵ  ****************************/
// ����ַ��0x40024000 ƫ�Ƶ�ַ��0x0004
//volatile	uint32_t	SlaveAddress           0x0000; //__attribute__((at(BKPSRAM_BASE + 0x0000))); // �ӻ���ַ
//volatile	uint32_t	ConnectLineRes          //__attribute__((at(BKPSRAM_BASE + 0x0004))); // �����ߵ���
//volatile	uint32_t	TransBaudRate           //__attribute__((at(BKPSRAM_BASE + 0x0008)));	// ���䲨����
//volatile	uint32_t	TotalNumBat             //__attribute__((at(BKPSRAM_BASE + 0x000C)));	// �������
//volatile	uint32_t	MaxWarnRes              //__attribute__((at(BKPSRAM_BASE + 0x0010))); // ��߸澯����
//volatile	uint32_t	HigWarnVolSinBat        //__attribute__((at(BKPSRAM_BASE + 0x0014))); // ��߸澯����ص�ѹ
//volatile	uint32_t	LowWarnVolSinBat        //__attribute__((at(BKPSRAM_BASE + 0x0018)));	// ��͸澯����ص�ѹ
//volatile	uint32_t	MaxWarnVolDiff          //__attribute__((at(BKPSRAM_BASE + 0x001C)));	// ���澯��ѹ��
//volatile	uint32_t	MaxWarnChargeCurVal     //__attribute__((at(BKPSRAM_BASE + 0x0020)));	// ���澯������
//volatile	uint32_t	MaxWarnDischargeCurVal  //__attribute__((at(BKPSRAM_BASE + 0x0024))); // ���澯�ŵ����
//volatile	uint32_t	HigWarnTemperature      //__attribute__((at(BKPSRAM_BASE + 0x0028)));	// ��߸澯�����¶�
//volatile	uint32_t	LowWarnTemperature      //__attribute__((at(BKPSRAM_BASE + 0x002C)));	// ��͸澯�����¶�
//volatile	uint32_t	RTC_Year                //__attribute__((at(BKPSRAM_BASE + 0x0030)));	// RTC��
//volatile	uint32_t	RTC_Moth                //__attribute__((at(BKPSRAM_BASE + 0x0034)));	// RTC��
//volatile	uint32_t	RTC_Day                 //__attribute__((at(BKPSRAM_BASE + 0x0038)));	// RTC��
//volatile	uint32_t	RTC_Hour                //__attribute__((at(BKPSRAM_BASE + 0x003C)));	// RTCʱ
//volatile	uint32_t	RTC_Min                 //__attribute__((at(BKPSRAM_BASE + 0x0040)));	// RTC��
//volatile	uint32_t	RTC_Sec                 //__attribute__((at(BKPSRAM_BASE + 0x0044)));	// RTC��

//volatile	uint32_t	BatCapacity             //__attribute__((at(BKPSRAM_BASE + 0x0048)));	// �������
//volatile	uint32_t	BKPBatSOC               //__attribute__((at(BKPSRAM_BASE + 0x004C)));	// ���SOC

//volatile	uint32_t	BKP_Flag                //__attribute__((at(BKPSRAM_BASE + 0x0050)));	// ���ݼĴ�����д��־�Ĵ���


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
	TotalBatVol      = 0; // �����ѹ
	ChargeCurrentVal = 0; // ��ŵ����
	FloatCurrentVal  = 0; // �������
	Temperature      = 0; // �����¶�
  
	HigVolBatNum1		 = 0;	// ��ߵ�ѹ1��ر��
  HigVolBatVal1		 = 0;	// ��ߵ�ѹ1��ص�ѹ
  HigVolBatNum2		 = 0; // ��ߵ�ѹ2��ر��
  HigVolBatVal2		 = 0; // ��ߵ�ѹ2��ص�ѹ
  HigVolBatNum3		 = 0; // ��ߵ�ѹ3��ر��
  HigVolBatVal3		 = 0;	// ��ߵ�ѹ3��ص�ѹ
  LowVolBatNum1		 = 0;	// ��͵�ѹ1��ر��
  LowVolBatVal1		 = 0;	// ��͵�ѹ1��ص�ѹ
  LowVolBatNum2		 = 0;	// ��͵�ѹ2��ر��
  LowVolBatVal2		 = 0;	// ��͵�ѹ2��ص�ѹ
  LowVolBatNum3		 = 0;	// ��͵�ѹ3��ر��
  LowVolBatVal3		 = 0;	// ��͵�ѹ3��ص�ѹ
	
	MaxResBatNum     = 0; // ��������ر��
	MaxResBatVal     = 0; // �����������ֵ
	
	AverageVolVal    = 0; // ƽ����ѹֵ
	MaxVolDiff       = 0; // ����ѹ��ֵ
	
	ReadTheClkYear   = 0; // ��ȡʱ����
	ReadTheClkMonth  = 0; // ��ȡʱ����
	ReadTheClkDay    = 0; // ��ȡʱ����
	ReadTheClkHour   = 0; // ��ȡʱ��ʱ
	ReadTheClkMinu   = 0; // ��ȡʱ�ӷ�
	ReadTheClkSeco   = 0; // ��ȡʱ����
	
	GroBatWarn       = 0; // �����ؾ���
	
	BatSOC           = 0; // ���ʣ�����SOC
}


void	BSP_BKPSRAM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	/* ʹ�� BKPRAM ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM,ENABLE);
	/* ���ñ���SRAM�ĵ͹�����ѹ������VBATģʽ�Ա�������*/
	PWR_BackupRegulatorCmd(ENABLE);
	/* �ȴ�ֱ������SRAM�ĵ͹�����ѹ������ */
	while(PWR_GetFlagStatus(PWR_FLAG_BRR) == RESET)
	{
	}
//	
//	if(BKP_Flag != BKP_Write_Flag)
//	{
//		SlaveAddress           = 1;     // �ӻ���ַ
//		ConnectLineRes         = 1;     // �����ߵ���
//		TransBaudRate          = 19200; // ���䲨����
//		TotalNumBat            = 24;    // �������
//		MaxWarnRes             = 15;    // ��߸澯����
//		HigWarnVolSinBat       = 2300;  // ��߸澯����ص�ѹ
//		LowWarnVolSinBat       = 1800;  // ��͸澯����ص�ѹ
//		MaxWarnVolDiff         = 1800;  // ���澯��ѹ��
//		MaxWarnChargeCurVal    = 500;	  // ���澯������
//		MaxWarnDischargeCurVal = 200;   // ���澯�ŵ����
//		HigWarnTemperature     = 33468; // ��߸澯�����¶�
//		LowWarnTemperature     = 400;   // ��͸澯�����¶�
//		
//		RTC_Year               = 0;     // RTC��
//		RTC_Moth               = 0;     // RTC��
//		RTC_Day                = 0;     // RTC��
//		RTC_Hour               = 0;     // RTCʱ
//		RTC_Min                = 0;     // RTC��
//		RTC_Sec                = 0;     // RTC��
//		
//		BatCapacity            = 0;     // �������
//		BKPBatSOC              = 0;     // ���SOC
//		
//		BKP_Flag = BKP_Write_Flag;      // д��־λ
//	}

	
	//Ϊ�˱����ظ���ʼ���������ڴ˴�����һ����־������Ϊ�ж�
	if(BKP_ReadBackupRegister(BKP_Flag) != BKP_Write_Flag)
	{
		BKP_WriteBackupRegister(SlaveAddress,             1); //�ӻ���ַ
		BKP_WriteBackupRegister(ConnectLineRes,           1); //�����ߵ���
		BKP_WriteBackupRegister(TransBaudRate,        19200); //���䲨����
		BKP_WriteBackupRegister(TotalNumBat,             24); //�������
		BKP_WriteBackupRegister(MaxWarnRes,              15); //��߸澯����
		BKP_WriteBackupRegister(HigWarnVolSinBat,      2300); //��߸澯����ص�ѹ
		BKP_WriteBackupRegister(LowWarnVolSinBat,      1800); //��͸澯����ص�ѹ
		BKP_WriteBackupRegister(MaxWarnVolDiff,        1800); //���澯��ѹ��
		BKP_WriteBackupRegister(MaxWarnChargeCurVal,    500); //���澯������
		BKP_WriteBackupRegister(MaxWarnDischargeCurVal, 200);	//���澯�ŵ����
		BKP_WriteBackupRegister(HigWarnTemperature,   33468); //��߸澯�����¶�
		BKP_WriteBackupRegister(LowWarnTemperature,     400); //��͸澯�����¶�
		
		BKP_WriteBackupRegister(RTC_Year,               0); // RTC��
		BKP_WriteBackupRegister(RTC_Moth,               0); // RTC��
		BKP_WriteBackupRegister(RTC_Day,                0); // RTC��
		BKP_WriteBackupRegister(RTC_Hour,               0); // RTCʱ
		BKP_WriteBackupRegister(RTC_Min,                0); // RTC��
		BKP_WriteBackupRegister(RTC_Sec,                0); // RTC��
		
		BKP_WriteBackupRegister(BatCapacity,            0); //�������
		BKP_WriteBackupRegister(BKPBatSOC,              0); //���SOC
		
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

