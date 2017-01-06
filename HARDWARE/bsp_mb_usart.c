#include  "bsp_mb.h"
#include  "bsp_mb_usart.h"
#include  "bsp_mb_timer.h"

#include  "bsp_mem.h"
#include  "bsp_led.h"

extern  uint8_t  GE_MB_RxBuffer[150];
extern  uint8_t  GE_MB_RxCounter;
extern  uint8_t  GE_MB_FrmeReceiveDone_Flag;
extern  uint8_t  MB_TimeExpireT3_5;

uint8_t  MB_Slave_Busy_RxBuffer[150] = {0x00};
uint8_t  MB_Slave_Busy_TxBuffer[150] = {0x00};
uint8_t  MB_Slave_Busy_MB_RxCounter = 0;


void GE_MB_USART_Init(void)
{
	BSP_MB_USART_1_Init();
	//BSP_MB_USART_2_Init();
}
 

void BSP_MB_USART_1_Init(void)
{
	BSP_MB_USART_1_GPIO_Init();
	BSP_MB_USART_1_Mode_Init(BKP_ReadBackupRegister(TransBaudRate));
	BSP_MB_USART_1_NVIC_Init();
}


void BSP_MB_USART_1_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*ʹ��USART1ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	/*ʹ��GPIOA,GPIOGʱ��*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOG, ENABLE);
	
	  //����2���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA2����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA3����ΪUSART2
	
	/*PG8���������485ģʽ����*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART2    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA2��PA3
	
	GE_MB_485_REV_DATA;			//��rs485ʹ���ź����ͣ���Ϊ�ӻ�����������
}


void BSP_MB_USART_1_Mode_Init(uint16_t baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_2;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(GE_MB_485_USART_NUM, &USART_InitStructure); 
	
	USART_ITConfig(GE_MB_485_USART_NUM, USART_IT_RXNE, ENABLE);
	USART_ITConfig(GE_MB_485_USART_NUM, USART_IT_IDLE, ENABLE);
	
	USART_ClearFlag(GE_MB_485_USART_NUM, USART_FLAG_TXE);
	
	USART_Cmd(GE_MB_485_USART_NUM, ENABLE);
}


void BSP_MB_USART_1_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}


//void USART1_IRQHandler(void)
//{
//	uint16_t IDLE_Clear = IDLE_Clear; // ����������߿��б�־����
//	
//	if(MB_TimeExpireT3_5) // �ȴ�3.5���ַ����֮���ٴν�������
//	{
////		if(GE_MB_FrmeReceiveDone_Flag == GE_MB_ARM)
////		{
////			if(USART_GetITStatus(GE_MB_485_USART_NUM, USART_IT_RXNE) != RESET)
////			{
////				MB_Slave_Busy_RxBuffer[MB_Slave_Busy_MB_RxCounter++] = GE_MB_485_USART_NUM->DR; // ��ȡ����
////			}
////			
////			if(USART_GetITStatus(GE_MB_485_USART_NUM, USART_IT_IDLE) != RESET)
////			{
////				IDLE_Clear = GE_MB_485_USART_NUM->SR; // ������߿��б�־λ
////				IDLE_Clear = GE_MB_485_USART_NUM->DR; // ������߿��б�־λ
////				
////				MB_Slave_Busy_TxBuffer[0] = MB_Slave_Busy_RxBuffer[0];
////				MB_Slave_Busy_TxBuffer[1] = MB_Slave_Busy_RxBuffer[1] + 0x80;
////				MB_Slave_Busy_TxBuffer[2] = GE_MB_Slave_Busy;
////							
////				MB_Slave_Busy_TxBuffer[3] = GE_BSP_MB_CRC(MB_Slave_Busy_TxBuffer, 3) & 0x00FF;
////				MB_Slave_Busy_TxBuffer[4] = GE_BSP_MB_CRC(MB_Slave_Busy_TxBuffer, 3) >> 8;
////				
////				GE_MB_Data_Feedback(5, 0, MB_Slave_Busy_TxBuffer);
////				
////				MB_Slave_Busy_MB_RxCounter = 0;
////			}
////		}
////		else
////		{
//			if(USART_GetITStatus(GE_MB_485_USART_NUM, USART_IT_RXNE) != RESET)
//			{
//				GE_MB_RxBuffer[GE_MB_RxCounter++] = GE_MB_485_USART_NUM->DR; // ��ȡ����
//			}
//			
//			if(USART_GetITStatus(GE_MB_485_USART_NUM, USART_IT_IDLE) != RESET)
//			{
//				IDLE_Clear = GE_MB_485_USART_NUM->SR; // ������߿��б�־λ
//				IDLE_Clear = GE_MB_485_USART_NUM->DR; // ������߿��б�־λ
//				
//				GE_MB_FrmeReceiveDone_Flag = GE_MB_485;  // һ֡���ݽ������־λ
//				
//				MB_TimeExpireT3_5 = 0;   // һ֡���ݽ��պ󣬿�ʼ����3.5���ַ������ʱ
//				BSP_MB_TIMER_1_Enable(); // ������ʱ��
//			}
//		//}
//	}
//}


void BSP_MB_USART_2_Init(void)
{
	BSP_MB_USART_2_GPIO_Init();
	BSP_MB_USART_2_Mode_Init(BKP_ReadBackupRegister(TransBaudRate));
	BSP_MB_USART_2_NVIC_Init();
}


void BSP_MB_USART_2_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PA9��PA10
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void BSP_MB_USART_2_Mode_Init(uint16_t baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_2;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	
	USART_ClearFlag(USART3,USART_FLAG_RXNE);   
  USART_ClearITPendingBit(USART3, USART_IT_RXNE); 
	
	USART_Cmd(USART3, ENABLE);
}


void BSP_MB_USART_2_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}


void USART3_IRQHandler(void)
{
	uint16_t IDLE_Clear = IDLE_Clear; // ����������߿��б�־����
	
	if(MB_TimeExpireT3_5) // �ȴ�3.5���ַ����֮���ٴν�������
	{
		if(GE_MB_FrmeReceiveDone_Flag == GE_MB_485)
		{
			if(USART_GetITStatus(GE_MB_ARM_USART_NUM, USART_IT_RXNE) != RESET)
			{
				MB_Slave_Busy_RxBuffer[MB_Slave_Busy_MB_RxCounter++] = GE_MB_ARM_USART_NUM->DR; // ��ȡ����
			}
			
			if(USART_GetITStatus(GE_MB_ARM_USART_NUM, USART_IT_IDLE) != RESET)
			{
				IDLE_Clear = GE_MB_ARM_USART_NUM->SR; // ������߿��б�־λ
				IDLE_Clear = GE_MB_ARM_USART_NUM->DR; // ������߿��б�־λ
				
				MB_Slave_Busy_TxBuffer[0] = MB_Slave_Busy_RxBuffer[0];					//��һ���ֽڣ��ӻ���ַ
				MB_Slave_Busy_TxBuffer[1] = MB_Slave_Busy_RxBuffer[1] + 0x80;		//���س������룬+0x80
				MB_Slave_Busy_TxBuffer[2] = GE_MB_Slave_Busy;										//�������ֽڣ��ӻ�æ
							
				MB_Slave_Busy_TxBuffer[3] = GE_BSP_MB_CRC(MB_Slave_Busy_TxBuffer, 3) & 0x00FF;
				MB_Slave_Busy_TxBuffer[4] = GE_BSP_MB_CRC(MB_Slave_Busy_TxBuffer, 3) >> 8;
				
				GE_MB_Data_Feedback(5, 0, MB_Slave_Busy_TxBuffer);
				
				MB_Slave_Busy_MB_RxCounter = 0;
			}
		}
		else
		{
			if(USART_GetITStatus(GE_MB_ARM_USART_NUM, USART_IT_RXNE) != RESET)
			{
				GE_MB_RxBuffer[GE_MB_RxCounter++] = GE_MB_ARM_USART_NUM->DR; // ��ȡ����
			}
			
			if(USART_GetITStatus(GE_MB_ARM_USART_NUM, USART_IT_IDLE) != RESET)
			{
				IDLE_Clear = GE_MB_ARM_USART_NUM->SR; // ������߿��б�־λ
				IDLE_Clear = GE_MB_ARM_USART_NUM->DR; // ������߿��б�־λ
				
				GE_MB_FrmeReceiveDone_Flag = GE_MB_ARM;  // һ֡���ݽ������־λ
				
				MB_TimeExpireT3_5 = 0;   // һ֡���ݽ��պ󣬿�ʼ����3.5���ַ������ʱ
				BSP_MB_TIMER_1_Enable(); // ������ʱ��
			}
		}
	}
}

