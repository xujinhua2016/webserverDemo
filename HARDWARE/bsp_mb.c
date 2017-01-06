#include  "bsp_mb.h"
#include  "bsp_led.h"
#include  <ucos_ii.h>
#include  "bsp_mem.h"


volatile  uint8_t  GE_MB_RxBuffer[200]        = {0x00};					//modbus���ݽ��ջ�����
					uint8_t  GE_MB_TxBuffer[200]        = {0x00};					//modbus���ݷ��ͻ�����
volatile  uint8_t  GE_MB_RxCounter            = 0;
volatile  uint8_t  GE_MB_FrmeReceiveDone_Flag = GE_MB_IDLE;			//֡���ձ�־λ����ʼֵΪ����״̬
volatile  uint8_t  MB_TimeExpireT3_5          = 1;

uint8_t   BuffCounter = 0;

uint32_t  GE_MB_REQ_STAR_REGISTERS     = 0;
uint8_t   GE_MB_REQ_START_REGISTER_HI  = 0;
uint8_t   GE_MB_REQ_START_REGISTER_LOW = 0;
uint8_t   GE_MB_REQ_REGISTERS          = 0;
uint8_t   GE_MB_REQ_REGISTERS_HI       = 0;
uint8_t   GE_MB_REQ_REGISTERS_LOW      = 0;


void  GE_MB_FUNC_READ_MULTIPLE_REGISTERS(void);												//���������Ĵ������ݵĺ���
void  GE_MB_FUNC_WRITE_SINGLE_REGISTERS(void);												//����д�����Ĵ������ݵĺ���
void  GE_MB_FUNC_WRITE_MULTIPLE_REGISTERS(void);											//����д����Ĵ������ݵĺ���
void  GE_MB_Data_Feedback(uint8_t leng, uint8_t cnt, uint8_t *data);	//���ݷ����������ӻ���Ӧ�澯


void  GE_MB_Init(void)
{
	GE_MB_USART_Init();
	GE_MB_TIMER_Init();
}


