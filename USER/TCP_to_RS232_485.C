/***********************************************************************
文件名称：TCP_to_RS232_485_CAN.C
功    能：完成TCP到RS232/485/CAN协议的转换
编写时间：2013.4.25
编 写 人：赵志峰
注    意：
***********************************************************************/
#include "stm32_eth.h"
#include "lwip/tcp.h"

#include "TCP_to_RS232_485.h"
#include "RS232_485_to_TCP.h"

#define RS_485_RX_EN 	GPIO_ResetBits(GPIOD , GPIO_Pin_4)	//接收使能
#define RS_485_TX_EN 	GPIO_SetBits(GPIOD , GPIO_Pin_4)  	//发送使能
#define RS232_485_CAN_SEND_BUF_SIZE  100
#define ERR_FLAG    				  2


unsigned char TCP_RS485_flag = 0;
unsigned char TCP_RS232_flag = 0;
unsigned char TCP_CAN1_flag = 0;
unsigned char TCP_CAN2_flag = 0;

unsigned int RS232_send_lenth = 0;
unsigned int RS485_send_lenth = 0;
unsigned int CAN1_send_lenth = 0;
unsigned int CAN2_send_lenth = 0;

unsigned char RS232_REC_Flag = 0;
unsigned char RS485_REC_Flag = 0;
unsigned char CAN1_REC_Flag  = 0;
unsigned char CAN2_REC_Flag  = 0;
unsigned char RS232_rec_counter = 0;
unsigned char RS485_rec_counter = 0;
unsigned char CAN1_rec_counter = 0;
unsigned char CAN2_rec_counter = 0;
unsigned char RS232_rec_buf[RS232_REC_BUF_SIZE]={1};
unsigned char RS485_rec_buf[RS485_REC_BUF_SIZE];

struct tcp_pcb *RS232_pcb;
struct tcp_pcb *RS485_pcb;

unsigned int RS232_REC_time_counter = 0;
unsigned int RS485_REC_time_counter = 0;

unsigned char RS232_TCP_send_counter = 0;
unsigned char RS485_TCP_send_counter = 0;

unsigned int RS232_485_can_send_lenth = 0;

char *RS232_send_data;
char *RS485_send_data;
char *CAN1_send_data;
char *CAN2_send_data;

char RS232_485_can_send_data[RS232_485_CAN_SEND_BUF_SIZE];
/*************************TCP测试的WEB服务器建立网页与服务器声明***********************/
void RS232_Send_Data(unsigned char *data,unsigned int len);



/***********************************************************************
函数名称：RS232_Send_Data(unsigned char *data,unsigned int len)
功    能：RS232数据发送			 
输入参数：data指向数据的指针；len待发送的数据长度
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
void RS232_Send_Data(unsigned char *data,unsigned int len)
{
 	unsigned int i = 0;
	for(i = 0;i < len;i ++)
	{			
		USART2->DR = data[i];
		while((USART2->SR&0X40)==0);	
	}
	
}

/***********************************************************************
函数名称：RS485_Send_Data(unsigned char *data,unsigned int len)
功    能：RS485数据发送			 
输入参数：data指向数据的指针；len待发送的数据长度
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
void RS485_Send_Data(unsigned char *data,unsigned int len)
{
 	unsigned int i = 0;
	RS_485_TX_EN;		//485发送使能
	OSTimeDlyHMSM(0, 0, 0, 10);
	for(i = 0;i < len;i ++)
	{			
		USART1->DR = data[i];
		while((USART1->SR&0X40)==0);	
	}
	OSTimeDlyHMSM(0, 0, 0, 10); 		//稍作延时，注意延时的长短根据波特率来定，波特率越小，延时应该越长
	RS_485_RX_EN;  		//485接收使能
	
}
/***********************************************************************
函数名称：tcp_232_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
功    能：RS232端口对应的TCP数据接收和RS232数据的发送
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：这是一个回调函数，当一个TCP段到达这个连接时会被调用
***********************************************************************/
static err_t tcp_232_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
	char *data;
	char *data_temp;
	unsigned char temp[1];
	RS232_pcb = pcb;
	if(p != NULL)
	{
		tcp_recved(pcb, p->tot_len);				//获取数据长度 tot_len：tcp数据块的长度
		RS232_Send_Data(p->payload,p->tot_len);	 	// payload为TCP数据块的起始位置
		pbuf_free(p); 											/* 释放该TCP段 */
	}
	else
	{
		tcp_close(pcb); 							//如果客户端断开连接，则服务器也应断开
	}
	err = ERR_OK;
	return err;
}

