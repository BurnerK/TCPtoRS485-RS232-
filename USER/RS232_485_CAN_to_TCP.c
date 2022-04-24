/***********************************************************************
�ļ����ƣ�RS232_485_CAN_to_TCP.C
��    �ܣ����RS232/485/CAN��TCPЭ���ת��
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
***********************************************************************/
#include "stm32_eth.h"
#include "lwip/tcp.h"
#include "RS232_485_CAN_to_TCP.h"



unsigned char RS232_REC_Flag = 0;
unsigned char RS485_REC_Flag = 0;
unsigned char CAN1_REC_Flag  = 0;
unsigned char CAN2_REC_Flag  = 0;
unsigned char RS232_rec_counter = 0;
unsigned char RS485_rec_counter = 0;
unsigned char CAN1_rec_counter = 0;
unsigned char CAN2_rec_counter = 0;
unsigned char RS232_rec_buf[RS232_REC_BUF]={1};
unsigned char RS485_rec_buf[RS485_REC_BUF];
unsigned char CAN1_rec_buf[CAN1_REC_BUF];
unsigned char CAN2_rec_buf[CAN2_REC_BUF];


void RS232_TCP_Client_Init(u16_t local_port,u16_t remote_port,unsigned char a,unsigned char b,unsigned char c,unsigned char d);
void RS485_TCP_Client_Init(u16_t local_port,u16_t remote_port,unsigned char a,unsigned char b,unsigned char c,unsigned char d);
/***********************************************************************
�������ƣ�RS232_to_TCP(void)
��    �ܣ�RS232��TCP��Э��ת������ں���
���������
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺�ڸú����������ж�RS232�Ƿ������һ�����ݣ�һ�������ԡ�������;'��β�����������꣬
		  �򽫸�֡������RS232_TCP_Client_Init����ָ���Ĳ�����TCP�������ݡ�
		  ��UCOS�¿���ͨ���ź����ķ�ʽ���ж�һ�������Ƿ������ɣ����Ըú����ò����ˡ�
***********************************************************************/
void RS232_to_TCP(void)
{
	if(RS232_REC_Flag == 1)//RS232���յ�һ֡����
	{	
		RS232_TCP_Client_Init(RS232_LOCAL_PORT,RS232_REMOTE_PORT,RS232_SERVER_IP);//��RS232�˿ڣ�ip����������
		RS232_REC_Flag = 0;
	}
}

/***********************************************************************
�������ƣ�RS485_to_TCP(void)
��    �ܣ�RS485��TCP��Э��ת������ں���
���������
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺�ڸú����������ж�RS485�Ƿ������һ�����ݣ�һ�������ԡ�������;'��β�����������꣬
		  �򽫸�֡������RS485_TCP_Client_Init����ָ���Ĳ�����TCP�������ݡ�
		  ��UCOS�¿���ͨ���ź����ķ�ʽ���ж�һ�������Ƿ������ɣ����Ըú����ò����ˡ�
***********************************************************************/
void RS485_to_TCP(void)
{
	if(RS485_REC_Flag == 1)//RS485���յ�һ֡����
	{	
		RS485_TCP_Client_Init(RS485_LOCAL_PORT,RS485_REMOTE_PORT,RS485_SERVER_IP);//��RS485�˿ڣ�ip����������
		RS485_REC_Flag = 0;
	}
}
/***********************************************************************
�������ƣ�Delay_s(unsigned long ulVal)
��    �ܣ�����ѭ������һ������ʱ
���������
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
***********************************************************************/
void Delay_s(unsigned long ulVal) /* ����ѭ������һ������ʱ */
{
	while ( --ulVal != 0 );
}

/***********************************************************************
�������ƣ�err_t RS232_TCP_Connected(void *arg,struct tcp_pcb *pcb,err_t err)
��    �ܣ����RS232��TCP�����ݷ���
���������
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺����һ���ص���������TCP�ͻ�����������ӽ���ʱ������
***********************************************************************/
err_t RS232_TCP_Connected(void *arg,struct tcp_pcb *pcb,err_t err)
{
	tcp_write(pcb,RS232_rec_buf,RS232_rec_counter,0);//sizeof(RS232_rec_buf),0); /* �������� */
	RS232_rec_counter = 0; //������󣬽����ռ�����ֵ����
	tcp_close(pcb);
	return ERR_OK;
}

/***********************************************************************
�������ƣ�err_t RS485_TCP_Connected(void *arg,struct tcp_pcb *pcb,err_t err)
��    �ܣ����RS485��TCP�����ݷ���
���������
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺����һ���ص���������TCP�ͻ�����������ӽ���ʱ������
***********************************************************************/
err_t RS485_TCP_Connected(void *arg,struct tcp_pcb *pcb,err_t err)
{
	tcp_write(pcb,RS485_rec_buf,RS485_rec_counter,0);//sizeof(RS485_rec_buf),0); /* �������� */
	RS485_rec_counter = 0; //������󣬽����ռ�����ֵ����
	tcp_close(pcb);
	return ERR_OK;
}

/***********************************************************************
�������ƣ�RS232_TCP_Client_Init(u16_t local_port,u16_t remote_port,unsigned char a,unsigned char b,unsigned char c,unsigned char d)
��    �ܣ����RS232��TCP��tcp�ͻ��˳�ʼ��
���������local_port���ض˿ںţ�remote_port��Ŀ��˿ںţ�a,b,c,d��������ip
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺��������ΪTCP�ͻ���
***********************************************************************/
void RS232_TCP_Client_Init(u16_t local_port,u16_t remote_port,unsigned char a,unsigned char b,unsigned char c,unsigned char d)
{
	struct tcp_pcb *Clipcb;
	struct ip_addr ipaddr;
	IP4_ADDR(&ipaddr,a,b,c,d);
	Clipcb = tcp_new(); /* ����ͨ�ŵ�TCP���ƿ�(Clipcb) */
	tcp_bind(Clipcb,IP_ADDR_ANY,local_port); /* �󶨱���IP��ַ�Ͷ˿ںţ�����ip��ַ��LwIP_Init()���Ѿ���ʼ�� */
	tcp_connect(Clipcb,&ipaddr,remote_port,RS232_TCP_Connected);//ע��ص�����	
}

/***********************************************************************
�������ƣ�RS485_TCP_Client_Init(u16_t local_port,u16_t remote_port,unsigned char a,unsigned char b,unsigned char c,unsigned char d)
��    �ܣ����RS485��TCP��tcp�ͻ��˳�ʼ��
���������local_port���ض˿ںţ�remote_port��Ŀ��˿ںţ�a,b,c,d��������ip
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺��������ΪTCP�ͻ���
***********************************************************************/
void RS485_TCP_Client_Init(u16_t local_port,u16_t remote_port,unsigned char a,unsigned char b,unsigned char c,unsigned char d)
{
	struct tcp_pcb *Clipcb;
	struct ip_addr ipaddr;
	IP4_ADDR(&ipaddr,a,b,c,d);           //������IP��ַ
	Clipcb = tcp_new(); /* ����ͨ�ŵ�TCP���ƿ�(Clipcb) */
	tcp_bind(Clipcb,IP_ADDR_ANY,local_port); /* �󶨱���IP��ַ�Ͷ˿ں� ������ip��ַ��LwIP_Init()���Ѿ���ʼ��*/
	tcp_connect(Clipcb,&ipaddr,remote_port,RS485_TCP_Connected);//ע��ص�����
}
