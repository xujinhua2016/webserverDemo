/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*
* Filename      : My_UI.c
* Version       : V1.00
* Programmer(s) : WEN&XU
*
* Note          : Command:
*
*********************************************************************************************************
*/

#include  "includes.h"
#include  "GE_PM.h"

static uint8_t BSP_findRecDataCmd(uint32_t ID,uint8_t cmd)
{
	uint8_t IDLow = ID & 0xFF;
	RxMessage.Data[2] = 0x00;
	RxMessage.Data[3] = 0x00;
	BSP_CAN_TimOut  = 0;
	
	while(!((RxMessage.Data[2] == cmd) && (RxMessage.Data[3] == IDLow) || (BSP_CAN_TimOut == 3)))
	{
		BSP_CAN_Rx_Flag = 1;
		
		BSP_CAN_SetTxMsg(ID, cmd);
		
		OSTimeDlyHMSM(0, 0, 0, 15);
		
		if(BSP_CAN_Rx_Flag == 0)                                    
		{
			if((RxMessage.Data[2] == cmd) && (RxMessage.Data[3] == IDLow))                          
			{
					return 1;
			}
			else
			{
					return 0;     
			}
		}
		BSP_CAN_TimOut++;
	}
	
	return 0;
}

static void  BSP_CAN_Detec(void)
{
	/************************************* A *************************************/
	
	BSP_findRecDataCmd(BSP_CAN_A_ID,0XFF) ? (BSP_BAT_CAN_BAD_Num &= (~(1<<0))) : (BSP_BAT_CAN_BAD_Num |= (1<<0));
	
	/************************************* B *************************************/
	BSP_findRecDataCmd(BSP_CAN_B_ID,0XFF) ? (BSP_BAT_CAN_BAD_Num &= (~(1<<1))) : (BSP_BAT_CAN_BAD_Num |= (1<<1));
	
	/************************************* C *************************************/
	BSP_findRecDataCmd(BSP_CAN_C_ID,0XFF) ? (BSP_BAT_CAN_BAD_Num &= (~(1<<2))) : (BSP_BAT_CAN_BAD_Num |= (1<<2));
	
	/************************************* D *************************************/
	BSP_findRecDataCmd(BSP_CAN_D_ID,0XFF) ? (BSP_BAT_CAN_BAD_Num &= (~(1<<3))) : (BSP_BAT_CAN_BAD_Num |= (1<<3));
}

//para: num :0-23
static void BSP_getBatV(int num)
{
	uint8_t m = 0,n = 0,cmd;
	m = num / 6;//�Բ�������6��ȡ��
	n = num % 6;//�Բ�������6��ȡ��
	
	cmd = 0X00 | (n+1);
	
	switch(m)
	{
		case 0:
			BSP_findRecDataCmd(BSP_CAN_A_ID,cmd)?(NoConvBatVol[num] = (RxMessage.Data[0]<<8) | RxMessage.Data[1]):(NoConvBatVol[num] = 0);
			break;
		case 1:
			BSP_findRecDataCmd(BSP_CAN_B_ID,cmd)?(NoConvBatVol[num] = (RxMessage.Data[0]<<8) | RxMessage.Data[1]):(NoConvBatVol[num] = 0);
			break;
		case 2:
			BSP_findRecDataCmd(BSP_CAN_C_ID,cmd)?(NoConvBatVol[num] = (RxMessage.Data[0]<<8) | RxMessage.Data[1]):(NoConvBatVol[num] = 0);
			break;
		case 3:
			BSP_findRecDataCmd(BSP_CAN_D_ID,cmd)?(NoConvBatVol[num] = (RxMessage.Data[0]<<8) | RxMessage.Data[1]):(NoConvBatVol[num] = 0);
			break;
		
		default:
			break;
	}
}


