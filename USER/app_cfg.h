/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2006; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                      APPLICATION CONFIGURATION
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210B-LK1 Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.10
* Programmer(s) : BH3NVN
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__

/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              TASKS NAMES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*
*                           +--------------------------------------------------+
*                           |函数执行异常或无效时，检查是不是任务优先级排序不对|
*                           +--------------------------------------------------+
*
*********************************************************************************************************
*/

#define  APP_TASK_START_PRIO                               2

#define  APP_Task_CAN_PRIO                                 8

#define  APP_Task_MB_PRIO                                  9

#define  APP_Task_KEY_PRIO                                 10

#define  APP_Task_LCD_PRIO                                 11


#define  APP_Task_NET_PRIO                                 12

//#define  APP_Task_LED2_PRIO                                11


/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define  APP_TASK_START_STK_SIZE                         128

#define  APP_TASK_CAN_STK_SIZE                           512

#define  APP_TASK_KEY_STK_SIZE                           128

#define  APP_TASK_LCD_STK_SIZE                           512

#define  APP_TASK_MB_STK_SIZE                            512

#define  APP_TASK_NET_STK_SIZE                           512

//#define  APP_TASK_LED2_STK_SIZE                          512


/*
*********************************************************************************************************
*                                                  LIB
*********************************************************************************************************
*/

#define  uC_CFG_OPTIMIZE_ASM_EN                 DEF_ENABLED
#define  LIB_STR_CFG_FP_EN                      DEF_DISABLED

#endif
