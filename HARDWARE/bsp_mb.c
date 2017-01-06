#include  "bsp_mb.h"
#include  "bsp_led.h"
#include  <ucos_ii.h>
#include  "bsp_mem.h"


volatile  uint8_t  GE_MB_RxBuffer[200]        = {0x00};					//modbus数据接收缓存区
					uint8_t  GE_MB_TxBuffer[200]        = {0x00};					//modbus数据发送缓存区
volatile  uint8_t  GE_MB_RxCounter            = 0;
volatile  uint8_t  GE_MB_FrmeReceiveDone_Flag = GE_MB_IDLE;			//帧接收标志位，初始值为空闲状态
volatile  uint8_t  MB_TimeExpireT3_5          = 1;

uint8_t   BuffCounter = 0;

uint32_t  GE_MB_REQ_STAR_REGISTERS     = 0;
uint8_t   GE_MB_REQ_START_REGISTER_HI  = 0;
uint8_t   GE_MB_REQ_START_REGISTER_LOW = 0;
uint8_t   GE_MB_REQ_REGISTERS          = 0;
uint8_t   GE_MB_REQ_REGISTERS_HI       = 0;
uint8_t   GE_MB_REQ_REGISTERS_LOW      = 0;


void  GE_MB_FUNC_READ_MULTIPLE_REGISTERS(void);												//定义读多个寄存器数据的函数
void  GE_MB_FUNC_WRITE_SINGLE_REGISTERS(void);												//定义写单个寄存器数据的函数
void  GE_MB_FUNC_WRITE_MULTIPLE_REGISTERS(void);											//定义写多个寄存器数据的函数
void  GE_MB_Data_Feedback(uint8_t leng, uint8_t cnt, uint8_t *data);	//数据反馈函数，从机相应告警


void  GE_MB_Init(void)
{
	GE_MB_USART_Init();
	GE_MB_TIMER_Init();
}


void  GE_MB_Protocol(void)
{
		
		// 等待串口中断置位 一帧数据接收完成标志
		if((GE_MB_FrmeReceiveDone_Flag == GE_MB_485) || (GE_MB_FrmeReceiveDone_Flag == GE_MB_ARM))
		{
			BuffCounter = 0;
			
			/* 从机地址判断 */
			if(GE_MB_RxBuffer[BuffCounter++] == BKP_ReadBackupRegister(SlaveAddress))
			{
				/* CRC校验 */
				if(GE_BSP_MB_CRC(GE_MB_RxBuffer, GE_MB_RxCounter - 2) == ((GE_MB_RxBuffer[GE_MB_RxCounter - 1] << 8) | (GE_MB_RxBuffer[GE_MB_RxCounter - 2])))
				{
					/* CRC校验正确，解析功能码 */
					switch(GE_MB_RxBuffer[BuffCounter++])
					{
						/* 读取多个寄存器 */
						case 0x03:
							GE_MB_FUNC_READ_MULTIPLE_REGISTERS();
						break;
						
						/* 写单个寄存器 */
						case 0x06:
							GE_MB_FUNC_WRITE_SINGLE_REGISTERS();
						break;
						
						/* 写多个寄存器 */
						case 0x16:
							GE_MB_FUNC_WRITE_MULTIPLE_REGISTERS();
						break;
						
						/* 非法功能码响应：无效功能码 */
						default:
							GE_MB_WarningFunCode_Feedback(GE_MB_Invalid_Func_Code);
						break;
					}
				}
				else // CRC校验出错响应：无效数据
			  {
				  GE_MB_WarningFunCode_Feedback(GE_MB_Invalid_Data);
			  }
			}
			GE_MB_RxCounter = 0;      // 复位数据计数变量
			GE_MB_FrmeReceiveDone_Flag = GE_MB_IDLE; // 复位数据帧接收完成变量，等待下一帧数据
		}
		
	//	OSTimeDlyHMSM(0, 0, 0, 50);// 延时用于多任务调度
	
}


