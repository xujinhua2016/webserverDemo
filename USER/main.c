



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

static  OS_STK App_TaskCAN_Stk[APP_TASK_CAN_STK_SIZE];				//CAN数据传输任务
static  OS_STK App_TaskKEY_Stk[APP_TASK_KEY_STK_SIZE];				//按键任务
static  OS_STK App_TaskLCD_Stk[APP_TASK_LCD_STK_SIZE];				//显示屏任务
static  OS_STK App_TaskMB_Stk[APP_TASK_MB_STK_SIZE];					//modbus任务
static  OS_STK App_TaskNET_Stk[APP_TASK_NET_STK_SIZE];					//ethernet任务


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
	delay_init(168);       	//延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断分组配置

	uart_init(115200);   	//串口波特率设置
	
	FSMC_SRAM_Init(); 		//SRAM初始化
	
	mymem_init(SRAMIN);  	//初始化内部内存池
	mymem_init(SRAMEX);  	//初始化外部内存池
	mymem_init(SRAMCCM); 	//初始化CCM内存池

	OSInit(); 					//UCOS初始化											//http初始化
	OSTaskCreateExt(App_TaskStart,                                               /* 指向任务代码的指针                                                    */
                  (void *)0,                                                   /* 任务开始执行时，传递给任务的参数的指针                                */
                  (OS_STK *)&App_Task_Start_Stk[APP_TASK_START_STK_SIZE - 1],  /* 分配给任务的堆栈的栈顶指针，从顶向下递减                              */
                  APP_TASK_START_PRIO,                                         /* 分配给任务的优先级                                                    */
                  APP_TASK_START_PRIO,                                         /* 预备给以后版本的特殊标识符，在现行版本同任务优先级                    */
                  (OS_STK *)&App_Task_Start_Stk[0],                            /* 指向任务堆栈栈底的指针，用于堆栈的检验                                */
                  APP_TASK_START_STK_SIZE,                                     /* 指定堆栈的容量，用于堆栈的检验                                        */
                  (void *)0,                                                   /* 指向用户附加的数据域的指针，用来扩展任务的任务控制块                  */
                  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);                  /* 选项，指定是否允许堆栈检验，是否将堆栈清0,任务是否要进行浮点运算等等  */

	OSStart(); //开启UCOS
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
	
	if(LAN8720_Init()){;}			//初始化LAN8720失败
		
			lwip_comm_init();	//lwip初始化
				httpd_init();  	
		
				OSStatInit();  			//初始化统计任务
				OS_ENTER_CRITICAL();  	//关中断
				#if	LWIP_DHCP
					lwip_comm_dhcp_creat();	//创建DHCP任务
				#endif
				OS_EXIT_CRITICAL();  //开中断
		
	while (1)
	{
		LED1_TOGGLE;	
//		//LAN8720_Get_link() 	返回值1为断开
//		if (LAN8720_Get_link()){
//			#define LINK_STATUS 1
//		}else{
//			#define LINK_STATUS 0
//		}
//		#if LINK_STATUS 
//		
//		/////////////////
//	  //在有网线连接的时候，其他任务会发出一个信号量，并且任务能够得到执行
//		
//				lwip_comm_init();	//lwip初始化
//				httpd_init();  	
//		
//				OSStatInit();  			//初始化统计任务
//				OS_ENTER_CRITICAL();  	//关中断
//				#if	LWIP_DHCP
//					lwip_comm_dhcp_creat();	//创建DHCP任务
//				#endif
//				OS_EXIT_CRITICAL();  //开中断
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
//	lwip_comm_init();	//lwip初始化
//	httpd_init();  	
//	
/////////////////////////////////
//	OSStatInit();  			//初始化统计任务
//	OS_ENTER_CRITICAL();  	//关中断
//#if	LWIP_DHCP
//	lwip_comm_dhcp_creat();	//创建DHCP任务
//#endif
//	//OSTaskSuspend(OS_PRIO_SELF); //挂起start_task任务
//	OS_EXIT_CRITICAL();  //开中断
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



