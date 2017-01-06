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

/********  �����ѹ���棬ʹ�õ�16λ����س��ֵ�ѹ���棬��Ӧ����λ��1  **********/
extern volatile uint32_t 	BatVolWarnBit000_015;	// #000~#015�ŵ��
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

/***************  �����������ر��#0~#191������ַ0x20001400  ***************/
extern volatile	uint32_t	BatIntRes[192];

/*******  ���������辯�棬ʹ�õ�16λ����س��ֵ�ѹ���棬��Ӧ����λ��1  *******/
extern volatile uint32_t	BatResWarnBit000_015;	// #000~#015�ŵ��
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

/*******************************  �����ز���ֵ  ******************************/
extern volatile	uint32_t	TotalBatVol;      // �����ѹ
extern volatile	uint32_t	ChargeCurrentVal; // ��ŵ����
extern volatile	uint32_t	FloatCurrentVal;	// �������
extern volatile	uint32_t	Temperature;	    // �����¶�
extern volatile	uint32_t	HigVolBatNum1;    // ��ߵ�ѹ1��ر��
extern volatile	uint32_t	HigVolBatVal1;    // ��ߵ�ѹ1��ص�ѹ
extern volatile	uint32_t	HigVolBatNum2;    // ��ߵ�ѹ2��ر��
extern volatile	uint32_t	HigVolBatVal2;    // ��ߵ�ѹ2��ص�ѹ
extern volatile	uint32_t	HigVolBatNum3;	  // ��ߵ�ѹ3��ر��
extern volatile	uint32_t	HigVolBatVal3;	  // ��ߵ�ѹ3��ص�ѹ
extern volatile	uint32_t	LowVolBatNum1;	  // ��͵�ѹ1��ر��
extern volatile	uint32_t	LowVolBatVal1;	  // ��͵�ѹ1��ص�ѹ
extern volatile	uint32_t	LowVolBatNum2;	  // ��͵�ѹ2��ر��
extern volatile	uint32_t	LowVolBatVal2;	  // ��͵�ѹ2��ص�ѹ
extern volatile	uint32_t	LowVolBatNum3;	  // ��͵�ѹ3��ر��
extern volatile	uint32_t	LowVolBatVal3;	  // ��͵�ѹ3��ص�ѹ
extern volatile	uint32_t	MaxResBatNum;   	// ��������ر��
extern volatile	uint32_t	MaxResBatVal;    	// �����������ֵ
extern volatile	uint32_t	AverageVolVal;	  // ƽ����ѹֵ
extern volatile	uint32_t	MaxVolDiff;	      // ����ѹ��ֵ

extern volatile	uint32_t	ReadTheClkYear;	  // ��ȡʱ����
extern volatile	uint32_t	ReadTheClkMonth;  // ��ȡʱ����
extern volatile	uint32_t	ReadTheClkDay;	  // ��ȡʱ����
extern volatile	uint32_t	ReadTheClkHour;	  // ��ȡʱ��ʱ
extern volatile	uint32_t	ReadTheClkMinu;	  // ��ȡʱ�ӷ�
extern volatile	uint32_t	ReadTheClkSeco;  	// ��ȡʱ����

extern volatile	uint32_t  GroBatWarn;	      // �����ؾ���

extern volatile	uint32_t	BatSOC;	          // ���ʣ�����SOC