//para: num :0-23
static void BSP_getBatR(int num)
{
	uint8_t m = 0,n = 0,cmd;
	m = num / 6;//�Բ�������6��ȡ��
	n = num % 6;//�Բ�������6��ȡ��
	
	cmd = 0X50 | (n+1);
	
	switch(m)
	{
		case 0:
			BSP_findRecDataCmd(BSP_CAN_A_ID,cmd)?(NoConvBatRes[num] = (RxMessage.Data[0]<<8) | RxMessage.Data[1]):(NoConvBatRes[num] = 0);
			break;
		case 1:
			BSP_findRecDataCmd(BSP_CAN_B_ID,cmd)?(NoConvBatRes[num] = (RxMessage.Data[0]<<8) | RxMessage.Data[1]):(NoConvBatRes[num] = 0);
			break;
		case 2:
			BSP_findRecDataCmd(BSP_CAN_C_ID,cmd)?(NoConvBatRes[num] = (RxMessage.Data[0]<<8) | RxMessage.Data[1]):(NoConvBatRes[num] = 0);
			break;
		case 3:
			BSP_findRecDataCmd(BSP_CAN_D_ID,cmd)?(NoConvBatRes[num] = (RxMessage.Data[0]<<8) | RxMessage.Data[1]):(NoConvBatRes[num] = 0);
			break;
		
		default:
			break;
	}
}


//para: num :0-23
static void BSP_sendBatB(int num)
{
	uint8_t m = 0,n = 0,cmd;
	m = num / 6;//�Բ�������6��ȡ��
	n = num % 6;//�Բ�������6��ȡ��
	
	cmd = 0XA0 | (n+1);
	
	switch(m)
	{
		case 0:
			BSP_findRecDataCmd(BSP_CAN_A_ID,cmd);
			break;
		case 1:
			BSP_findRecDataCmd(BSP_CAN_B_ID,cmd);
			break;
		case 2:
			BSP_findRecDataCmd(BSP_CAN_C_ID,cmd);
			break;
		case 3:
			BSP_findRecDataCmd(BSP_CAN_D_ID,cmd);
			break;
		
		default:
			break;
	}
}


static void BSP_getVol(void)
{
	uint8_t i;
	if( (BSP_BAT_CAN_BAD_Num & 0x01) == 0)
	{
		for(i = 0; i < 6; i++)
		{
			BSP_getBatV(i);
		}
	}
	if((BSP_BAT_CAN_BAD_Num & 0x02) == 0)
	{
		for(i = 6; i < 12; i++)
		{
			BSP_getBatV(i);
		}
	}
	if((BSP_BAT_CAN_BAD_Num & 0x04) == 0)
	{
		for(i = 12; i < 18; i++)
		{
			BSP_getBatV(i);
		}
	}
	if((BSP_BAT_CAN_BAD_Num & 0x08) == 0)
	{
		for(i = 18; i < 24; i++)
		{
			BSP_getBatV(i);
		}
	}
	
}

static void BSP_getRes(void)
{
	/**
	*�����жϵ���Ƿ��о���ָ��
	*���Ҫ�жϵ�صĹ���״̬�Ƿ�������ֵ
	*/
	uint8_t i;
	
	if (BAT_Vol_Res_Dect_Flag)		//��ѯ�Ƿ���Ҫ��������ı�־
	{
		if(BSP_BAT_Balan_Flag != 1)	//���û�д��ھ���������
		{
			//��ѯ��ص�ѹ�Ƿ��������Ĺ�����Χ��
			//�����������ָ��,�������ȫ�����Ի�ȫ�������Է���
			if(((BatVolWarnBit000_015 | BatVolWarnBit016_031) == 0))//��ع���״̬����
			{
				for(i = 0; i < 24; i++)
				{
					BSP_getBatR(i);
				}
			}
			else			//��ع���״̬������
			{;}
			BAT_Vol_Res_Dect_Flag = 0;	
		}
		else
		{;}	
	}
	else
	{;}
}

