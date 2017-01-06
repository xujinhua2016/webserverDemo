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
	/*使能USART1时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	/*使能GPIOA,GPIOG时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOG, ENABLE);
	
	  //串口2引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA3复用为USART2
	
	/*PG8推挽输出，485模式控制*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
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
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA2，PA3
	
	GE_MB_485_REV_DATA;			//将rs485使能信号拉低，作为从机，接收数据
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
//	uint16_t IDLE_Clear = IDLE_Clear; // 用于清楚总线空闲标志变量
//	
//	if(MB_TimeExpireT3_5) // 等待3.5个字符间隔之后再次接收数据
//	{
////		if(GE_MB_FrmeReceiveDone_Flag == GE_MB_ARM)
////		{
////			if(USART_GetITStatus(GE_MB_485_USART_NUM, USART_IT_RXNE) != RESET)
////			{
////				MB_Slave_Busy_RxBuffer[MB_Slave_Busy_MB_RxCounter++] = GE_MB_485_USART_NUM->DR; // 读取数据
////			}
////			
////			if(USART_GetITStatus(GE_MB_485_USART_NUM, USART_IT_IDLE) != RESET)
////			{
////				IDLE_Clear = GE_MB_485_USART_NUM->SR; // 清楚总线空闲标志位
////				IDLE_Clear = GE_MB_485_USART_NUM->DR; // 清楚总线空闲标志位
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
//				GE_MB_RxBuffer[GE_MB_RxCounter++] = GE_MB_485_USART_NUM->DR; // 读取数据
//			}
//			
//			if(USART_GetITStatus(GE_MB_485_USART_NUM, USART_IT_IDLE) != RESET)
//			{
//				IDLE_Clear = GE_MB_485_USART_NUM->SR; // 清楚总线空闲标志位
//				IDLE_Clear = GE_MB_485_USART_NUM->DR; // 清楚总线空闲标志位
//				
//				GE_MB_FrmeReceiveDone_Flag = GE_MB_485;  // 一帧数据接收完标志位
//				
//				MB_TimeExpireT3_5 = 0;   // 一帧数据接收后，开始进入3.5个字符间隔计时
//				BSP_MB_TIMER_1_Enable(); // 开启定时器
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
	
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PA9，PA10
	
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
	uint16_t IDLE_Clear = IDLE_Clear; // 用于清楚总线空闲标志变量
	
	if(MB_TimeExpireT3_5) // 等待3.5个字符间隔之后再次接收数据
	{
		if(GE_MB_FrmeReceiveDone_Flag == GE_MB_485)
		{
			if(USART_GetITStatus(GE_MB_ARM_USART_NUM, USART_IT_RXNE) != RESET)
			{
				MB_Slave_Busy_RxBuffer[MB_Slave_Busy_MB_RxCounter++] = GE_MB_ARM_USART_NUM->DR; // 读取数据
			}
			
			if(USART_GetITStatus(GE_MB_ARM_USART_NUM, USART_IT_IDLE) != RESET)
			{
				IDLE_Clear = GE_MB_ARM_USART_NUM->SR; // 清楚总线空闲标志位
				IDLE_Clear = GE_MB_ARM_USART_NUM->DR; // 清楚总线空闲标志位
				
				MB_Slave_Busy_TxBuffer[0] = MB_Slave_Busy_RxBuffer[0];					//第一个字节，从机地址
				MB_Slave_Busy_TxBuffer[1] = MB_Slave_Busy_RxBuffer[1] + 0x80;		//返回出错功能码，+0x80
				MB_Slave_Busy_TxBuffer[2] = GE_MB_Slave_Busy;										//第三个字节，从机忙
							
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
				GE_MB_RxBuffer[GE_MB_RxCounter++] = GE_MB_ARM_USART_NUM->DR; // 读取数据
			}
			
			if(USART_GetITStatus(GE_MB_ARM_USART_NUM, USART_IT_IDLE) != RESET)
			{
				IDLE_Clear = GE_MB_ARM_USART_NUM->SR; // 清楚总线空闲标志位
				IDLE_Clear = GE_MB_ARM_USART_NUM->DR; // 清楚总线空闲标志位
				
				GE_MB_FrmeReceiveDone_Flag = GE_MB_ARM;  // 一帧数据接收完标志位
				
				MB_TimeExpireT3_5 = 0;   // 一帧数据接收后，开始进入3.5个字符间隔计时
				BSP_MB_TIMER_1_Enable(); // 开启定时器
			}
		}
	}
}

