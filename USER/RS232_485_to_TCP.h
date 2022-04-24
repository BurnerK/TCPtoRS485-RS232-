/********************************************************************
文件名称：RS232_485_to_TCP.h
功    能：完成RS232/485到TCP协议转换的数据定义
编写时间：2013.4.25
编 写 人：
注    意：
*********************************************************************/
#ifndef _RS232_485_TO_TCP_H_
#define _RS232_485_TO_TCP_H_

#include <includes.h>

/***************开发板ip及MAC定义*************************/
#define BOARD_IP  			 192,168,1,252   		//开发板ip 
#define BOARD_NETMASK   	 255,255,255,0   		//开发板子网掩码
#define BOARD_WG		   	 255,255,1,1   			//开发板子网关
#define BOARD_MAC_ADDR       0,0,0,0,0,1			//开发板MAC地址

//********以下定义开发板作为服务器时各个通讯口对应的端口号************************************/

#define RS232_SERVER_PORT 		1030 
#define RS485_SERVER_PORT 		1031

#define CAN1_SERVER_PORT 		1032 
#define CAN2_SERVER_PORT 		1033

//********以下定义开发板作为客户端时RS232/485/CAN对应的服务器ip地址(即远端ip地址)**************//
#define RS232_SERVER_IP         192,168,1,21
#define RS485_SERVER_IP         192,168,1,21
#define CAN1_SERVER_IP          192,168,1,21
#define CAN2_SERVER_IP          192,168,1,21		

//********以下定义开发板作为客户端时RS232/485/CAN对应的服务器远端端口号******//
//#define RS232_REMOTE_PORT 		1026 
//#define RS485_REMOTE_PORT 		1027
//
//#define CAN1_REMOTE_PORT 		1028 
//#define CAN2_REMOTE_PORT 		1029

//********以下定义开发板作为客户端时RS232/485/CAN对应的本地端口号***************//
#define RS232_CLIENT_PORT 		4000 
#define RS485_CLIENT_PORT 		4001

#define CAN1_CLIENT_PORT 		4002 
#define CAN2_CLIENT_PORT 		4003



//********以下定义RS232/485接收缓冲区大小*****************//
#define RS232_REC_BUF_SIZE    	2048
#define RS485_REC_BUF_SIZE    	2048

#define CAN1_SEND_ID            0x18400000
#define CAN2_SEND_ID            0x18412345
#define RS232_TIMER_OUT         5//RS232接收字节间的最大时间间隔。用这个时间来分割232数据帧

//#define RS232_REC_BUF    		100
//#define RS485_REC_BUF    		100
//#define CAN1_REC_BUF     		100
//#define CAN2_REC_BUF     		100

extern  struct tcp_pcb *can1_pcb;
extern  struct tcp_pcb *can2_pcb;
extern  struct tcp_pcb *RS232_pcb;
extern  struct tcp_pcb *RS485_pcb;
extern  unsigned char tcp_rec_flag;

extern volatile unsigned int rs485_time_out_flag;
extern unsigned int rs485_timer_counter;
extern unsigned char rs485_start_time_flag ;

extern volatile unsigned int rs232_time_out_flag;
extern unsigned int rs232_timer_counter;
extern unsigned char rs232_start_time_flag;

extern unsigned char RS232_TCP_send_counter ;
extern unsigned char RS485_TCP_send_counter ;

extern unsigned char RS232_rec_counter;
extern unsigned char RS485_rec_counter;
extern unsigned int RS232_REC_time_counter;
extern unsigned int RS485_REC_time_counter;
extern unsigned char RS232_rec_buf[RS232_REC_BUF_SIZE];
extern unsigned char RS485_rec_buf[RS485_REC_BUF_SIZE];

extern OS_EVENT  *sem_RS232_rec_flag;			//RS232接收完一桢数据信号量定义
extern OS_EVENT  *sem_RS485_rec_flag;			//RS485接收完一桢数据信号量定义

void RS232_to_TCP(void);
void RS485_to_TCP(void);

#endif