static void BSP_sendBala(void)
{
	uint8_t i, j;
	uint16_t tempBatVol[24];
	/**
	*�����жϵ���Ƿ񷢲��������ָ��
	*�жϵ�صĹ���״̬�Ƿ�������ֵ
	*���ݴ������жϣ�������͵�ѹ��أ�����һ������ֵ��
	*����ָ����󣬿���led2  LED2_ON
	*/
	if(BSP_BAT_Balan_Flag)
	{
		if(BAT_Vol_Res_Dect_Flag != 1)
		{		
			//�жϵ�ѹ�Ĺ�����Χ�Ƿ�����		//(BatVolWarnBit000_015 | BatVolWarnBit016_031) == 0)  XX ���иߵ�ѹ��ҲӦ�ÿ�����������ؿ������⿪��
			
			//�ҵ���͵�ص�ѹ�ͱ��
			//��ѯ��͵�ѹ��ƽ����ѹ֮��Ĳ�ֵ����͵�ѹ�Ƿ������͸澯ֵ
			//�������ڵ�ؾ���ָ�����led2		
			
				for(i = 0; i < 24; i++)
				{
					tempBatVol[i] = GE_BAT_Vol[i];
				}
			
				j = 0;		//���ڱ�����͵�ѹ��صı��
				
				for(i = 0; i < 24; i++)
				{
					if(tempBatVol[j] > GE_BAT_Vol[i])
					{
						j = i;
						tempBatVol[j] = GE_BAT_Vol[i];
					}
					else
					{
						tempBatVol[j] = tempBatVol[j];
					}
				}
			
			//��͵�ص�ѹ������͵�ѹ�澯ֵ����ƽ����ѹ����͵�ѹ֮��Ĳ�ֵ�����趨����ֵ
			if((tempBatVol[j] > BKP_ReadBackupRegister(LowWarnVolSinBat)) && (AverageVolVal - tempBatVol[j]) > D_VALUE)		
			{
				//���;���ָ��
				BSP_sendBatB(j);
				
				LED2_ON;
			}
				
			BSP_BAT_Balan_Flag = 0;
		}
		
		
	}
	else
	{
		;
	}
}
static void  BSP_getI_T(void)
{	
	/************************************* Current *************************************/
	BSP_findRecDataCmd(BSP_CAN_E_ID,0XF1)?(ChargeCurrentVal = RxMessage.Data[0]<<8 | RxMessage.Data[1]):(ChargeCurrentVal = 0);
		
	/************************************ Temperature **********************************/
	BSP_findRecDataCmd(BSP_CAN_E_ID,0XF2)?(Temperature = RxMessage.Data[0]<<8 | RxMessage.Data[1]):(Temperature = 0);
}