/***********************************************************************
函数名称：tcp_485_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
功    能：RS485端口对应的TCP数据接收和RS485数据的发送
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：这是一个回调函数，当一个TCP段到达这个连接时会被调用
***********************************************************************/
static err_t tcp_485_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
	char *data;
	char *data_temp;
	unsigned char temp[1];
	RS485_pcb = pcb;
	if(p != NULL)									
	{
		tcp_recved(pcb, p->tot_len);				//获取数据长度：tot_len为tcp数据块的长度
		RS485_Send_Data(p->payload,p->tot_len);	    // payload为tcp数据块的位置
		pbuf_free(p); 											/* 释放该TCP段 */
	}
	else
	{
		tcp_close(pcb); 							//如果客户端断开连接，则服务器也应断开
	}						
	err = ERR_OK;
	return err;
}


/***********************************************************************
函数名称：tcp_232_server_accept(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
功    能：RS232服务器的回调函数
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：这是一个回调函数，当一个连接已经接受时会被调用
***********************************************************************/
static err_t tcp_232_server_accept(void *arg,struct tcp_pcb *pcb,err_t err)
{
	tcp_setprio(pcb, TCP_PRIO_MIN + 1); 		/* 设置回调函数优先级，当存在几个连接时特别重要,此函数必须调用*/
	tcp_recv(pcb,tcp_232_server_recv); 				/* 设置TCP段到时的回调函数 */
	RS232_pcb = pcb;
	err = ERR_OK;
	return err;
}

/***********************************************************************
函数名称：tcp_485_server_accept(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
功    能：RS485服务器的回调函数
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：这是一个回调函数，当一个连接已经接受时会被调用
***********************************************************************/
static err_t tcp_485_server_accept(void *arg,struct tcp_pcb *pcb,err_t err)
{
	tcp_setprio(pcb, TCP_PRIO_MIN); 		/* 设置回调函数优先级，当存在几个连接时特别重要,此函数必须调用*/
	tcp_recv(pcb,tcp_485_server_recv); 				/* 设置TCP段到时的回调函数 */
	RS485_pcb = pcb;
	err = ERR_OK;
	return err;
}

/***********************************************************************
函数名称：TCP_server_init(void)
功    能：完成TCP服务器的初始化，主要是使得TCP通讯快进入监听状态
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
void TCP_server_init(void)
{
	struct tcp_pcb *pcb;

	/********************以下为RS232端口初始化*********************************/
	pcb = tcp_new(); 								/* 建立通信的TCP控制块(pcb) */
	tcp_bind(pcb,IP_ADDR_ANY,RS232_SERVER_PORT); 	/* 绑定本地IP地址和端口号（作为tcp服务器） */
	pcb = tcp_listen(pcb); 							/* 进入监听状态 */
	tcp_accept(pcb,tcp_232_server_accept); 			/* 设置有连接请求时的回调函数 */

	/********************以下为RS485端口初始化*********************************/
	pcb = tcp_new(); 								/* 建立通信的TCP控制块(pcb) */
	tcp_bind(pcb,IP_ADDR_ANY,RS485_SERVER_PORT); 	/* 绑定本地IP地址和端口号（作为tcp服务器） */
	pcb = tcp_listen(pcb); 							/* 进入监听状态 */
	tcp_accept(pcb,tcp_485_server_accept); 			/* 设置有连接请求时的回调函数 */
}