/*
*********************************************************************************************************
*                 GE_MB_FUNC_READ_MULTIPLE_REGISTERS(void)
*
* Description : Feedback register value.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : ARM & 485.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void  GE_MB_FUNC_READ_MULTIPLE_REGISTERS(void)
{
	uint8_t  index = 3;
	uint16_t crc_temp;
	
	if(GE_MB_RxBuffer[2] == 9)     // 判断访问可配置寄存器
	{
		/* 解析要读取的寄存器数目 */
		GE_MB_REQ_REGISTERS = GE_MB_RxBuffer[4] << 8 | GE_MB_RxBuffer[5];
		
		// 需要读取的寄存器数超出范围响应：无效数据
		if(((GE_MB_RxBuffer[3] + GE_MB_REQ_REGISTERS) > 0x12) || (GE_MB_REQ_REGISTERS == 0))
		{
			GE_MB_WarningFunCode_Feedback(GE_MB_Invalid_Data_Addr);
		}
		else
		{
			/* 解析可配置寄存器地址低字节 */
		  //GE_MB_REQ_START_REGISTER_LOW = 0;//GE_MB_RxBuffer[3];
			//GE_MB_REQ_STAR_REGISTERS     = 0;//(GE_MB_REQ_START_REGISTER_LOW * 4) + 0x0040;
			
			GE_MB_REQ_START_REGISTER_LOW = GE_MB_RxBuffer[3];
			GE_MB_REQ_STAR_REGISTERS     = (GE_MB_REQ_START_REGISTER_LOW * 4);// + 0x0040; 
			
			GE_MB_TxBuffer[0] = GE_MB_RxBuffer[0];       // 加载回传数据地址
			GE_MB_TxBuffer[1] = GE_MB_RxBuffer[1];       // 加载回传数据功能码
			GE_MB_TxBuffer[2] = GE_MB_REQ_REGISTERS * 2; // 加载回传数据字节数
			
			while(GE_MB_REQ_REGISTERS--) // 加载回传数据
			{
				GE_MB_TxBuffer[index++] = BKP_ReadBackupRegister(GE_MB_REQ_STAR_REGISTERS) >> 8;
				GE_MB_TxBuffer[index++] = BKP_ReadBackupRegister(GE_MB_REQ_STAR_REGISTERS) & 0x00ff;
				GE_MB_REQ_STAR_REGISTERS += 0x0004;
			}
			
			crc_temp = GE_BSP_MB_CRC(GE_MB_TxBuffer, index); // 计算CRC校验码
			GE_MB_TxBuffer[index++] = crc_temp & 0x00FF;  // 加载CRC校验码低字节
			GE_MB_TxBuffer[index++] = crc_temp >> 8;      // 加载CRC校验码高字节
			
			GE_MB_Data_Feedback(index, 0, GE_MB_TxBuffer);
		}
	}
	else if(GE_MB_RxBuffer[2] < 9) // 访问只读寄存器
	{
		/* 解析寄存器起始地址 */
		GE_MB_REQ_START_REGISTER_HI  = GE_MB_RxBuffer[BuffCounter++];
	  GE_MB_REQ_START_REGISTER_LOW = GE_MB_RxBuffer[BuffCounter++];
		GE_MB_REQ_STAR_REGISTERS     = GE_BAT_BASE + (GE_MB_REQ_START_REGISTER_HI<<8 | (GE_MB_REQ_START_REGISTER_LOW * 4));
		//GE_MB_REQ_STAR_REGISTERS     = GE_BAT_BASE + (GE_MB_REQ_START_REGISTER_HI<<8 | (GE_MB_REQ_START_REGISTER_LOW * 4));
		
		
		/* 解析要读取的寄存器数目 */
		GE_MB_REQ_REGISTERS_HI  = GE_MB_RxBuffer[BuffCounter++];
		GE_MB_REQ_REGISTERS_LOW = GE_MB_RxBuffer[BuffCounter++];
		GE_MB_REQ_REGISTERS     = GE_MB_REQ_REGISTERS_HI<<8 | GE_MB_REQ_REGISTERS_LOW;
		
		GE_MB_TxBuffer[0] = GE_MB_RxBuffer[0];       // 加载回传数据地址
		GE_MB_TxBuffer[1] = GE_MB_RxBuffer[1];       // 加载回传数据功能码
		GE_MB_TxBuffer[2] = GE_MB_REQ_REGISTERS * 2; // 加载回传数据字节数
		
		/* 加载要读取的寄存器数据 */
		while(GE_MB_REQ_REGISTERS--)
		{
			//数据从GE_MB_TxBuffer[3]开始
			GE_MB_TxBuffer[index++] = ((*(uint16_t *)(GE_MB_REQ_STAR_REGISTERS)) & 0xff00) >> 8;
			GE_MB_TxBuffer[index++] = ( *(uint16_t *)(GE_MB_REQ_STAR_REGISTERS)) & 0x00ff;
			GE_MB_REQ_STAR_REGISTERS += 0x0004;
		} 
		
		/* 加载CRC校验码 */
		crc_temp = GE_BSP_MB_CRC(GE_MB_TxBuffer, index);
		GE_MB_TxBuffer[index++] = crc_temp & 0x00FF;
		GE_MB_TxBuffer[index++] = crc_temp >> 8;
		
		GE_MB_Data_Feedback(index, 0, GE_MB_TxBuffer);
	}
	else                           // 访问非法寄存器响应：无效数据地址
	{
		GE_MB_WarningFunCode_Feedback(GE_MB_Invalid_Data_Addr);
	}
}


