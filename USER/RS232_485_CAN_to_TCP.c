/***********************************************************************
文件名称：RS232_485_CAN_to_TCP.C
功    能：完成RS232/485/CAN到TCP协议的转换
编写时间：2013.4.25
编 写 人：
注    意：
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
函数名称：RS232_to_TCP(void)
功    能：RS232到TCP的协议转换的入口函数
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：在该函数中首先判断RS232是否接收完一桢数据（一桢数据以‘？’‘;'结尾），若接收完，
		  则将该帧数据以RS232_TCP_Client_Init函数指定的参数向TCP发送数据。
		  在UCOS下可以通过信号量的方式来判断一桢数据是否接收完成，所以该函数用不到了。
***********************************************************************/
void RS232_to_TCP(void)
{
	if(RS232_REC_Flag == 1)//RS232接收到一帧数据
	{	
		RS232_TCP_Client_Init(RS232_LOCAL_PORT,RS232_REMOTE_PORT,RS232_SERVER_IP);//向RS232端口（ip）发送数据
		RS232_REC_Flag = 0;
	}
}

/***********************************************************************
函数名称：RS485_to_TCP(void)
功    能：RS485到TCP的协议转换的入口函数
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：在该函数中首先判断RS485是否接收完一桢数据（一桢数据以‘？’‘;'结尾），若接收完，
		  则将该帧数据以RS485_TCP_Client_Init函数指定的参数向TCP发送数据。
		  在UCOS下可以通过信号量的方式来判断一桢数据是否接收完成，所以该函数用不到了。
***********************************************************************/
void RS485_to_TCP(void)
{
	if(RS485_REC_Flag == 1)//RS485接收到一帧数据
	{	
		RS485_TCP_Client_Init(RS485_LOCAL_PORT,RS485_REMOTE_PORT,RS485_SERVER_IP);//向RS485端口（ip）发送数据
		RS485_REC_Flag = 0;
	}
}
/***********************************************************************
函数名称：Delay_s(unsigned long ulVal)
功    能：利用循环产生一定的延时
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
void Delay_s(unsigned long ulVal) /* 利用循环产生一定的延时 */
{
	while ( --ulVal != 0 );
}

/***********************************************************************
函数名称：err_t RS232_TCP_Connected(void *arg,struct tcp_pcb *pcb,err_t err)
功    能：完成RS232到TCP的数据发送
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：这是一个回调函数，当TCP客户端请求的连接建立时被调用
***********************************************************************/
err_t RS232_TCP_Connected(void *arg,struct tcp_pcb *pcb,err_t err)
{
	tcp_write(pcb,RS232_rec_buf,RS232_rec_counter,0);//sizeof(RS232_rec_buf),0); /* 发送数据 */
	RS232_rec_counter = 0; //发送完后，将接收计数器值清零
	tcp_close(pcb);
	return ERR_OK;
}

/***********************************************************************
函数名称：err_t RS485_TCP_Connected(void *arg,struct tcp_pcb *pcb,err_t err)
功    能：完成RS485到TCP的数据发送
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：这是一个回调函数，当TCP客户端请求的连接建立时被调用
***********************************************************************/
err_t RS485_TCP_Connected(void *arg,struct tcp_pcb *pcb,err_t err)
{
	tcp_write(pcb,RS485_rec_buf,RS485_rec_counter,0);//sizeof(RS485_rec_buf),0); /* 发送数据 */
	RS485_rec_counter = 0; //发送完后，将接收计数器值清零
	tcp_close(pcb);
	return ERR_OK;
}

/***********************************************************************
函数名称：RS232_TCP_Client_Init(u16_t local_port,u16_t remote_port,unsigned char a,unsigned char b,unsigned char c,unsigned char d)
功    能：完成RS232到TCP的tcp客户端初始化
输入参数：local_port本地端口号；remote_port：目标端口号；a,b,c,d：服务器ip
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：开发板作为TCP客户端
***********************************************************************/
void RS232_TCP_Client_Init(u16_t local_port,u16_t remote_port,unsigned char a,unsigned char b,unsigned char c,unsigned char d)
{
	struct tcp_pcb *Clipcb;
	struct ip_addr ipaddr;
	IP4_ADDR(&ipaddr,a,b,c,d);
	Clipcb = tcp_new(); /* 建立通信的TCP控制块(Clipcb) */
	tcp_bind(Clipcb,IP_ADDR_ANY,local_port); /* 绑定本地IP地址和端口号，本地ip地址在LwIP_Init()中已经初始化 */
	tcp_connect(Clipcb,&ipaddr,remote_port,RS232_TCP_Connected);//注册回调函数	
}

/***********************************************************************
函数名称：RS485_TCP_Client_Init(u16_t local_port,u16_t remote_port,unsigned char a,unsigned char b,unsigned char c,unsigned char d)
功    能：完成RS485到TCP的tcp客户端初始化
输入参数：local_port本地端口号；remote_port：目标端口号；a,b,c,d：服务器ip
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：开发板作为TCP客户端
***********************************************************************/
void RS485_TCP_Client_Init(u16_t local_port,u16_t remote_port,unsigned char a,unsigned char b,unsigned char c,unsigned char d)
{
	struct tcp_pcb *Clipcb;
	struct ip_addr ipaddr;
	IP4_ADDR(&ipaddr,a,b,c,d);           //服务器IP地址
	Clipcb = tcp_new(); /* 建立通信的TCP控制块(Clipcb) */
	tcp_bind(Clipcb,IP_ADDR_ANY,local_port); /* 绑定本地IP地址和端口号 ，本地ip地址在LwIP_Init()中已经初始化*/
	tcp_connect(Clipcb,&ipaddr,remote_port,RS485_TCP_Connected);//注册回调函数
}
