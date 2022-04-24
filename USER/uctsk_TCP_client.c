/***********************************************************************
�ļ����ƣ�uctsk_TCP_client.c
��    �ܣ�����TCP�ͻ�������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
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

OS_EVENT  *sem_RS232_rec_flag;			//RS232������һ�������ź�������
OS_EVENT  *sem_RS485_rec_flag;			//RS485������һ�������ź�������

/***********************************************************************
�������ƣ�App_TCP_client_taskCreate (void)
��    �ܣ�TCP�ͻ�������Ĵ���
���������
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
***********************************************************************/
void  App_TCP_client_taskCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	/****************************RS232_to_TCP_task���񴴽�*********************************/
	os_err = OSTaskCreate((void (*)(void *)) RS232_to_TCP_task,				
                          (void          * ) 0,							
                          (OS_STK        * )&AppRS232_to_TCPTaskStk[APP_TASK_RS232_TO_TCP_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_RS232_TO_TCP_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_RS232_TO_TCP_PRIO, "Task RS232_TO_TCP", &os_err);
	#endif

		/****************************RS485_to_TCP_task���񴴽�*********************************/
	os_err = OSTaskCreate((void (*)(void *)) RS485_to_TCP_task,				
                          (void          * ) 0,							
                          (OS_STK        * )&AppRS485_to_TCPTaskStk[APP_TASK_RS485_TO_TCP_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_RS485_TO_TCP_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_RS485_TO_TCP_PRIO, "Task RS485_TO_TCP", &os_err);
	#endif


}

/***********************************************************************
�������ƣ�RS232_to_TCP_task(void)
��    �ܣ�ʵ��RS232��TCP���ݵ�ת��
���������
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
***********************************************************************/
static void RS232_to_TCP_task(void)
{
	CPU_INT08U  os_err;
	struct tcp_pcb *cpcb;
    sem_RS232_rec_flag = OSSemCreate(1); //����һ���ź���,
	while(1)
	{    
		OSSemPend(sem_RS232_rec_flag,0,&os_err);	//�����ȴ�sem_RS232_rec_flag�ź�����Ч
	
		RS232_TCP_send_counter = RS232_rec_counter;//Ҫ���͵�TCP�ϵ����ݳ��ȼ�ΪRS232���յ������ݳ��ȡ�
		RS232_rec_counter = 0;		//RS232���ռ��������㣬׼��������һ֡���ݡ�
		/* ���Ѿ��������ӵ�����tcp�ͻ��˷���RS232���� */
		for(cpcb = tcp_active_pcbs;cpcb != NULL; cpcb = cpcb->next) 
		{
			tcp_write(cpcb,RS232_rec_buf,RS232_TCP_send_counter,TCP_WRITE_FLAG_COPY);
			tcp_output(cpcb);
		}
	}
}

/***********************************************************************
�������ƣ�RS485_to_TCP_task(void)
��    �ܣ�ʵ��RS485��TCP���ݵ�ת��
���������
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
***********************************************************************/
static void RS485_to_TCP_task(void)
{
	CPU_INT08U  os_err;
	struct tcp_pcb *cpcb;
    sem_RS485_rec_flag = OSSemCreate(1); //����һ���ź���,
	RS_485_RX_EN;	//485Ĭ�Ͻ���
	while(1)
	{    
		OSSemPend(sem_RS485_rec_flag,0,&os_err);	//�����ȴ�sem_RS485_rec_flag�ź�����Ч
		RS485_TCP_send_counter = RS485_rec_counter;//Ҫ���͵�TCP�ϵ����ݳ��ȼ�ΪRS485���յ������ݳ��ȡ�
		RS485_rec_counter = 0;		//RS485���ռ��������㣬׼��������һ֡���ݡ�
		/* ���Ѿ��������ӵ�����tcp�ͻ��˷���RS485���� */
		for(cpcb = tcp_active_pcbs;cpcb != NULL; cpcb = cpcb->next) 
		{
			tcp_write(cpcb,RS485_rec_buf,RS485_TCP_send_counter,TCP_WRITE_FLAG_COPY);
			tcp_output(cpcb);
		}
	}
}