void  GE_MB_FUNC_WRITE_SINGLE_REGISTERS(void)
{
	uint8_t  index = 0;
	
	// 判断写备份寄存器地址是否合法
	if((GE_MB_RxBuffer[2] == 0x09) && (GE_MB_RxBuffer[3] <= 0x11))
	{
		// 写入备份寄存器值
		//BKP_WriteBackupRegister((GE_MB_RxBuffer[3] * 4) + 0x0040, (GE_MB_RxBuffer[4] << 8) | GE_MB_RxBuffer[5]);
		BKP_WriteBackupRegister((GE_MB_RxBuffer[3] * 4), (GE_MB_RxBuffer[4] << 8) | GE_MB_RxBuffer[5]);
		
		// 加载回传数据
		while(GE_MB_RxCounter--)
		{
			GE_MB_TxBuffer[index] = GE_MB_RxBuffer[index];
			index++;
		}
		
		// 发送数据
		GE_MB_Data_Feedback(index, 0, GE_MB_TxBuffer);
		
		// 修改相应备份寄存器函数
		if(GE_MB_RxBuffer[3] == 2)
		{
			BSP_MB_USART_1_Mode_Init(BKP_ReadBackupRegister(TransBaudRate));
			//BSP_MB_USART_2_Mode_Init(BKP_ReadBackupRegister(TransBaudRate));
		}
		// 修改相应备份寄存器函数
		if(GE_MB_RxBuffer[3] > 0x0B)
		{
			// RTC 时钟修改时间
		}
	}
	else
	{
		GE_MB_WarningFunCode_Feedback(GE_MB_Invalid_Data_Addr);
	}
}


