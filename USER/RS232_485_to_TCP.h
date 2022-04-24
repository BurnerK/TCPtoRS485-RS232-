/********************************************************************
�ļ����ƣ�RS232_485_to_TCP.h
��    �ܣ����RS232/485��TCPЭ��ת�������ݶ���
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
*********************************************************************/
#ifndef _RS232_485_TO_TCP_H_
#define _RS232_485_TO_TCP_H_

#include <includes.h>

/***************������ip��MAC����*************************/
#define BOARD_IP  			 192,168,1,252   		//������ip 
#define BOARD_NETMASK   	 255,255,255,0   		//��������������
#define BOARD_WG		   	 255,255,1,1   			//������������
#define BOARD_MAC_ADDR       0,0,0,0,0,1			//������MAC��ַ

//********���¶��忪������Ϊ������ʱ����ͨѶ�ڶ�Ӧ�Ķ˿ں�************************************/

#define RS232_SERVER_PORT 		1030 
#define RS485_SERVER_PORT 		1031

#define CAN1_SERVER_PORT 		1032 
#define CAN2_SERVER_PORT 		1033

//********���¶��忪������Ϊ�ͻ���ʱRS232/485/CAN��Ӧ�ķ�����ip��ַ(��Զ��ip��ַ)**************//
#define RS232_SERVER_IP         192,168,1,21
#define RS485_SERVER_IP         192,168,1,21
#define CAN1_SERVER_IP          192,168,1,21
#define CAN2_SERVER_IP          192,168,1,21		

//********���¶��忪������Ϊ�ͻ���ʱRS232/485/CAN��Ӧ�ķ�����Զ�˶˿ں�******//
//#define RS232_REMOTE_PORT 		1026 
//#define RS485_REMOTE_PORT 		1027
//
//#define CAN1_REMOTE_PORT 		1028 
//#define CAN2_REMOTE_PORT 		1029

//********���¶��忪������Ϊ�ͻ���ʱRS232/485/CAN��Ӧ�ı��ض˿ں�***************//
#define RS232_CLIENT_PORT 		4000 
#define RS485_CLIENT_PORT 		4001

#define CAN1_CLIENT_PORT 		4002 
#define CAN2_CLIENT_PORT 		4003



//********���¶���RS232/485���ջ�������С*****************//
#define RS232_REC_BUF_SIZE    	2048
#define RS485_REC_BUF_SIZE    	2048

#define CAN1_SEND_ID            0x18400000
#define CAN2_SEND_ID            0x18412345
#define RS232_TIMER_OUT         5//RS232�����ֽڼ�����ʱ�����������ʱ�����ָ�232����֡

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

extern OS_EVENT  *sem_RS232_rec_flag;			//RS232������һ�������ź�������
extern OS_EVENT  *sem_RS485_rec_flag;			//RS485������һ�������ź�������

void RS232_to_TCP(void);
void RS485_to_TCP(void);

#endif