



#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lwip_comm.h"
#include "LAN8720.h"
#include "timer.h"
#include "sram.h"
#include "malloc.h"
#include "lwip_comm.h"
#include "lwipopts.h"
#include "httpd.h"


#include "includes.h"

extern  u8 LAN8720_Get_link(void);
/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  OS_STK App_Task_Start_Stk[APP_TASK_START_STK_SIZE];

static  OS_STK App_TaskCAN_Stk[APP_TASK_CAN_STK_SIZE];				//CAN���ݴ�������
static  OS_STK App_TaskKEY_Stk[APP_TASK_KEY_STK_SIZE];				//��������
static  OS_STK App_TaskLCD_Stk[APP_TASK_LCD_STK_SIZE];				//��ʾ������
static  OS_STK App_TaskMB_Stk[APP_TASK_MB_STK_SIZE];					//modbus����
static  OS_STK App_TaskNET_Stk[APP_TASK_NET_STK_SIZE];					//ethernet����


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  App_TaskStart(void *p_arg);

static  void  App_TaskCreate(void);
static	void  App_EventCreate(void);

static  void  App_TaskCAN(void *p_arg);
static  void  App_TaskKEY(void *p_arg);
static  void  App_TaskLCD(void *p_arg);
static  void  App_TaskMB (void *p_arg);
static  void  App_TaskNET (void *p_arg);

int main(void)
{
	delay_init(168);       	//��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�жϷ�������

	uart_init(115200);   	//���ڲ���������
	
	FSMC_SRAM_Init(); 		//SRAM��ʼ��
	
	mymem_init(SRAMIN);  	//��ʼ���ڲ��ڴ��
	mymem_init(SRAMEX);  	//��ʼ���ⲿ�ڴ��
	mymem_init(SRAMCCM); 	//��ʼ��CCM�ڴ��

	OSInit(); 					//UCOS��ʼ��											//http��ʼ��
	OSTaskCreateExt(App_TaskStart,                                               /* ָ����������ָ��                                                    */
                  (void *)0,                                                   /* ����ʼִ��ʱ�����ݸ�����Ĳ�����ָ��                                */
                  (OS_STK *)&App_Task_Start_Stk[APP_TASK_START_STK_SIZE - 1],  /* ���������Ķ�ջ��ջ��ָ�룬�Ӷ����µݼ�                              */
                  APP_TASK_START_PRIO,                                         /* �������������ȼ�                                                    */
                  APP_TASK_START_PRIO,                                         /* Ԥ�����Ժ�汾�������ʶ���������а汾ͬ�������ȼ�                    */
                  (OS_STK *)&App_Task_Start_Stk[0],                            /* ָ�������ջջ�׵�ָ�룬���ڶ�ջ�ļ���                                */
                  APP_TASK_START_STK_SIZE,                                     /* ָ����ջ�����������ڶ�ջ�ļ���                                        */
                  (void *)0,                                                   /* ָ���û����ӵ��������ָ�룬������չ�����������ƿ�                  */
                  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);                  /* ѡ�ָ���Ƿ������ջ���飬�Ƿ񽫶�ջ��0,�����Ƿ�Ҫ���и�������ȵ�  */

	OSStart(); //����UCOS
}

/*
*********************************************************************************************************
*                                          App_TaskStart()
*
* Description : The startup task.  The uC/OS-II ticker should only be initialize once multitasking starts.
*
* Argument : p_arg       Argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return   : none.
*
* Caller   : This is a task.
*
* Note     : none.
*********************************************************************************************************
*/

static  void  App_TaskStart(void* p_arg)
{
	OS_CPU_SR cpu_sr;
	
	#if (OS_TASK_STAT_EN > 0)                  /* Enable the statistics task                               */
		OSStatInit();                              /* Determine CPU capacity                                   */                    
	#endif
  
	App_EventCreate();                         /* Create Application Kernel objects                        */
	App_TaskCreate();                          /* Task body, always written as an infinite loop.           */
	BSP_Init();
	
	if(LAN8720_Init()){;}			//��ʼ��LAN8720ʧ��
		
			lwip_comm_init();	//lwip��ʼ��
				httpd_init();  	
		
				OSStatInit();  			//��ʼ��ͳ������
				OS_ENTER_CRITICAL();  	//���ж�
				#if	LWIP_DHCP
					lwip_comm_dhcp_creat();	//����DHCP����
				#endif
				OS_EXIT_CRITICAL();  //���ж�
		
	while (1)
	{
		LED1_TOGGLE;	
//		//LAN8720_Get_link() 	����ֵ1Ϊ�Ͽ�
//		if (LAN8720_Get_link()){
//			#define LINK_STATUS 1
//		}else{
//			#define LINK_STATUS 0
//		}
//		#if LINK_STATUS 
//		
//		/////////////////
//	  //�����������ӵ�ʱ����������ᷢ��һ���ź��������������ܹ��õ�ִ��
//		
//				lwip_comm_init();	//lwip��ʼ��
//				httpd_init();  	
//		
//				OSStatInit();  			//��ʼ��ͳ������
//				OS_ENTER_CRITICAL();  	//���ж�
//				#if	LWIP_DHCP
//					lwip_comm_dhcp_creat();	//����DHCP����
//				#endif
//				OS_EXIT_CRITICAL();  //���ж�
//	  #endif
		OSTimeDlyHMSM(0, 0, 1, 0);
  }
}