void  GE_MB_Protocol(void)
{
		
		// �ȴ������ж���λ һ֡���ݽ�����ɱ�־
		if((GE_MB_FrmeReceiveDone_Flag == GE_MB_485) || (GE_MB_FrmeReceiveDone_Flag == GE_MB_ARM))
		{
			BuffCounter = 0;
			
			/* �ӻ���ַ�ж� */
			if(GE_MB_RxBuffer[BuffCounter++] == BKP_ReadBackupRegister(SlaveAddress))
			{
				/* CRCУ�� */
				if(GE_BSP_MB_CRC(GE_MB_RxBuffer, GE_MB_RxCounter - 2) == ((GE_MB_RxBuffer[GE_MB_RxCounter - 1] << 8) | (GE_MB_RxBuffer[GE_MB_RxCounter - 2])))
				{
					/* CRCУ����ȷ������������ */
					switch(GE_MB_RxBuffer[BuffCounter++])
					{
						/* ��ȡ����Ĵ��� */
						case 0x03:
							GE_MB_FUNC_READ_MULTIPLE_REGISTERS();
						break;
						
						/* д�����Ĵ��� */
						case 0x06:
							GE_MB_FUNC_WRITE_SINGLE_REGISTERS();
						break;
						
						/* д����Ĵ��� */
						case 0x16:
							GE_MB_FUNC_WRITE_MULTIPLE_REGISTERS();
						break;
						
						/* �Ƿ���������Ӧ����Ч������ */
						default:
							GE_MB_WarningFunCode_Feedback(GE_MB_Invalid_Func_Code);
						break;
					}
				}
				else // CRCУ�������Ӧ����Ч����
			  {
				  GE_MB_WarningFunCode_Feedback(GE_MB_Invalid_Data);
			  }
			}
			GE_MB_RxCounter = 0;      // ��λ���ݼ�������
			GE_MB_FrmeReceiveDone_Flag = GE_MB_IDLE; // ��λ����֡������ɱ������ȴ���һ֡����
		}
		
	//	OSTimeDlyHMSM(0, 0, 0, 50);// ��ʱ���ڶ��������
	
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
	
	if(GE_MB_RxBuffer[2] == 9)     // �жϷ��ʿ����üĴ���
	{
		/* ����Ҫ��ȡ�ļĴ�����Ŀ */
		GE_MB_REQ_REGISTERS = GE_MB_RxBuffer[4] << 8 | GE_MB_RxBuffer[5];
		
		// ��Ҫ��ȡ�ļĴ�����������Χ��Ӧ����Ч����
		if(((GE_MB_RxBuffer[3] + GE_MB_REQ_REGISTERS) > 0x12) || (GE_MB_REQ_REGISTERS == 0))
		{
			GE_MB_WarningFunCode_Feedback(GE_MB_Invalid_Data_Addr);
		}
		else
		{
			/* ���������üĴ�����ַ���ֽ� */
		  //GE_MB_REQ_START_REGISTER_LOW = 0;//GE_MB_RxBuffer[3];
			//GE_MB_REQ_STAR_REGISTERS     = 0;//(GE_MB_REQ_START_REGISTER_LOW * 4) + 0x0040;
			
			GE_MB_REQ_START_REGISTER_LOW = GE_MB_RxBuffer[3];
			GE_MB_REQ_STAR_REGISTERS     = (GE_MB_REQ_START_REGISTER_LOW * 4);// + 0x0040; 
			
			GE_MB_TxBuffer[0] = GE_MB_RxBuffer[0];       // ���ػش����ݵ�ַ
			GE_MB_TxBuffer[1] = GE_MB_RxBuffer[1];       // ���ػش����ݹ�����
			GE_MB_TxBuffer[2] = GE_MB_REQ_REGISTERS * 2; // ���ػش������ֽ���
			
			while(GE_MB_REQ_REGISTERS--) // ���ػش�����
			{
				GE_MB_TxBuffer[index++] = BKP_ReadBackupRegister(GE_MB_REQ_STAR_REGISTERS) >> 8;
				GE_MB_TxBuffer[index++] = BKP_ReadBackupRegister(GE_MB_REQ_STAR_REGISTERS) & 0x00ff;
				GE_MB_REQ_STAR_REGISTERS += 0x0004;
			}
			
			crc_temp = GE_BSP_MB_CRC(GE_MB_TxBuffer, index); // ����CRCУ����
			GE_MB_TxBuffer[index++] = crc_temp & 0x00FF;  // ����CRCУ������ֽ�
			GE_MB_TxBuffer[index++] = crc_temp >> 8;      // ����CRCУ������ֽ�
			
			GE_MB_Data_Feedback(index, 0, GE_MB_TxBuffer);
		}
	}
	else if(GE_MB_RxBuffer[2] < 9) // ����ֻ���Ĵ���
	{
		/* �����Ĵ�����ʼ��ַ */
		GE_MB_REQ_START_REGISTER_HI  = GE_MB_RxBuffer[BuffCounter++];
	  GE_MB_REQ_START_REGISTER_LOW = GE_MB_RxBuffer[BuffCounter++];
		GE_MB_REQ_STAR_REGISTERS     = GE_BAT_BASE + (GE_MB_REQ_START_REGISTER_HI<<8 | (GE_MB_REQ_START_REGISTER_LOW * 4));
		//GE_MB_REQ_STAR_REGISTERS     = GE_BAT_BASE + (GE_MB_REQ_START_REGISTER_HI<<8 | (GE_MB_REQ_START_REGISTER_LOW * 4));
		
		
		/* ����Ҫ��ȡ�ļĴ�����Ŀ */
		GE_MB_REQ_REGISTERS_HI  = GE_MB_RxBuffer[BuffCounter++];
		GE_MB_REQ_REGISTERS_LOW = GE_MB_RxBuffer[BuffCounter++];
		GE_MB_REQ_REGISTERS     = GE_MB_REQ_REGISTERS_HI<<8 | GE_MB_REQ_REGISTERS_LOW;
		
		GE_MB_TxBuffer[0] = GE_MB_RxBuffer[0];       // ���ػش����ݵ�ַ
		GE_MB_TxBuffer[1] = GE_MB_RxBuffer[1];       // ���ػش����ݹ�����
		GE_MB_TxBuffer[2] = GE_MB_REQ_REGISTERS * 2; // ���ػش������ֽ���
		
		/* ����Ҫ��ȡ�ļĴ������� */
		while(GE_MB_REQ_REGISTERS--)
		{
			//���ݴ�GE_MB_TxBuffer[3]��ʼ
			GE_MB_TxBuffer[index++] = ((*(uint16_t *)(GE_MB_REQ_STAR_REGISTERS)) & 0xff00) >> 8;
			GE_MB_TxBuffer[index++] = ( *(uint16_t *)(GE_MB_REQ_STAR_REGISTERS)) & 0x00ff;
			GE_MB_REQ_STAR_REGISTERS += 0x0004;
		} 
		
		/* ����CRCУ���� */
		crc_temp = GE_BSP_MB_CRC(GE_MB_TxBuffer, index);
		GE_MB_TxBuffer[index++] = crc_temp & 0x00FF;
		GE_MB_TxBuffer[index++] = crc_temp >> 8;
		
		GE_MB_Data_Feedback(index, 0, GE_MB_TxBuffer);
	}
	else                           // ���ʷǷ��Ĵ�����Ӧ����Ч���ݵ�ַ
	{
		GE_MB_WarningFunCode_Feedback(GE_MB_Invalid_Data_Addr);
	}
}


