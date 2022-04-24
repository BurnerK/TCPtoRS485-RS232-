/********************************************************************
�ļ����ƣ�RS232_485_CAN_to_TCP.h
��    �ܣ����RS232/485/CAN��TCPЭ��ת�������ݶ���
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
*********************************************************************/
 #ifndef _RS232_485_CAN_TO_TCP_H_
 #define _RS232_485_CAN_TO_TCP_H_
 #define RS_485_RX_EN 	GPIO_ResetBits(GPIOD , GPIO_Pin_4)	//����ʹ��
 #define RS_485_TX_EN 	GPIO_SetBits(GPIOD , GPIO_Pin_4)  	//����ʹ��
//********���¶���RS232/485/CAN1/CAN2��Ӧ�ķ�����ip��ַ**************//
//#define SERVER_IP_ADDR     		192,168,1,2
#define RS232_SERVER_IP         192,168,1,21
#define RS485_SERVER_IP         192,168,1,21		
#define CAN1_SERVER_IP          192,168,1,21
#define CAN2_SERVER_IP          192,168,1,21

//********���¶���RS232/485/CAN1/CAN2��Ӧ�ķ�������Զ�ˣ��˿ں�******//
#define RS232_REMOTE_PORT 		1026 
#define RS485_REMOTE_PORT 		1027
#define CAN1_REMOTE_PORT 		1028
#define CAN2_REMOTE_PORT 		1029

//********���¶���RS232/485/CAN1/CAN2��Ӧ�ı��ض˿ں�***************//
#define RS232_LOCAL_PORT 		1030
#define RS485_LOCAL_PORT 		1031
#define CAN1_LOCAL_PORT 		1032
#define CAN2_LOCAL_PORT 		1033

//********���¶���RS232/485/CAN1/CAN2���ջ�������С*****************//
#define RS232_REC_BUF    		100
#define RS485_REC_BUF    		100
#define CAN1_REC_BUF     		100
#define CAN2_REC_BUF     		100



#endif