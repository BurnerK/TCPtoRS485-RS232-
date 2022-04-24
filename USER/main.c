/*
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

/* Includes ------------------------------------------------------------------*/
#include <includes.h>
#include "TCP_to_RS232_485.h"
#include "RS232_485_to_TCP.h"
/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskStartStk[APP_TASK_START_STK_SIZE];



/* Private function prototypes -----------------------------------------------*/
#if (OS_VIEW_MODULE == DEF_ENABLED)
extern void  App_OSViewTaskCreate (void);
#endif

static  void  App_TaskCreate		(void);
static  void  App_TaskStart			(void		*p_arg);  
extern  void  App_BlinkTaskCreate   (void);
extern  void  App_LWIPTaskCreate    (void);


/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/
INT32S main (void)
{
    CPU_INT08U  os_err;
	os_err = os_err; /* prevent warning... */

	/* Note:  ����ʹ��UCOS, ��OS����֮ǰ����,ע���ʹ���κ��ж�. */
	CPU_IntDis();                    /* Disable all ints until we are ready to accept them.  */

    OSInit();                        /* Initialize "uC/OS-II, The Real-Time Kernel".         */
	

	os_err = OSTaskCreateExt((void (*)(void *)) App_TaskStart,  /* Create the start task.                               */
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_START_PRIO,
                             (INT16U          ) APP_TASK_START_PRIO,
                             (OS_STK        * )&App_TaskStartStk[0],
                             (INT32U          ) APP_TASK_START_STK_SIZE,
                             (void          * )0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
	
#if OS_TASK_NAME_EN > 0
    OSTaskNameSet(APP_TASK_START_PRIO, (CPU_INT08U *)"Start Task", &os_err);
#endif

	OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II).  */

	return (0);
}


/*
*********************************************************************************************************
*                                          App_TaskStart()
*
* Description : The startup task.  The uC/OS-II ticker should only be initialize once multitasking starts.
*
* Argument(s) : p_arg       Argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/	  
static  void  App_TaskStart (void *p_arg)
{   
	(void)p_arg;
	
	/***************Ӳ����ʼ��***************/
	Ethernet_Initialize();
	LED_GPIO_Configuration();
    Init_lwIP();
	USART_Configuration();
	TIM_Configuration(RS232_TIMER_OUT);
    OS_CPU_SysTickInit();                                    /* Initialize the SysTick.                              */

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                            /* Determine CPU capacity.                              */
#endif

    App_TaskCreate();                                        /* Create application tasks.                            */

	for(;;)
   	{
      	OSTimeDlyHMSM(0, 1, 0, 0);							 /* Delay One minute */
    }	
}


/*
*********************************************************************************************************
*                                            App_TaskCreate()
*
* Description : Create the application tasks.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : App_TaskStart().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_TaskCreate (void)
{
#if (OS_VIEW_MODULE == DEF_ENABLED)
	App_OSViewTaskCreate();
#endif	
	
	//App_BlinkTaskCreate();		//��ֹ��������
	App_TCP_server_taskCreate();	//����TCP����������,�����������ͻ����������һ��	
	App_TCP_client_taskCreate();	//����TCP�ͻ�������
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/