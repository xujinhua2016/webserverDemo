#ifndef __BSP_MB_H
#define	__BSP_MB_H

//#include "stm32f10x.h"
#include  "stm32f4xx.h"	

#include "bsp_mb_usart.h"
#include "bsp_mb_timer.h"
#include "bsp_mb_crc.h"


#define  GE_MB_Invalid_Func_Code 	0x01			//无效功能码
#define  GE_MB_Invalid_Data_Addr 	0x02			//无效数据地址
#define  GE_MB_Invalid_Data      	0x03			//无效数据
#define  GE_MB_Slave_Fault 				0x04			//从机设备故障
#define  GE_MB_Rec_Command 				0x05			//收到命令
#define  GE_MB_Slave_Busy 				0x06			//从机忙

#define  GE_MB_IDLE               0
#define  GE_MB_485                1
#define  GE_MB_ARM                2

void  GE_MB_Init(void);						//modbus相关初始化
void  GE_MB_Protocol(void);
void  GE_MB_WarningFunCode_Feedback(uint8_t fault_code);
void  GE_MB_Data_Feedback(uint8_t leng, uint8_t cnt, uint8_t *data);


#endif