static void  BSP_Voltage_Proc(void)
{
	int i, j, k, iMax;
	uint32_t Ave_Vol;
	uint16_t temp, vMax;
	
	//���Ƚ�����ת��Ϊģ���ѹֵ
	for(i = 0; i < 24; i++)
	{
		if(i<6)
		{
			GE_BAT_Vol[i] = NoConvBatVol[i] * SLAVE_VREF1 /4095;		//ת���ɵ�ѹģ����ֵ
		}
		else if(i>=6 && i<12)
		{
			GE_BAT_Vol[i] = NoConvBatVol[i] * SLAVE_VREF2 /4095;		//ת���ɵ�ѹģ����ֵ
		}
		else if(i>=12 && i<18)
		{
			GE_BAT_Vol[i] = NoConvBatVol[i] * SLAVE_VREF3 /4095;		//ת���ɵ�ѹģ����ֵ
		}
		else if(i>=18 && i<24)
		{
			GE_BAT_Vol[i] = NoConvBatVol[i] * SLAVE_VREF4 /4095;		//ת���ɵ�ѹģ����ֵ
		}
		else
		{
			;
		}
	}
	
	/************** Average voltage **************/
	Ave_Vol = 0;
	for(i=0;i<24;i++)	
	{
		Ave_Vol = Ave_Vol + GE_BAT_Vol[i];
	}
	TotalBatVol = Ave_Vol;					//���µ�ص�ѹ�ܺ�
	AverageVolVal = Ave_Vol / 24;		//���µ��ƽ����ѹ
	
	if(AverageVolVal > BKP_ReadBackupRegister(HigWarnVolSinBat))
	{
		GroBatWarn = GroBatWarn | (1<<0);			//�����ƽ����ѹ������߸澯����ص�ѹHigWarnVolSinBatʱ��GroBatWarn�Ĵ���bit 0 set 1
	}
	else
	{
		GroBatWarn = GroBatWarn & (~(1<<0));
	}
	
	/************** Max and Min voltage **************/
	
	for (i=0;i<24;i++)	
	{
		BSP_BAT_Vol_Min_To_Max[i] = GE_BAT_Vol[i];
		BSP_BAT_Vol_Min_To_Max_Num[i] = i;
	}
	//����ð�ݷ�����24�ڵ�ص����ݽ������������Ұ�˳�򱣴��صı��
	for (i=0;i<24;i++)
	{
		iMax = i;														//���ڱ����صı��
		vMax = BSP_BAT_Vol_Min_To_Max[i];		//���ڱ����ص�ѹֵ
		for (j=i;j<24;j++)
		{
			if (BSP_BAT_Vol_Min_To_Max[j]>vMax)
			{
				iMax = j;
				vMax = BSP_BAT_Vol_Min_To_Max[j];
			}
		}
		temp = BSP_BAT_Vol_Min_To_Max[i];
		BSP_BAT_Vol_Min_To_Max[i] = BSP_BAT_Vol_Min_To_Max[iMax];
		BSP_BAT_Vol_Min_To_Max[iMax] = temp;
		
		k = BSP_BAT_Vol_Min_To_Max_Num[i];
		BSP_BAT_Vol_Min_To_Max_Num[i] = BSP_BAT_Vol_Min_To_Max_Num[iMax];
		BSP_BAT_Vol_Min_To_Max_Num[iMax] = k;
	}
	//��������ڵ�غ�������ڵ�ص����ݺ͵�ر�Ž��и�ֵ����
	HigVolBatVal1 = BSP_BAT_Vol_Min_To_Max[0];
	HigVolBatNum1 = BSP_BAT_Vol_Min_To_Max_Num[0] + 1;
	HigVolBatVal2 = BSP_BAT_Vol_Min_To_Max[1];
	HigVolBatNum2 = BSP_BAT_Vol_Min_To_Max_Num[1] + 1;
	HigVolBatVal3 = BSP_BAT_Vol_Min_To_Max[2];
	HigVolBatNum3 = BSP_BAT_Vol_Min_To_Max_Num[2] + 1;
	LowVolBatVal1 = BSP_BAT_Vol_Min_To_Max[23];
	LowVolBatNum1 = BSP_BAT_Vol_Min_To_Max_Num[23] + 1;
	LowVolBatVal2 = BSP_BAT_Vol_Min_To_Max[22];
	LowVolBatNum2 = BSP_BAT_Vol_Min_To_Max_Num[22] + 1;
	LowVolBatVal3 = BSP_BAT_Vol_Min_To_Max[21];
	LowVolBatNum3 = BSP_BAT_Vol_Min_To_Max_Num[21] + 1;
	
	/************** VPP voltage **************/
	MaxVolDiff = HigVolBatVal1 - LowVolBatVal1;
	if(MaxVolDiff > BKP_ReadBackupRegister(MaxWarnVolDiff))
	{
		GroBatWarn = GroBatWarn | (1<<1);							//�������澯��ѹ��MaxWarnVolDiffʱ��GroBatWarn�Ĵ���bit 1 set 1
	}
	else
	{
		GroBatWarn = GroBatWarn & (~(1<<1));
	}
	
	/******** Warning voltage number *********/
	/********  �����ѹ���棬ʹ�õ�16λ����س��ֵ�ѹ���棬��Ӧ����λ��1  **********/
	for(i=0;i<16;i++)
	{
		if((GE_BAT_Vol[i]>BKP_ReadBackupRegister(HigWarnVolSinBat))\
		 ||(GE_BAT_Vol[i]<BKP_ReadBackupRegister(LowWarnVolSinBat)))
		{
			BatVolWarnBit000_015 = BatVolWarnBit000_015 | (1<<i);
		}
		else
		{
			BatVolWarnBit000_015 = BatVolWarnBit000_015 & (~(1<<i));
		}
	}
	
	for(i=16;i<24;i++)
	{
		if((GE_BAT_Vol[i]>BKP_ReadBackupRegister(HigWarnVolSinBat))\
		 ||(GE_BAT_Vol[i]<BKP_ReadBackupRegister(LowWarnVolSinBat)))
		{
			BatVolWarnBit016_031 = BatVolWarnBit016_031 | (1<<(i-16));
		}
		else
		{
			BatVolWarnBit016_031 = BatVolWarnBit016_031 & (~(1<<(i-16)));
		}
	}
}


