/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                              Fendou
*                                        Evaluation Board
*
* Filename      : bsp_can.c
* Version       : V1.00
* Programmer(s) : WEN
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  "bsp_can.h" 

CanTxMsg TxMessage;
CanRxMsg RxMessage;

__IO  uint8_t  BSP_CAN_Rx_Flag = 1;


static void CAN_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*����ʱ������*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��	                   											 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��PA11,PA12
	
	//���Ÿ���ӳ������
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //GPIOA11����ΪCAN1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); //GPIOA12����ΪCAN1
}

static void CAN_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			   //�����ȼ�Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


static void CAN_Mode_Config(void)
{
  CAN_InitTypeDef CAN_InitStructure;
	/************************CANͨ�Ų�������**********************************/
	/*CAN�Ĵ�����ʼ��*/
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	/*CAN��Ԫ��ʼ��*/
	CAN_InitStructure.CAN_TTCM=DISABLE;			       //MCR-TTCM  ʱ�䴥��ͨ��ģʽʹ��
  CAN_InitStructure.CAN_ABOM=ENABLE;			       //MCR-ABOM  �Զ����߹��� 
  CAN_InitStructure.CAN_AWUM=ENABLE;			       //MCR-AWUM  �Զ�����ģʽ
  CAN_InitStructure.CAN_NART=DISABLE;			       //MCR-NART  ��ֹ�����Զ��ش�	  DISABLE-�Զ��ش�
  CAN_InitStructure.CAN_RFLM=DISABLE;			       //MCR-RFLM  ����FIFO ����ģʽ  DISABLE-���ʱ�±��ĻḲ��ԭ�б���  
  CAN_InitStructure.CAN_TXFP=DISABLE;			       //MCR-TXFP  ����FIFO���ȼ� DISABLE-���ȼ�ȡ���ڱ��ı�ʾ�� 
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  //��������ģʽ
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;			   //BTR-SJW ����ͬ����Ծ��� 2��ʱ�䵥Ԫ
  CAN_InitStructure.CAN_BS1=CAN_BS1_3tq;			   //BTR-TS1 ʱ���1 ռ����6��ʱ�䵥Ԫ
  CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;			   //BTR-TS1 ʱ���2 ռ����3��ʱ�䵥Ԫ
  CAN_InitStructure.CAN_Prescaler =14;			     //BTR-BRP �����ʷ�Ƶ��  ������ʱ�䵥Ԫ��ʱ�䳤�� 36/(1+6+3)/4=0.8Mbps
	CAN_Init(CAN1, &CAN_InitStructure);
}

static void CAN_Filter_Config(void)
{
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

	/*CAN��������ʼ��*/
	CAN_FilterInitStructure.CAN_FilterNumber=0;										//��������0
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//�����ڱ�ʶ������λģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//������λ��Ϊ����32λ��
	/* ʹ�ܱ��ı�ʾ�����������ձ�ʾ�������ݽ��бȶԹ��ˣ���չID�������µľ����������ǵĻ��������FIFO0�� */

  CAN_FilterInitStructure.CAN_FilterIdHigh= (((u32)0x5000<<3)&0xFFFF0000)>>16;				//Ҫ���˵�ID��λ 
  CAN_FilterInitStructure.CAN_FilterIdLow= (((u32)0x5000<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF; //Ҫ���˵�ID��λ 
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0xFFFF;			//��������16λÿλ����ƥ��
  CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0xFFFF;			//��������16λÿλ����ƥ��
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FilterFIFO0;				//��������������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);
	/*CANͨ���ж�ʹ��*/
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}


void BSP_CAN_Init(void)
{
	CAN_GPIO_Config();
  CAN_NVIC_Config();
  CAN_Mode_Config();
  CAN_Filter_Config(); 
}


void BSP_CAN_SetTxMsg(uint32_t ID, uint8_t cmd)
{
  TxMessage.ExtId   = ID;
  TxMessage.IDE     = CAN_ID_EXT;
  TxMessage.RTR     = CAN_RTR_DATA;
  TxMessage.DLC     = 1;
  TxMessage.Data[0] = cmd;
	
	CAN_Transmit(CAN1, &TxMessage);	
}
