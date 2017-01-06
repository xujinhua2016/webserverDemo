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
	m = num / 6;//对参数除以6，取商
	n = num % 6;//对参数除以6，取余
	
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
	m = num / 6;//对参数除以6，取商
	n = num % 6;//对参数除以6，取余
	
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
	m = num / 6;//对参数除以6，取商
	n = num % 6;//对参数除以6，取余
	
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
	*首先判断电池是否含有均衡指令
	*其次要判断电池的工作状态是否处于正常值
	*/
	uint8_t i;
	
	if (BAT_Vol_Res_Dect_Flag)		//查询是否需要做内阻检测的标志
	{
		if(BSP_BAT_Balan_Flag != 1)	//电池没有处于均衡的情况下
		{
			//查询电池电压是否处于正常的工作范围内
			//发送内阻均衡指令,这里采用全部测试或全部不测试方略
			if(((BatVolWarnBit000_015 | BatVolWarnBit016_031) == 0))//电池工作状态正常
			{
				for(i = 0; i < 24; i++)
				{
					BSP_getBatR(i);
				}
			}
			else			//电池工作状态不正常
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
	*首先判断电池是否发布内阻测试指令
	*判断电池的工作状态是否处于正常值
	*根据处理函数判断，均衡最低电压电池（低于一定的阈值）
	*均衡指令开启后，开启led2  LED2_ON
	*/
	if(BSP_BAT_Balan_Flag)
	{
		if(BAT_Vol_Res_Dect_Flag != 1)
		{		
			//判断电压的工作范围是否正常		//(BatVolWarnBit000_015 | BatVolWarnBit016_031) == 0)  XX 若有高电压，也应该开启给其他电池开启均衡开关
			
			//找到最低电池电压和编号
			//查询最低电压与平均电压之间的差值与最低电压是否大于最低告警值
			//发布单节电池均衡指令，开启led2		
			
				for(i = 0; i < 24; i++)
				{
					tempBatVol[i] = GE_BAT_Vol[i];
				}
			
				j = 0;		//用于保存最低电压电池的编号
				
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
			
			//最低电池电压大于最低电压告警值并且平均电压与最低电压之间的差值大于设定的阈值
			if((tempBatVol[j] > BKP_ReadBackupRegister(LowWarnVolSinBat)) && (AverageVolVal - tempBatVol[j]) > D_VALUE)		
			{
				//发送均衡指令
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
	
	//首先将数据转换为模拟电压值
	for(i = 0; i < 24; i++)
	{
		if(i<6)
		{
			GE_BAT_Vol[i] = NoConvBatVol[i] * SLAVE_VREF1 /4095;		//转换成电压模拟数值
		}
		else if(i>=6 && i<12)
		{
			GE_BAT_Vol[i] = NoConvBatVol[i] * SLAVE_VREF2 /4095;		//转换成电压模拟数值
		}
		else if(i>=12 && i<18)
		{
			GE_BAT_Vol[i] = NoConvBatVol[i] * SLAVE_VREF3 /4095;		//转换成电压模拟数值
		}
		else if(i>=18 && i<24)
		{
			GE_BAT_Vol[i] = NoConvBatVol[i] * SLAVE_VREF4 /4095;		//转换成电压模拟数值
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
	TotalBatVol = Ave_Vol;					//更新电池电压总和
	AverageVolVal = Ave_Vol / 24;		//更新电池平均电压
	
	if(AverageVolVal > BKP_ReadBackupRegister(HigWarnVolSinBat))
	{
		GroBatWarn = GroBatWarn | (1<<0);			//电池组平均电压大于最高告警单电池电压HigWarnVolSinBat时，GroBatWarn寄存器bit 0 set 1
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
	//采用冒泡方法对24节电池的数据进行排序处理，并且按顺序保存电池的编号
	for (i=0;i<24;i++)
	{
		iMax = i;														//用于保存电池的编号
		vMax = BSP_BAT_Vol_Min_To_Max[i];		//用于保存电池电压值
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
	//将最低三节电池和最高三节电池的数据和电池编号进行赋值处理
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
		GroBatWarn = GroBatWarn | (1<<1);							//高于最大告警电压差MaxWarnVolDiff时，GroBatWarn寄存器bit 1 set 1
	}
	else
	{
		GroBatWarn = GroBatWarn & (~(1<<1));
	}
	
	/******** Warning voltage number *********/
	/********  单体电压警告，使用低16位，电池出现电压警告，相应数据位置1  **********/
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
	
	//首先将电压的差值映射到内阻值量
	for(i = 0; i < 24; i++)
	{
		BatIntRes[i] = NoConvBatRes[i]  * REF_RES * 10 / REF_DAC1V;		//转换成内阻的模拟数值
		BSP_BAT_Res_Min_To_Max[i] = BatIntRes[i];
		BSP_BAT_Res_Min_To_Max_Num[i] = i;
	}
	
	/************** Max and Min resistor **************/
	
	//采用冒泡方法对24节电池的数据进行排序处理
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
		GroBatWarn = GroBatWarn | (1<<6);							//高于最高告警内阻MaxWarnRes时，GroBatWarn寄存器bit 6 set 1
	}
	else
	{
		GroBatWarn = GroBatWarn & (~(1<<6));
	}

	/**** Warning internal resistor number ***/
/*******  单体电池内阻警告，使用低16位，电池出现电压警告，相应数据位置1  *******/
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
			GroBatWarn = GroBatWarn | (1<<2);//大于最大告警充电电流MaxWarnChargeCurVal时，将整组电池警告寄存器GroBatWarn的bit2 set 1
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
			GroBatWarn = GroBatWarn | (1<<3);//大于最大告警放电电流MaxWarnDischargeCurVal时，将整组电池警告寄存器GroBatWarn的bit3 set 1
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
			GroBatWarn = GroBatWarn | (1<<4);//大于最高告警环境温度HigWarnTemperature时，将整组电池警告寄存器GroBatWarn的bit4 set 1
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
			GroBatWarn = GroBatWarn | (1<<5);//大于最低告警环境温度LowWarnTemperature时，将整组电池警告寄存器GroBatWarn的bit5 set 1
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
	BSP_CAN_Detec();							//	CAN通信状态检测
	
	BSP_getVol();
	BSP_Voltage_Proc();	// 电压数据处理：最大值，最小值，平均值，峰峰值，电压告警位
	
	BSP_getRes();
	BSP_Res_Proc();
	
	BSP_sendBala();
		
	BSP_getI_T();	            //  电流、温度检测
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
				if(BSP_LCD_DISP_DATA_TYPE_Flag == 6) // 配置模式下，按键2作为 退出配置模式按键 使用
				{
					BSP_LCD_DISP_DATA_TYPE_Flag = 6; // 将显示切面切回到第一页
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
				BSP_LCD_DISP_DATA_TYPE_Flag = 6; // 若设置为5，在正常页面切换时可能会跳到该界面
				OSTimeDlyHMSM(0, 0, 0, 300);
				BSP_LCD_Disp_Str_X_Y(0, 1, "                                        ");
	      BSP_LCD_Disp_Str_X_Y(0, 2, "                                        ");
			}
			
			BSP_LCD_ON_TIME = 0; 							// Reset LCD screen time.
			TIM_Cmd(TIM2, ENABLE);						//切换后使能定时器TIM2,更新BSP_LCD_ON_TIME的值
		}
		
		/* ----------------------------- KEY2 ----------------------------- */
		if (Key_Scan(BSP_KEY_GPIO2, BSP_KEY_PIN2) == KEY_ON)
		{
			BSP_LCD_ON;	// LCD screen ON.
			
			if (BSP_LCD_ON_TIME != 0)
			{
				if (BSP_LCD_PAGE_Disp_flag == 3)
				{
					BSP_LCD_PAGE_Disp_flag = 1; // 页面超出范围后回归
				}
				else
				{
					if(BSP_LCD_DISP_DATA_TYPE_Flag == 6) // 配置模式下，按键2作为 退出配置模式按键 使用
					{
						BSP_LCD_DISP_DATA_TYPE_Flag = 4; // 将显示切面切回到第4页
					}
					else
					{
						BSP_LCD_PAGE_Disp_flag++; // 如果不是在配置模式下，按键2继续作为 页面切换按键 使用
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
				if(BSP_LCD_DISP_DATA_TYPE_Flag == 6) // 配置模式下，按键3减Modbus地址
				{
					if(BKP_ReadBackupRegister(SlaveAddress) == 1)
					{
						modbus_add_temp = 1;
					}
					else
					{
						modbus_add_temp = BKP_ReadBackupRegister(SlaveAddress) - 1;
					}
					BKP_WriteBackupRegister(SlaveAddress, modbus_add_temp);		//更新备份寄存器里的数据，掉电情况下，不会丢失
				}
				else
				{
					BAT_Vol_Res_Dect_Flag = 1;			//否则，触发内阻测试标志量。
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
				if(BSP_LCD_DISP_DATA_TYPE_Flag == 6) // 配置模式下，按键3加Modbus地址
				{
					if(BKP_ReadBackupRegister(SlaveAddress) == 12)			//modbus地址最高为12
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
			case 1:														//显示电池电压的数据
				BSP_LCD_Disp_Vol(BSP_LCD_PAGE_Disp_flag,//第几页数据，作为形参变量可控制余下参数的数值
												 GE_BAT_Vol[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 0],//假设第二页，则显示9-16节电池的数据，那么8*（2-1）+0是GE_BAT_Vol[8]第九节电池的电压值
												 GE_BAT_Vol[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 1],
												 GE_BAT_Vol[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 2],
												 GE_BAT_Vol[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 3],
												 GE_BAT_Vol[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 4],
												 GE_BAT_Vol[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 5],
												 GE_BAT_Vol[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 6],
												 GE_BAT_Vol[8 * (BSP_LCD_PAGE_Disp_flag - 1) + 7]);
				OSTimeDlyHMSM(0, 0, 0, 100);
				BSP_LCD_Disp_Bad_Vol(); 				// 查询告警电池编号，擦出LCD上相应的位置，造成闪烁现象。
				OSTimeDlyHMSM(0, 0, 0, 100);
			break;
			
			case 2:														//显示电池内阻的数据
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
				BSP_LCD_Disp_Bad_Res(); 				// 查询告警电池编号，擦出LCD上相应的位置，造成闪烁现象。
				OSTimeDlyHMSM(0, 0, 0, 100);
			break;
			
			case 3:														// 显示电池电压的三个最高值，平均值，三个最低值，峰值
				BSP_LCD_Disp_BAT_Info();
			  OSTimeDlyHMSM(0, 0, 0, 200);
			break;
			
			case 4:														//（默认初始值）显示电池组的环境温度，环境电流，以及断开从机页面
				BSP_LCD_Disp_Current_Tempe();
				BSP_LCD_Disp_CAN_Stat(BSP_BAT_CAN_BAD_Num);
			  OSTimeDlyHMSM(0, 0, 0, 200);
			break;
			
			case 6:														//配置modbus address的模式
				BSP_LCD_Disp_Config_Parameter();
			  OSTimeDlyHMSM(0, 0, 0, 200);
			break;
			
			default:
			break;
		}
	
}