static void  BSP_Res_Proc(void)
{
	int i, j, k, iMax;
	uint16_t temp, vMax;
	
	//���Ƚ���ѹ�Ĳ�ֵӳ�䵽����ֵ��
	for(i = 0; i < 24; i++)
	{
		BatIntRes[i] = NoConvBatRes[i]  * REF_RES * 10 / REF_DAC1V;		//ת���������ģ����ֵ
		BSP_BAT_Res_Min_To_Max[i] = BatIntRes[i];
		BSP_BAT_Res_Min_To_Max_Num[i] = i;
	}
	
	/************** Max and Min resistor **************/
	
	//����ð�ݷ�����24�ڵ�ص����ݽ���������
	for (i=0;i<24;i++)
	{
		iMax = i;
		vMax = BSP_BAT_Res_Min_To_Max[i];
		for (j=i;j<24;j++)
		{
			if (BSP_BAT_Res_Min_To_Max[j]>vMax)
			{
				iMax = j;
				vMax = BSP_BAT_Res_Min_To_Max[j];
			}
		}
		temp = BSP_BAT_Res_Min_To_Max[i];
		BSP_BAT_Res_Min_To_Max[i] = BSP_BAT_Res_Min_To_Max[iMax];
		BSP_BAT_Res_Min_To_Max[iMax] = temp;
		
		k = BSP_BAT_Res_Min_To_Max_Num[i];
		BSP_BAT_Res_Min_To_Max_Num[i] = BSP_BAT_Res_Min_To_Max_Num[iMax];
		BSP_BAT_Res_Min_To_Max_Num[iMax] = k;
	}

	MaxResBatVal = BSP_BAT_Res_Min_To_Max[0];
	MaxResBatNum = BSP_BAT_Res_Min_To_Max_Num[0] + 1;
	
	if(MaxResBatVal > BKP_ReadBackupRegister(MaxWarnRes))
	{
		GroBatWarn = GroBatWarn | (1<<6);							//������߸澯����MaxWarnResʱ��GroBatWarn�Ĵ���bit 6 set 1
	}
	else
	{
		GroBatWarn = GroBatWarn & (~(1<<6));
	}

	/**** Warning internal resistor number ***/
/*******  ���������辯�棬ʹ�õ�16λ����س��ֵ�ѹ���棬��Ӧ����λ��1  *******/
	for(i=0;i<16;i++)
	{
		if(BatIntRes[i]>BKP_ReadBackupRegister(MaxWarnRes))
		{
			BatResWarnBit000_015 = BatResWarnBit000_015 | (1<<i);
		}
		else
		{
			BatResWarnBit000_015 = BatResWarnBit000_015 & (~(1<<i));
		}
	}
	
	for(i=16;i<24;i++)
	{
		if(BatIntRes[i]>BKP_ReadBackupRegister(MaxWarnRes))
		{
			BatResWarnBit016_031 = BatResWarnBit016_031 | (1<<(i-16));
		}
		else
		{
			BatResWarnBit016_031 = BatResWarnBit016_031 & (~(1<<(i-16)));
		}
	}
}