/*
*********************************************************************************************************
*                                          App_TaskCAN()
*
* Description : 
*
* Argument : p_arg       Argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return   : none.
*
* Caller   : This is a task.
*
* Note     : none.
*********************************************************************************************************
*/

static  void  App_TaskCAN(void* p_arg)
{
	while (1)
	{
		BSP_Vol_Res_Bala();
		OSTimeDlyHMSM(0, 0, 0, 800);
  }
}


/*
*********************************************************************************************************
*                                          App_TaskMB()
*
* Description : 
*
* Argument : p_arg       Argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return   : none.
*
* Caller   : This is a task.
*
* Note     : none.
*********************************************************************************************************
*/

static  void  App_TaskMB(void* p_arg)
{
	while (1)
	{
		GE_MB_Protocol();
		OSTimeDlyHMSM(0, 0, 0, 500);
  }
}


/*
*********************************************************************************************************
*                                          App_TaskKEY()
*
* Description : 
*
* Argument : p_arg       Argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return   : none.
*
* Caller   : This is a task.
*
* Note     : none.
*********************************************************************************************************
*/

static  void  App_TaskKEY(void* p_arg)
{
	while (1u)
	{
		BSP_Key_Proc();
		OSTimeDlyHMSM(0, 0, 0, 40);		
  }
}


/*
*********************************************************************************************************
*                                          App_TaskLCD()
*
* Description : 
*
* Argument : p_arg       Argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return   : none.
*
* Caller   : This is a task.
*
* Note     : none.
*********************************************************************************************************
*/

static  void  App_TaskLCD(void* p_arg)
{
	while (1u)
	{
		BSP_LCD_Proc();
		OSTimeDlyHMSM(0, 0, 0, 500);
	}
}


/*
*********************************************************************************************************
*                                          App_TaskLCD()
*
* Description : 
*
* Argument : p_arg       Argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return   : none.
*
* Caller   : This is a task.
*
* Note     : none.
*********************************************************************************************************
*/

static  void  App_TaskNET(void* p_arg)
{
		OS_CPU_SR cpu_sr;
	while (1u)
	{
		LED4_TOGGLE;	
		OSTimeDlyHMSM(0, 0, 0, 800);
//	lwip_comm_init();	//lwip��ʼ��
//	httpd_init();  	
//	
/////////////////////////////////
//	OSStatInit();  			//��ʼ��ͳ������
//	OS_ENTER_CRITICAL();  	//���ж�
//#if	LWIP_DHCP
//	lwip_comm_dhcp_creat();	//����DHCP����
//#endif
//	//OSTaskSuspend(OS_PRIO_SELF); //����start_task����
//	OS_EXIT_CRITICAL();  //���ж�
///////////////////////////////
		OSTimeDlyHMSM(0, 0, 0, 800);
	}
}


/*
*********************************************************************************************************
*                                            App_TaskCreate()
*
* Description : Create the application tasks.
*
* Argument : none.
*
* Return   : none.
*
* Caller   : App_TaskStart().
*
* Note     : none.
*********************************************************************************************************
*/

static  void  App_TaskCreate(void)
{
	OSTaskCreateExt(App_TaskCAN,
	                (void *)0,
	                (OS_STK *)&App_TaskCAN_Stk[APP_TASK_CAN_STK_SIZE - 1],
	                APP_Task_CAN_PRIO,
	                APP_Task_CAN_PRIO,
	                (OS_STK *)&App_TaskCAN_Stk[0],
                  APP_TASK_CAN_STK_SIZE,
                  (void *)0,
                  OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
	
	OSTaskCreateExt(App_TaskKEY,
	                (void *)0,
	                (OS_STK *)&App_TaskKEY_Stk[APP_TASK_KEY_STK_SIZE - 1],
	                APP_Task_KEY_PRIO,
	                APP_Task_KEY_PRIO,
	                (OS_STK *)&App_TaskKEY_Stk[0],
                  APP_TASK_KEY_STK_SIZE,
                  (void *)0,
                  OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
									
	OSTaskCreateExt(App_TaskLCD,
	                (void *)0,
	                (OS_STK *)&App_TaskLCD_Stk[APP_TASK_LCD_STK_SIZE - 1],
	                APP_Task_LCD_PRIO,
	                APP_Task_LCD_PRIO,
	                (OS_STK *)&App_TaskLCD_Stk[0],
                  APP_TASK_LCD_STK_SIZE,
                  (void *)0,
                  OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
									
	OSTaskCreateExt(App_TaskMB,
	                (void *)0,
	                (OS_STK *)&App_TaskMB_Stk[APP_TASK_MB_STK_SIZE - 1],
	                APP_Task_MB_PRIO,
	                APP_Task_MB_PRIO,
	                (OS_STK *)&App_TaskMB_Stk[0],
                  APP_TASK_MB_STK_SIZE,
                  (void *)0,
                  OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
									
	OSTaskCreateExt(App_TaskNET,
	                (void *)0,
	                (OS_STK *)&App_TaskNET_Stk[APP_TASK_NET_STK_SIZE - 1],
	                APP_Task_NET_PRIO,
	                APP_Task_NET_PRIO,
	                (OS_STK *)&App_TaskNET_Stk[0],
                  APP_TASK_NET_STK_SIZE,
                  (void *)0,
                  OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
}



/*
*********************************************************************************************************
*                                          AppEventCreate()
*
* Description : Create application kernel objects tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_EventCreate(void)
{
	
}