void  GE_MB_FUNC_WRITE_SINGLE_REGISTERS(void)
{
	uint8_t  index = 0;
	
	// �ж�д���ݼĴ�����ַ�Ƿ�Ϸ�
	if((GE_MB_RxBuffer[2] == 0x09) && (GE_MB_RxBuffer[3] <= 0x11))
	{
		// д�뱸�ݼĴ���ֵ
		//BKP_WriteBackupRegister((GE_MB_RxBuffer[3] * 4) + 0x0040, (GE_MB_RxBuffer[4] << 8) | GE_MB_RxBuffer[5]);
		BKP_WriteBackupRegister((GE_MB_RxBuffer[3] * 4), (GE_MB_RxBuffer[4] << 8) | GE_MB_RxBuffer[5]);
		
		// ���ػش�����
		while(GE_MB_RxCounter--)
		{
			GE_MB_TxBuffer[index] = GE_MB_RxBuffer[index];
			index++;
		}
		
		// ��������
		GE_MB_Data_Feedback(index, 0, GE_MB_TxBuffer);
		
		// �޸���Ӧ���ݼĴ�������
		if(GE_MB_RxBuffer[3] == 2)
		{
			BSP_MB_USART_1_Mode_Init(BKP_ReadBackupRegister(TransBaudRate));
			//BSP_MB_USART_2_Mode_Init(BKP_ReadBackupRegister(TransBaudRate));
		}
		// �޸���Ӧ���ݼĴ�������
		if(GE_MB_RxBuffer[3] > 0x0B)
		{
			// RTC ʱ���޸�ʱ��
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
	
	// �ж�д���ݼĴ�����ַ�Ƿ�Ϸ�
	if((GE_MB_RxBuffer[2] == 0x09) && (GE_MB_RxBuffer[3] <= 0x11))
	{
		// ����д���ݼĴ�����
		GE_MB_REQ_REGISTERS = GE_MB_RxBuffer[4] << 8 | GE_MB_RxBuffer[5];
		
		// �ж�д���ݼĴ������Ƿ�Ϸ�
		if((GE_MB_REQ_REGISTERS != GE_MB_RxBuffer[6]) || \
			 ((GE_MB_RxBuffer[3] + GE_MB_REQ_REGISTERS) > 0x12) || (GE_MB_REQ_REGISTERS == 0))
		{
			GE_MB_WarningFunCode_Feedback(GE_MB_Invalid_Data_Addr);
		}
		else
		{
			// ����д���ݼĴ�����ʼ��ַ
			GE_MB_REQ_STAR_REGISTERS = (GE_MB_RxBuffer[3] * 4) + 0x0040;
			
			// ���±��ݼĴ���ֵ
			while(GE_MB_RxBuffer[6]--)
			{
				BKP_WriteBackupRegister(GE_MB_REQ_STAR_REGISTERS, (GE_MB_RxBuffer[index] << 8) | GE_MB_RxBuffer[index+1]);
				GE_MB_REQ_STAR_REGISTERS += 0x0004;
				index += 2;
			}
			
			// ���ػش�����
			GE_MB_TxBuffer[0] = BKP_ReadBackupRegister(SlaveAddress);
			GE_MB_TxBuffer[1] = GE_MB_RxBuffer[1];
			GE_MB_TxBuffer[2] = GE_MB_RxBuffer[2];
			GE_MB_TxBuffer[3] = GE_MB_RxBuffer[3];
			GE_MB_TxBuffer[4] = GE_MB_RxBuffer[4]; //(BKP_ReadBackupRegister(ConnectLineRes) & 0xFF00)>>8;
			GE_MB_TxBuffer[5] = GE_MB_RxBuffer[5]; //BKP_ReadBackupRegister(ConnectLineRes) & 0x00FF;
			
			/* ����CRCУ���� */
		  crc_temp = GE_BSP_MB_CRC(GE_MB_TxBuffer, 6);
		  GE_MB_TxBuffer[6] = crc_temp & 0x00FF; //(BKP_ReadBackupRegister(TransBaudRate) & 0xFF00)>>8;
		  GE_MB_TxBuffer[7] = crc_temp >> 8; //BKP_ReadBackupRegister(TransBaudRate) & 0x00FF;
			
			// ��������
			GE_MB_Data_Feedback(8, 0, GE_MB_TxBuffer);
			
			// �޸���Ӧ���ݼĴ�������
			if((GE_MB_RxBuffer[3] <= 2) && (GE_MB_RxBuffer[3] + GE_MB_REQ_REGISTERS) > 0x02)
			{
				BSP_MB_USART_1_Mode_Init(BKP_ReadBackupRegister(TransBaudRate));
				//BSP_MB_USART_2_Mode_Init(BKP_ReadBackupRegister(TransBaudRate));
			}
			
			// �޸���Ӧ���ݼĴ�������
			if((GE_MB_RxBuffer[3] + GE_MB_REQ_REGISTERS) > 0x0B)
			{
				// RTC ʱ���޸�ʱ��
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
	// ARM USART �� 485 USART ���ɵ��ø� �澯�뷵�� ����
	if(GE_MB_FrmeReceiveDone_Flag == GE_MB_485)
	{
		GE_MB_485_SEND_DATA;
		while(leng--)
		{
			USART_SendData(GE_MB_485_USART_NUM, data[cnt++]);
			while(USART_GetFlagStatus(GE_MB_485_USART_NUM, USART_FLAG_TXE) == RESET);
		}
		// û�в�ѯ�ñ�־λ�����ᶪʧ���һ���ֽ����ݵĴ��䣬��Ϊ485оƬ�ܿ�תΪ����ģʽ��
		// ������ʱ�����������䣬���ή��Ч�ʡ�
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