static void  BSP_I_T_Proc(void)
{
	/************************************* Current *************************************/
	if(ChargeCurrentVal & 0x8000)
	{
		if((ChargeCurrentVal & 0x7FFF) > BKP_ReadBackupRegister(MaxWarnChargeCurVal))
		{
			GroBatWarn = GroBatWarn | (1<<2);//�������澯������MaxWarnChargeCurValʱ���������ؾ���Ĵ���GroBatWarn��bit2 set 1
		}
		else
		{
			GroBatWarn = GroBatWarn & (~(1<<2));
		}
	}
	else
	{
		if((ChargeCurrentVal & 0x7FFF) > BKP_ReadBackupRegister(MaxWarnDischargeCurVal))
		{
			GroBatWarn = GroBatWarn | (1<<3);//�������澯�ŵ����MaxWarnDischargeCurValʱ���������ؾ���Ĵ���GroBatWarn��bit3 set 1
		}
		else
		{
			GroBatWarn = GroBatWarn & (~(1<<3));
		}
	}
	
	/************************************* Temperature *************************************/
	if(Temperature & 0x8000)
	{
		if((Temperature & 0x7FFF) > BKP_ReadBackupRegister(HigWarnTemperature))
		{
			GroBatWarn = GroBatWarn | (1<<4);//������߸澯�����¶�HigWarnTemperatureʱ���������ؾ���Ĵ���GroBatWarn��bit4 set 1
		}
		else
		{
			GroBatWarn = GroBatWarn & (~(1<<4));
		}
	}
	else
	{
		if((Temperature & 0x7FFF) > (BKP_ReadBackupRegister(LowWarnTemperature) & 0x7FFF))
		{
			GroBatWarn = GroBatWarn | (1<<5);//������͸澯�����¶�LowWarnTemperatureʱ���������ؾ���Ĵ���GroBatWarn��bit5 set 1
		}
		else
		{
			GroBatWarn = GroBatWarn & (~(1<<5));
		}
	}
}


