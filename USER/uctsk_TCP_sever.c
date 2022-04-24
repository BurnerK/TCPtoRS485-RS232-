												  /***********************************************************************
文件名称：uctsk_TCP_sever.c
功    能：创建TCP服务器任务
编写时间：2013.4.25
编 写 人：赵志峰
注    意：
***********************************************************************/
#include <includes.h>
#include "stm32_eth.h"
#include "lwip/tcp.h"

static void TCP_server_task(void);


static  OS_STK  AppTCP_serverTaskStk[APP_TASK_TCP_SERVER_STK_SIZE];
static  OS_STK  AppTCP_RS232_serverTaskStk[APP_TASK_TCP_RS232_SERVER_STK_SIZE];
/***********************************************************************
函数名称：App_TCP_client_taskCreate (void)
功    能：TCP服务器任务的创建
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
void  App_TCP_server_taskCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) TCP_server_task,				
                          (void          * ) 0,							
                          (OS_STK        * )&AppTCP_serverTaskStk[APP_TASK_TCP_SERVER_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_TCP_SERVER_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_TCP_SERVER_PRIO, "Task TCP_SERVER", &os_err);
	#endif
}
/***********************************************************************
函数名称：TCP_server_task(void)
功    能：TCP服务器任务实现
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
static void TCP_server_task(void)
{
	TCP_server_init();	//tcp服务器初始化
	while(1)
	{    
		OSTimeDlyHMSM(0, 0, 1, 0);//延时，这样别的任务才能够得以运行。
	}
}