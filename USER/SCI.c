
#include "stm32f10x.h"
#include "TCP_to_RS232_485.h"
#include "RS232_485_to_TCP.h"
#include <includes.h> 
/***********************************************************************
�ļ����ƣ�SCI.C
��    �ܣ���ɶԴ��ڵĻ�������
��дʱ�䣺2012.11.22
�� д �ˣ�
ע    �⣺
***********************************************************************/

extern unsigned char RS232_REC_Flag;
extern unsigned char RS485_REC_Flag;
unsigned char SCI_receive_counter = 0;
extern unsigned char RS232_rec_counter;
extern unsigned char RS485_rec_counter;
extern unsigned char RS232_rec_buf[RS232_REC_BUF_SIZE];
extern unsigned char RS485_rec_buf[RS485_REC_BUF_SIZE];

volatile unsigned int rs485_time_out_flag = 0;
unsigned int rs485_timer_counter = 0;
unsigned char rs485_start_time_flag = 0;

volatile unsigned int rs232_time_out_flag = 0;
unsigned int rs232_timer_counter = 0;
unsigned char rs232_start_time_flag = 0;

extern OS_EVENT  *sem_RS232_rec_flag;			//RS232������һ�������ź�������
extern OS_EVENT  *sem_RS485_rec_flag;			//RS485������һ�������ź�������
void SCI_NVIC_Configuration(void);

void USART_Configuration(void)
{ 
  
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure; 
  USART_ClockInitTypeDef  USART_ClockInitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO,ENABLE);
  /*
  *  USART1_TX -> PA9 , USART1_RX ->	PA10
  */				
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);		   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 19200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//9λ����
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//1λֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No;//żУ��
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //���ͺͽ���ʹ��
  USART_Init(USART1, &USART_InitStructure); 

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  /***********************************************************************************
	  void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState)
	  ʹ�ܻ���ʧ��USART����
	  USARTx��x������1��2����3����ѡ��USART����
	  NewState: ����USARTx����״̬
	  �����������ȡ��ENABLE����DISABLE
  ***********************************************************************************/
  USART_Cmd(USART1, ENABLE); 
  USART_ClearITPendingBit(USART1, USART_IT_TC);//����ж�TCλ



  //����Ϊ����2����
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
  /*
  *  USART1_TX -> PA2 , USART1_RX ->	PA3
  */				
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);		   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_9b;//9λ����
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//1λֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_Even;//żУ��
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //���ͺͽ���ʹ��
  USART_Init(USART2, &USART_InitStructure); 
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART2, ENABLE); 
  USART_ClearITPendingBit(USART2, USART_IT_TC);//����ж�TCλ

  /***********************************GPIOD.4��RS485�������******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/***********************************************************************
�������ƣ�void USART1_IRQHandler(void) 
��    �ܣ����SCI�����ݵĽ��գ�������ʶ
���������
���������
��дʱ�䣺2012.11.22
�� д �ˣ�
ע    ��
***********************************************************************/
void USART1_IRQHandler(void)  
{
	INT8U err;
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();    // ���ж�                               
    OSIntNesting++;	   		//�ж�Ƕ�ײ�����֪ͨucos
    OS_EXIT_CRITICAL();	   	//���ж�

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{	
		RS485_rec_buf[RS485_rec_counter] = USART1->DR;//
		RS485_rec_counter ++;
		rs485_start_time_flag = 1;	 //�ӽ��ܵ�����ֽڿ�ʼ����ʱ��
		RS485_REC_time_counter = 0;
		if(RS485_rec_counter > RS485_REC_BUF_SIZE)//�������ջ�������С
		{
			RS485_rec_counter = 0;
			rs485_start_time_flag = 1;
			//err = OSSemPost(sem_RS485_rec_flag);  //�׳�һ���ź�����ʾRS485�Ѿ��������һ֡����
			rs485_start_time_flag = 0;//����������ʱ��
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) 
	{
        USART_ClearITPendingBit(USART1, USART_IT_TXE);           /* Clear the USART transmit interrupt                  */
    }
	OSIntExit();//�ж��˳���֪ͨucos�����þ����ӣ�	
}


	/***********************************************************************
�������ƣ�void USART2_IRQHandler(void) 
��    �ܣ����SCI�����ݵĽ��գ�������ʶ
���������
���������
��дʱ�䣺2012.11.22
�� д �ˣ�
ע    �⣺
***********************************************************************/
void USART2_IRQHandler(void)  
{
	INT8U err;
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();    // ���ж�                               
    OSIntNesting++;	   		//�ж�Ƕ�ײ�����֪ͨucos
    OS_EXIT_CRITICAL();	   	//���ж�

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//���յ�������
	{	
		RS232_rec_buf[RS232_rec_counter] = USART2->DR;//
		RS232_rec_counter ++;
		rs232_start_time_flag = 1;	  //�ӽ��ܵ�����ֽڿ�ʼ����ʱ��
		RS232_REC_time_counter = 0;
		if(RS232_rec_counter > RS232_REC_BUF_SIZE)//�������ջ�������С
		{
			RS232_rec_counter = 0;
			rs232_start_time_flag = 1;
			//err = OSSemPost(sem_RS232_rec_flag);  //�׳�һ���ź�����ʾ RS232�Ѿ��������һ֡����
			rs232_start_time_flag = 0;//������ʱ�����
		}		
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
	if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET) 
	{
        USART_ClearITPendingBit(USART2, USART_IT_TXE);           /* Clear the USART transmit interrupt                  */
    }
	OSIntExit();//�ж��˳���֪ͨucos�����þ����ӣ�
}