/////////////////////////////////////////////////////////////////
void  BSP_Vol_Res_Bala(void)
{
	BSP_CAN_Detec();							//	CANͨ��״̬���
	
	BSP_getVol();
	BSP_Voltage_Proc();	// ��ѹ���ݴ������ֵ����Сֵ��ƽ��ֵ�����ֵ����ѹ�澯λ
	
	BSP_getRes();
	BSP_Res_Proc();
	
	BSP_sendBala();
		
	BSP_getI_T();	            //  �������¶ȼ��
	BSP_I_T_Proc();
}
void BSP_Key_Proc()
{	
	uint8_t modbus_add_temp;
	
	/* ----------------------------- KEY1 ----------------------------- */
		if (Key_Scan(BSP_KEY_GPIO1, BSP_KEY_PIN1) == KEY_ON)
		{
			BSP_LCD_ON;	// LCD screen ON.
			
			if (BSP_LCD_ON_TIME != 0)
			{
				if(BSP_LCD_DISP_DATA_TYPE_Flag == 6) // ����ģʽ�£�����2��Ϊ �˳�����ģʽ���� ʹ��
				{
					BSP_LCD_DISP_DATA_TYPE_Flag = 6; // ����ʾ�����лص���һҳ
				}
				else
				{
					if (BSP_LCD_DISP_DATA_TYPE_Flag == 4)
					{
						BSP_LCD_DISP_DATA_TYPE_Flag = 1;	// Reset display data type.
					}
					else
					{
						BSP_LCD_DISP_DATA_TYPE_Flag++;	 // Change display data type.
					}
				}
			}
			
			BSP_LCD_ON_TIME = 0;                  // Reset LCD screen time.
			TIM_Cmd(TIM2, ENABLE);
		}
		
		/* ----------------------------- KEY1 ----------------------------- */
		/* ----------------------- Config parameter ----------------------- */
		if (Key_Scan(BSP_KEY_GPIO1, BSP_KEY_PIN1) == KEY_HOLD)
		{
			BSP_LCD_ON;	// LCD screen ON.
			
			if (BSP_LCD_ON_TIME != 0)
			{
				BSP_LCD_DISP_DATA_TYPE_Flag = 6; // ������Ϊ5��������ҳ���л�ʱ���ܻ������ý���
				OSTimeDlyHMSM(0, 0, 0, 300);
				BSP_LCD_Disp_Str_X_Y(0, 1, "                                        ");
	      BSP_LCD_Disp_Str_X_Y(0, 2, "                                        ");
			}
			
			BSP_LCD_ON_TIME = 0; 							// Reset LCD screen time.
			TIM_Cmd(TIM2, ENABLE);						//�л���ʹ�ܶ�ʱ��TIM2,����BSP_LCD_ON_TIME��ֵ
		}
		
		/* ----------------------------- KEY2 ----------------------------- */
		if (Key_Scan(BSP_KEY_GPIO2, BSP_KEY_PIN2) == KEY_ON)
		{
			BSP_LCD_ON;	// LCD screen ON.
			
			if (BSP_LCD_ON_TIME != 0)
			{
				if (BSP_LCD_PAGE_Disp_flag == 3)
				{
					BSP_LCD_PAGE_Disp_flag = 1; // ҳ�泬����Χ��ع�
				}
				else
				{
					if(BSP_LCD_DISP_DATA_TYPE_Flag == 6) // ����ģʽ�£�����2��Ϊ �˳�����ģʽ���� ʹ��
					{
						BSP_LCD_DISP_DATA_TYPE_Flag = 4; // ����ʾ�����лص���4ҳ
					}
					else
					{
						BSP_LCD_PAGE_Disp_flag++; // �������������ģʽ�£�����2������Ϊ ҳ���л����� ʹ��
					}
				}
			}
			
			BSP_LCD_ON_TIME = 0; // Reset LCD screen time.
			TIM_Cmd(TIM2, ENABLE);
		}
		
		/* ----------------------------- KEY3 ----------------------------- */
		if (Key_Scan(BSP_KEY_GPIO3, BSP_KEY_PIN3) == KEY_ON)
		{
			BSP_LCD_ON;	// LCD screen ON.
			
			if (BSP_LCD_ON_TIME != 0)
			{
				if(BSP_LCD_DISP_DATA_TYPE_Flag == 6) // ����ģʽ�£�����3��Modbus��ַ
				{
					if(BKP_ReadBackupRegister(SlaveAddress) == 1)
					{
						modbus_add_temp = 1;
					}
					else
					{
						modbus_add_temp = BKP_ReadBackupRegister(SlaveAddress) - 1;
					}
					BKP_WriteBackupRegister(SlaveAddress, modbus_add_temp);		//���±��ݼĴ���������ݣ���������£����ᶪʧ
				}
				else
				{
					BAT_Vol_Res_Dect_Flag = 1;			//���򣬴���������Ա�־����
				}
			}
			
			BSP_LCD_ON_TIME = 0;
			TIM_Cmd(TIM2, ENABLE);
		}
		
		/* ----------------------------- KEY4 ----------------------------- */
		if (Key_Scan(BSP_KEY_GPIO4, BSP_KEY_PIN4) == KEY_ON)
		{
			BSP_LCD_ON;	// LCD screen ON.
			
			if (BSP_LCD_ON_TIME != 0)
			{
				if(BSP_LCD_DISP_DATA_TYPE_Flag == 6) // ����ģʽ�£�����3��Modbus��ַ
				{
					if(BKP_ReadBackupRegister(SlaveAddress) == 12)			//modbus��ַ���Ϊ12
					{
						modbus_add_temp = 12;
					}
					else
					{
						modbus_add_temp = BKP_ReadBackupRegister(SlaveAddress) + 1;
					}
					BKP_WriteBackupRegister(SlaveAddress, modbus_add_temp);
				}
				else
				{
					if(BSP_BAT_Balan_Flag == 1)
				  {
					  BSP_BAT_Balan_Flag = 0;
					  LED2_OFF;
				  }
				  else
				  {
					  BSP_BAT_Balan_Flag = 1;
				  }
				}
			}
			
			BSP_LCD_ON_TIME = 0;
			TIM_Cmd(TIM2, ENABLE);
		}
	
}



