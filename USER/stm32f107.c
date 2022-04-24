/**
  ******************************************************************************
  * @file    stm32f107.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/20/2009
  * @brief   STM32F107 hardware configuration
  ******************************************************************************/
#include "RS232_485_to_TCP.h"
#include <includes.h>

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;

  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

  /* 2 bit for pre-emption priority, 2 bits for subpriority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
  
  /* Enable the Ethernet global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = ETH_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);   
  
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
 													
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

  	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

  													
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
  
}

/***********************************************************************
�������ƣ�TIM_Configuration(unsigned int time)
��    �ܣ���ʱ��2����
���������time����ʱʱ�䣬��λms��
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
***********************************************************************/
void TIM_Configuration(unsigned int time)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
  TIM_DeInit(TIM2);
  TIM_TimeBaseStructure.TIM_Period=time;		 					/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
																	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
  TIM_TimeBaseStructure.TIM_Prescaler= (12000 - 1);				    /* ʱ��Ԥ��Ƶ��   ���磺ʱ��Ƶ��=72MHZ/(ʱ��Ԥ��Ƶ+1) */
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 			/* ������Ƶ */
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		/* ���ϼ���ģʽ */
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    /* �������жϱ�־ */
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM2, ENABLE);											/* ����ʱ�� */
}
/***********************************************************************
�������ƣ�TIM2_IRQHandler(void)
��    �ܣ���ʱ��2�жϺ���
���������ÿ��	time��ms���ж�һ��
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺�ú�����Ҫ�ü���RS232�ֽ�ʱ������
***********************************************************************/
void TIM2_IRQHandler(void)
{
	INT8U err;
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();    // ���ж�                               
    OSIntNesting++;	   		//�ж�Ƕ�ײ�����֪ͨucos
    OS_EXIT_CRITICAL();	   	//���ж�

	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);

		/**************RS232��ʱ�����־***********/
		if(rs232_start_time_flag == 1)	//��ʼ��ʱ
		{
			RS232_REC_time_counter ++;
			if(RS232_REC_time_counter >= 1)
			{
				rs232_time_out_flag = 1;
				rs232_start_time_flag = 0;
				err = OSSemPost(sem_RS232_rec_flag);  //�׳�һ���ź�����ʾ RS232�Ѿ��������һ֡����
			}
		}
		/**************RS485��ʱ�����־***********/
		if(rs485_start_time_flag == 1)	//��ʼ��ʱ
		{
			RS485_REC_time_counter ++;
			if(RS485_REC_time_counter >= 1)
			{
				rs485_time_out_flag = 1;
				rs485_start_time_flag = 0;
				err = OSSemPost(sem_RS485_rec_flag);  //�׳�һ���ź�����ʾ RS232�Ѿ��������һ֡����
			}
		}	 
     }
	OSIntExit();//�ж��˳���֪ͨucos�����þ����ӣ�
}