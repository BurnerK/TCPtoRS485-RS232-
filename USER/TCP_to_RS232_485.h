/********************************************************************
�ļ����ƣ�TCP_to_RS232_485_CAN.h
��    �ܣ����TCP��RS232/485/CANЭ��ת�������ݶ���
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
*********************************************************************/
#ifndef _TCP_TO_RS232_485_CAN_H_
#define _TCP_TO_RS232_485_CAN_H_

#define RS_485_RX_EN 	GPIO_ResetBits(GPIOD , GPIO_Pin_4)	//����ʹ��
#define RS_485_TX_EN 	GPIO_SetBits(GPIOD , GPIO_Pin_4)  	//����ʹ��

#endif
