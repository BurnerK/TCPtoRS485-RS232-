/***********************************************************************
文件名称：uctsk_TCP_client.c
功    能：创建TCP客户端任务
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
#include <includes.h>

#include "TCP_to_RS232_485.h"
#include "RS232_485_to_TCP.h"
#include "stm32_eth.h"
#include "lwip/tcp.h"

static void RS232_to_TCP_task(void);
static void RS485_to_TCP_task(void);

static  OS_STK  AppRS232_to_TCPTaskStk[APP_TASK_RS232_TO_TCP_STK_SIZE];
static  OS_STK  AppRS485_to_TCPTaskStk[APP_TASK_RS485_TO_TCP_STK_SIZE];

OS_EVENT  *sem_RS232_rec_flag;			//RS232接收完一桢数据信号量定义
OS_EVENT  *sem_RS485_rec_flag;			//RS485接收完一桢数据信号量定义

/***********************************************************************
函数名称：App_TCP_client_taskCreate (void)
功    能：TCP客户端任务的创建
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
void  App_TCP_client_taskCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	/****************************RS232_to_TCP_task任务创建*********************************/
	os_err = OSTaskCreate((void (*)(void *)) RS232_to_TCP_task,				
                          (void          * ) 0,							
                          (OS_STK        * )&AppRS232_to_TCPTaskStk[APP_TASK_RS232_TO_TCP_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_RS232_TO_TCP_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_RS232_TO_TCP_PRIO, "Task RS232_TO_TCP", &os_err);
	#endif

		/****************************RS485_to_TCP_task任务创建*********************************/
	os_err = OSTaskCreate((void (*)(void *)) RS485_to_TCP_task,				
                          (void          * ) 0,							
                          (OS_STK        * )&AppRS485_to_TCPTaskStk[APP_TASK_RS485_TO_TCP_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_RS485_TO_TCP_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_RS485_TO_TCP_PRIO, "Task RS485_TO_TCP", &os_err);
	#endif


}

/***********************************************************************
函数名称：RS232_to_TCP_task(void)
功    能：实现RS232到TCP数据的转换
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
static void RS232_to_TCP_task(void)
{
	CPU_INT08U  os_err;
	struct tcp_pcb *cpcb;
    sem_RS232_rec_flag = OSSemCreate(1); //创建一个信号量,
	while(1)
	{    
		OSSemPend(sem_RS232_rec_flag,0,&os_err);	//持续等待sem_RS232_rec_flag信号量有效
	
		RS232_TCP_send_counter = RS232_rec_counter;//要发送到TCP上的数据长度即为RS232接收到的数据长度。
		RS232_rec_counter = 0;		//RS232接收计数器清零，准备接收下一帧数据。
		/* 向已经建立连接的所有tcp客户端发送RS232数据 */
		for(cpcb = tcp_active_pcbs;cpcb != NULL; cpcb = cpcb->next) 
		{
			tcp_write(cpcb,RS232_rec_buf,RS232_TCP_send_counter,TCP_WRITE_FLAG_COPY);
			tcp_output(cpcb);
		}
	}
}

/***********************************************************************
函数名称：RS485_to_TCP_task(void)
功    能：实现RS485到TCP数据的转换
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
static void RS485_to_TCP_task(void)
{
	CPU_INT08U  os_err;
	struct tcp_pcb *cpcb;
    sem_RS485_rec_flag = OSSemCreate(1); //创建一个信号量,
	RS_485_RX_EN;	//485默认接收
	while(1)
	{    
		OSSemPend(sem_RS485_rec_flag,0,&os_err);	//持续等待sem_RS485_rec_flag信号量有效
		RS485_TCP_send_counter = RS485_rec_counter;//要发送到TCP上的数据长度即为RS485接收到的数据长度。
		RS485_rec_counter = 0;		//RS485接收计数器清零，准备接收下一帧数据。
		/* 向已经建立连接的所有tcp客户端发送RS485数据 */
		for(cpcb = tcp_active_pcbs;cpcb != NULL; cpcb = cpcb->next) 
		{
			tcp_write(cpcb,RS485_rec_buf,RS485_TCP_send_counter,TCP_WRITE_FLAG_COPY);
			tcp_output(cpcb);
		}
	}
}