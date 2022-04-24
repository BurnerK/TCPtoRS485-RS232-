/********************************************************************
文件名称：RS232_485_CAN_to_TCP.h
功    能：完成RS232/485/CAN到TCP协议转换的数据定义
编写时间：2013.4.25
编 写 人：
注    意：
*********************************************************************/
 #ifndef _RS232_485_CAN_TO_TCP_H_
 #define _RS232_485_CAN_TO_TCP_H_
 #define RS_485_RX_EN 	GPIO_ResetBits(GPIOD , GPIO_Pin_4)	//接收使能
 #define RS_485_TX_EN 	GPIO_SetBits(GPIOD , GPIO_Pin_4)  	//发送使能
//********以下定义RS232/485/CAN1/CAN2对应的服务器ip地址**************//
//#define SERVER_IP_ADDR     		192,168,1,2
#define RS232_SERVER_IP         192,168,1,21
#define RS485_SERVER_IP         192,168,1,21		
#define CAN1_SERVER_IP          192,168,1,21
#define CAN2_SERVER_IP          192,168,1,21

//********以下定义RS232/485/CAN1/CAN2对应的服务器（远端）端口号******//
#define RS232_REMOTE_PORT 		1026 
#define RS485_REMOTE_PORT 		1027
#define CAN1_REMOTE_PORT 		1028
#define CAN2_REMOTE_PORT 		1029

//********以下定义RS232/485/CAN1/CAN2对应的本地端口号***************//
#define RS232_LOCAL_PORT 		1030
#define RS485_LOCAL_PORT 		1031
#define CAN1_LOCAL_PORT 		1032
#define CAN2_LOCAL_PORT 		1033

//********以下定义RS232/485/CAN1/CAN2接收缓冲区大小*****************//
#define RS232_REC_BUF    		100
#define RS485_REC_BUF    		100
#define CAN1_REC_BUF     		100
#define CAN2_REC_BUF     		100



#endif