void  GE_MB_FUNC_WRITE_MULTIPLE_REGISTERS(void)
{
	uint8_t  index = 7;
	uint16_t crc_temp;
	
	// 判断写备份寄存器地址是否合法
	if((GE_MB_RxBuffer[2] == 0x09) && (GE_MB_RxBuffer[3] <= 0x11))
	{
		// 解析写备份寄存器数
		GE_MB_REQ_REGISTERS = GE_MB_RxBuffer[4] << 8 | GE_MB_RxBuffer[5];
		
		// 判断写备份寄存器数是否合法
		if((GE_MB_REQ_REGISTERS != GE_MB_RxBuffer[6]) || \
			 ((GE_MB_RxBuffer[3] + GE_MB_REQ_REGISTERS) > 0x12) || (GE_MB_REQ_REGISTERS == 0))
		{
			GE_MB_WarningFunCode_Feedback(GE_MB_Invalid_Data_Addr);
		}
		else
		{
			// 解析写备份寄存器起始地址
			GE_MB_REQ_STAR_REGISTERS = (GE_MB_RxBuffer[3] * 4) + 0x0040;
			
			// 更新备份寄存器值
			while(GE_MB_RxBuffer[6]--)
			{
				BKP_WriteBackupRegister(GE_MB_REQ_STAR_REGISTERS, (GE_MB_RxBuffer[index] << 8) | GE_MB_RxBuffer[index+1]);
				GE_MB_REQ_STAR_REGISTERS += 0x0004;
				index += 2;
			}
			
			// 加载回传数据
			GE_MB_TxBuffer[0] = BKP_ReadBackupRegister(SlaveAddress);
			GE_MB_TxBuffer[1] = GE_MB_RxBuffer[1];
			GE_MB_TxBuffer[2] = GE_MB_RxBuffer[2];
			GE_MB_TxBuffer[3] = GE_MB_RxBuffer[3];
			GE_MB_TxBuffer[4] = GE_MB_RxBuffer[4]; //(BKP_ReadBackupRegister(ConnectLineRes) & 0xFF00)>>8;
			GE_MB_TxBuffer[5] = GE_MB_RxBuffer[5]; //BKP_ReadBackupRegister(ConnectLineRes) & 0x00FF;
			
			/* 加载CRC校验码 */
		  crc_temp = GE_BSP_MB_CRC(GE_MB_TxBuffer, 6);
		  GE_MB_TxBuffer[6] = crc_temp & 0x00FF; //(BKP_ReadBackupRegister(TransBaudRate) & 0xFF00)>>8;
		  GE_MB_TxBuffer[7] = crc_temp >> 8; //BKP_ReadBackupRegister(TransBaudRate) & 0x00FF;
			
			// 发送数据
			GE_MB_Data_Feedback(8, 0, GE_MB_TxBuffer);
			
			// 修改相应备份寄存器函数
			if((GE_MB_RxBuffer[3] <= 2) && (GE_MB_RxBuffer[3] + GE_MB_REQ_REGISTERS) > 0x02)
			{
				BSP_MB_USART_1_Mode_Init(BKP_ReadBackupRegister(TransBaudRate));
				//BSP_MB_USART_2_Mode_Init(BKP_ReadBackupRegister(TransBaudRate));
			}
			
			// 修改相应备份寄存器函数
			if((GE_MB_RxBuffer[3] + GE_MB_REQ_REGISTERS) > 0x0B)
			{
				// RTC 时钟修改时间
			}
		}
	}
	else
	{
		GE_MB_WarningFunCode_Feedback(GE_MB_Invalid_Data_Addr);
	}
}


/*
*********************************************************************************************************
*                 GE_MB_WarningFunCode_Feedback(uint8_t arm_485, uint8_t fault_code)
*
* Description : Feedback warning function code.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : ARM & 485.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void  GE_MB_WarningFunCode_Feedback(uint8_t fault_code)
{
	uint16_t crc_temp;
	
	GE_MB_TxBuffer[0] = GE_MB_RxBuffer[0];
	GE_MB_TxBuffer[1] = GE_MB_RxBuffer[1] + 0x80;
	GE_MB_TxBuffer[2] = fault_code;
	
	crc_temp = GE_BSP_MB_CRC(GE_MB_TxBuffer, 3);
	GE_MB_TxBuffer[3] = crc_temp & 0x00FF;
	GE_MB_TxBuffer[4] = crc_temp >> 8;
	
	GE_MB_Data_Feedback(5, 0, GE_MB_TxBuffer);
	
}


void  GE_MB_Data_Feedback(uint8_t leng, uint8_t cnt, uint8_t *data)
{
	// ARM USART 和 485 USART 均可调用该 告警码返回 函数
	if(GE_MB_FrmeReceiveDone_Flag == GE_MB_485)
	{
		GE_MB_485_SEND_DATA;
		while(leng--)
		{
			USART_SendData(GE_MB_485_USART_NUM, data[cnt++]);
			while(USART_GetFlagStatus(GE_MB_485_USART_NUM, USART_FLAG_TXE) == RESET);
		}
		// 没有查询该标志位，将会丢失最后一个字节数据的传输，因为485芯片很快转为接收模式。
		// 可用延时函数替代该语句，但会降低效率。
		while(USART_GetFlagStatus(GE_MB_485_USART_NUM, USART_FLAG_TC) == RESET);
		GE_MB_485_REV_DATA;
	}
	else if(GE_MB_FrmeReceiveDone_Flag == GE_MB_ARM)
	{
		while(leng--)
		{
			USART_SendData(GE_MB_ARM_USART_NUM, data[cnt++]);
			while(USART_GetFlagStatus(GE_MB_ARM_USART_NUM, USART_FLAG_TXE) == RESET);
		}
	}
}
