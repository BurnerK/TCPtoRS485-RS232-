/***********************************************************************
文件名称：TCP_to_RS232_485_CAN.C
功    能：完成TCP到RS232/485/CAN协议的转换
编写时间：2013.4.25
编 写 人：赵志峰
注    意： 为了区分TCP数据是发往RS232还是RS485还是CAN，约定如下：

TCP前N个字节      发送到的目标
 "RS232:"		    RS232
 "RS485:"			RS485
 "CAN1:"			    CAN1
 "CAN2:"				CAN2
具体可参考函数“tcp_server_recv” 
***********************************************************************/
#include "stm32_eth.h"
#include "lwip/tcp.h"

#include "TCP_to_RS232_485_CAN.h"
#include "RS232_485_CAN_to_TCP.h"

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

unsigned int RS232_485_can_send_lenth = 0;

char *RS232_send_data;
char *RS485_send_data;
char *CAN1_send_data;
char *CAN2_send_data;

char RS232_485_can_send_data[RS232_485_CAN_SEND_BUF_SIZE];
/*************************TCP测试的WEB服务器建立网页与服务器声明***********************/
const static u8 indexdata[]=" ";
const static u8 http_html_hdr []="HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
void RS232_Send_Data(unsigned char *data,unsigned int len);

/***********************************************************************
函数名称：TCP_to_RS232(void)
功    能：TCP到RS232协议转换的入口函数
输入参数：
输出参数：
编写时间：2013.4.25
编 写 人：
注    意：在该函数中首先判断TCP是否接收到了发往RS232的数据，若是，则向RS232发送数据。
			也可以把发送函数直接写在 tcp_server_recv函数中，此时该函数就不需要了。
***********************************************************************/
void TCP_to_RS232(void)
{
	if(TCP_RS232_flag == 1)	//TCP接收到了要发往RS232数据
	{	
		RS232_Send_Data(RS232_send_data,RS232_send_lenth);	//向RS232发送数据
		TCP_RS232_flag = 0;
	}
}

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
	Delay_s(10);		//稍作延时，注意延时的长短根据波特率来定，波特率越小，延时应该越长
	for(i = 0;i < len;i ++)
	{			
		USART1->DR = data[i];
		while((USART1->SR&0X40)==0);	
	}
	Delay_s(10); 		//稍作延时，注意延时的长短根据波特率来定，波特率越小，延时应该越长
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

	if(p != NULL)
	{
		tcp_recved(pcb, p->tot_len);				//获取数据长度 tot_len：tcp数据块的长度
		RS232_Send_Data(p->payload,p->tot_len);	 	// payload为TCP数据块的起始位置
		/******向客户端发送假数据*******************/
		tcp_write(pcb,http_html_hdr,0,0);//sizeof(http_html_hdr),0);
		pbuf_free(p); 											/* 释放该TCP段 */
	}
	//tcp_close(pcb); 											/* 作为TCP服务器不应主动关闭这个连接？ */
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

	if(p != NULL)									
	{
		tcp_recved(pcb, p->tot_len);				//获取数据长度：tot_len为tcp数据块的长度
		RS485_Send_Data(p->payload,p->tot_len);	    // payload为tcp数据块的位置

		/******向客户端发送假数据*******************/
		tcp_write(pcb,http_html_hdr,0,0);//sizeof(http_html_hdr),0);
		pbuf_free(p); 											/* 释放该TCP段 */
	}
	//tcp_close(pcb); 											/* 作为TCP服务器不主动关闭这个连接？ */
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
	tcp_bind(pcb,IP_ADDR_ANY,1034);//RS232_LOCAL_PORT); 	/* 绑定本地IP地址和端口号（作为tcp服务器） */
	pcb = tcp_listen(pcb); 							/* 进入监听状态 */
	tcp_accept(pcb,tcp_232_server_accept); 			/* 设置有连接请求时的回调函数 */

	/********************以下为RS485端口初始化*********************************/
	pcb = tcp_new(); 								/* 建立通信的TCP控制块(pcb) */
	tcp_bind(pcb,IP_ADDR_ANY,1035); 	/* 绑定本地IP地址和端口号（作为tcp服务器） */
	pcb = tcp_listen(pcb); 							/* 进入监听状态 */
	tcp_accept(pcb,tcp_485_server_accept); 			/* 设置有连接请求时的回调函数 */
}

