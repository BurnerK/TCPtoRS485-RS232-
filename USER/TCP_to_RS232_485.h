/********************************************************************
文件名称：TCP_to_RS232_485_CAN.h
功    能：完成TCP到RS232/485/CAN协议转换的数据定义
编写时间：2013.4.25
编 写 人：
注    意：
*********************************************************************/
#ifndef _TCP_TO_RS232_485_CAN_H_
#define _TCP_TO_RS232_485_CAN_H_

#define RS_485_RX_EN 	GPIO_ResetBits(GPIOD , GPIO_Pin_4)	//接收使能
#define RS_485_TX_EN 	GPIO_SetBits(GPIOD , GPIO_Pin_4)  	//发送使能

#endif
