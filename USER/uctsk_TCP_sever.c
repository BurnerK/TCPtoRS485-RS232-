												  /***********************************************************************
�ļ����ƣ�uctsk_TCP_sever.c
��    �ܣ�����TCP����������
��дʱ�䣺2013.4.25
�� д �ˣ���־��
ע    �⣺
***********************************************************************/
#include <includes.h>
#include "stm32_eth.h"
#include "lwip/tcp.h"

static void TCP_server_task(void);


static  OS_STK  AppTCP_serverTaskStk[APP_TASK_TCP_SERVER_STK_SIZE];
static  OS_STK  AppTCP_RS232_serverTaskStk[APP_TASK_TCP_RS232_SERVER_STK_SIZE];
/***********************************************************************
�������ƣ�App_TCP_client_taskCreate (void)
��    �ܣ�TCP����������Ĵ���
���������
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
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
�������ƣ�TCP_server_task(void)
��    �ܣ�TCP����������ʵ��
���������
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
***********************************************************************/
static void TCP_server_task(void)
{
	TCP_server_init();	//tcp��������ʼ��
	while(1)
	{    
		OSTimeDlyHMSM(0, 0, 1, 0);//��ʱ���������������ܹ��������С�
	}
}