/*********************  �������üĴ���BKP2�����ǵ��籣��   *********************/
//extern volatile	uint32_t	SlaveAddress;           // �ӻ���ַ
//extern volatile	uint32_t	ConnectLineRes;         // �����ߵ���
//extern volatile	uint32_t	TransBaudRate;          // ���䲨����
//extern volatile	uint32_t	TotalNumBat;            // �������
//extern volatile	uint32_t	MaxWarnRes;             // ��߸澯����
//extern volatile	uint32_t	HigWarnVolSinBat;       // ��߸澯����ص�ѹ
//extern volatile	uint32_t	LowWarnVolSinBat;	      // ��͸澯����ص�ѹ
//extern volatile	uint32_t	MaxWarnVolDiff;       	// ���澯��ѹ��
//extern volatile	uint32_t	MaxWarnChargeCurVal;	  // ���澯������
//extern volatile	uint32_t	MaxWarnDischargeCurVal; // ���澯�ŵ����
//extern volatile	uint32_t	HigWarnTemperature;	    // ��߸澯�����¶�
//extern volatile	uint32_t	LowWarnTemperature;    	// ��͸澯�����¶�
//extern volatile	uint32_t	RTC_Year;             	// RTC��
//extern volatile	uint32_t	RTC_Moth;	             	// RTC��
//extern volatile	uint32_t	RTC_Day;	             	// RTC��
//extern volatile	uint32_t	RTC_Hour;	             	// RTCʱ
//extern volatile	uint32_t	RTC_Min;	             	// RTC��
//extern volatile	uint32_t	RTC_Sec;             		// RTC��

//extern volatile	uint32_t	BatCapacity;	        	// �������
//extern volatile	uint32_t	BKPBatSOC;	          	// ���SOC


#define	SlaveAddress           0x0000//__attribute__((at(BKPSRAM_BASE + 0x0000))); // �ӻ���ַ
#define	ConnectLineRes         0x0004//__attribute__((at(BKPSRAM_BASE + 0x0004))); // �����ߵ���
#define	TransBaudRate          0x0008 //__attribute__((at(BKPSRAM_BASE + 0x0008)));	// ���䲨����
#define	TotalNumBat            0x000C //__attribute__((at(BKPSRAM_BASE + 0x000C)));	// �������
#define	MaxWarnRes             0x0010 //__attribute__((at(BKPSRAM_BASE + 0x0010))); // ��߸澯����
#define	HigWarnVolSinBat       0x0014 //__attribute__((at(BKPSRAM_BASE + 0x0014))); // ��߸澯����ص�ѹ
#define	LowWarnVolSinBat       0x0018 //__attribute__((at(BKPSRAM_BASE + 0x0018)));	// ��͸澯����ص�ѹ
#define	MaxWarnVolDiff         0x001C //__attribute__((at(BKPSRAM_BASE + 0x001C)));	// ���澯��ѹ��
#define	MaxWarnChargeCurVal    0x0020 //__attribute__((at(BKPSRAM_BASE + 0x0020)));	// ���澯������
#define	MaxWarnDischargeCurVal 0x0024 //__attribute__((at(BKPSRAM_BASE + 0x0024))); // ���澯�ŵ����
#define	HigWarnTemperature     0x0028 //__attribute__((at(BKPSRAM_BASE + 0x0028)));	// ��߸澯�����¶�
#define	LowWarnTemperature     0x002C //__attribute__((at(BKPSRAM_BASE + 0x002C)));	// ��͸澯�����¶�
#define	RTC_Year               0x0030 //__attribute__((at(BKPSRAM_BASE + 0x0030)));	// RTC��
#define	RTC_Moth               0x0034 //__attribute__((at(BKPSRAM_BASE + 0x0034)));	// RTC��
#define	RTC_Day                0x0038 //__attribute__((at(BKPSRAM_BASE + 0x0038)));	// RTC��
#define	RTC_Hour               0x003C //__attribute__((at(BKPSRAM_BASE + 0x003C)));	// RTCʱ
#define	RTC_Min                0x0040 //__attribute__((at(BKPSRAM_BASE + 0x0040)));	// RTC��
#define	RTC_Sec                0x0044 //__attribute__((at(BKPSRAM_BASE + 0x0044)));	// RTC��

#define	BatCapacity            0x0048 //__attribute__((at(BKPSRAM_BASE + 0x0048)));	// �������
#define	BKPBatSOC              0x004C //__attribute__((at(BKPSRAM_BASE + 0x004C)));	// ���SOC

#define	BKP_Flag               0x0050 //__attribute__((at(BKPSRAM_BASE + 0x0050)));	// ���ݼĴ�����д��־�Ĵ���




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