void BSP_LCD_Proc()
{
	BSP_Warn_LED();
		
		switch(BSP_LCD_DISP_DATA_TYPE_Flag)
		{
			case 1:														//��ʾ��ص�ѹ������
				BSP_LCD_Disp_Vol(BSP_LCD_PAGE_Disp_flag,//�ڼ�ҳ���ݣ���Ϊ�βα����ɿ������²�������ֵ
												 GE_BAT_Vol[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 0],//����ڶ�ҳ������ʾ9-16�ڵ�ص����ݣ���ô8*��2-1��+0��GE_BAT_Vol[8]�ھŽڵ�صĵ�ѹֵ
												 GE_BAT_Vol[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 1],
												 GE_BAT_Vol[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 2],
												 GE_BAT_Vol[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 3],
												 GE_BAT_Vol[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 4],
												 GE_BAT_Vol[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 5],
												 GE_BAT_Vol[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 6],
												 GE_BAT_Vol[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 7]);
				OSTimeDlyHMSM(0, 0, 0, 100);
				BSP_LCD_Disp_Bad_Vol(); 				// ��ѯ�澯��ر�ţ�����LCD����Ӧ��λ�ã������˸����
				OSTimeDlyHMSM(0, 0, 0, 100);
			break;
			
			case 2:														//��ʾ������������
				BSP_LCD_Disp_Res(BSP_LCD_PAGE_Disp_flag,
												 BatIntRes[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 0],
												 BatIntRes[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 1],
												 BatIntRes[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 2],
												 BatIntRes[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 3],
												 BatIntRes[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 4],
												 BatIntRes[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 5],
												 BatIntRes[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 6],
												 BatIntRes[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 7]);
				OSTimeDlyHMSM(0, 0, 0, 100);
				BSP_LCD_Disp_Bad_Res(); 				// ��ѯ�澯��ر�ţ�����LCD����Ӧ��λ�ã������˸����
				OSTimeDlyHMSM(0, 0, 0, 100);
			break;
			
			case 3:														// ��ʾ��ص�ѹ���������ֵ��ƽ��ֵ���������ֵ����ֵ
				BSP_LCD_Disp_BAT_Info();
			  OSTimeDlyHMSM(0, 0, 0, 200);
			break;
			
			case 4:														//��Ĭ�ϳ�ʼֵ����ʾ�����Ļ����¶ȣ������������Լ��Ͽ��ӻ�ҳ��
				BSP_LCD_Disp_Current_Tempe();
				BSP_LCD_Disp_CAN_Stat(BSP_BAT_CAN_BAD_Num);
			  OSTimeDlyHMSM(0, 0, 0, 200);
			break;
			
			case 6:														//����modbus address��ģʽ
				BSP_LCD_Disp_Config_Parameter();
			  OSTimeDlyHMSM(0, 0, 0, 200);
			break;
			
			default:
			break;
		}
	
}
