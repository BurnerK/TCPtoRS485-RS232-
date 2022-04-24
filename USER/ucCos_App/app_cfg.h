/****************************************Copyright (c)****************************************************

**--------------File Info---------------------------------------------------------------------------------
** File name:               app_cfg.h
** Descriptions:            ucosii configuration
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-9
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__
					  

/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/
#define  OS_VIEW_MODULE                  DEF_DISABLED	     	/* DEF_ENABLED = Present, DEF_DISABLED = Not Present        */


/*
*********************************************************************************************************
*                                              TASKS NAMES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/	
#define  APP_TASK_START_PRIO             (                 1) 

#define  LWIP_TASK_START_PRIO            (                 2)
#define  LWIP_TASK_END_PRIO              (                 4)

#define  APP_TASK_BLINK_PRIO          	 (                 5) 

#define  APP_TASK_LWIP_PRIO              (                 6)
#define  APP_TASK_RS232_TO_TCP_PRIO      (                 7)
#define  APP_TASK_RS485_TO_TCP_PRIO      (                 8)
#define  APP_TASK_TCP_SERVER_PRIO        (                 9)
#define  APP_TASK_TCP_RS232_SERVER_PRIO  (                 10)

#define  APP_TASK_OSVIEW_TERMINAL_PRIO	 (OS_LOWEST_PRIO - 6)

#define  OS_VIEW_TASK_PRIO               (OS_LOWEST_PRIO - 3)
#define  OS_TASK_TMR_PRIO                (OS_LOWEST_PRIO - 2)



/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/
#define  APP_TASK_LWIP_STK_SIZE                          256u
#define  APP_TASK_RS232_TO_TCP_STK_SIZE                  128U
#define  APP_TASK_RS485_TO_TCP_STK_SIZE                  128U
#define  APP_TASK_TCP_SERVER_STK_SIZE                    128U
#define  APP_TASK_TCP_RS232_SERVER_STK_SIZE              128U
#define  APP_TASK_START_STK_SIZE                         128u
#define  APP_TASK_BLINK_STK_SIZE                         128u
#define  APP_TASK_OSVIEW_TERMINAL_STK_SIZE   			 128u
#define  OS_VIEW_TASK_STK_SIZE                           128u




/*
*********************************************************************************************************
*                                                  LIB
*********************************************************************************************************
*/

#define  uC_CFG_OPTIMIZE_ASM_EN                 DEF_ENABLED
#define  LIB_STR_CFG_FP_EN                      DEF_DISABLED



#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

