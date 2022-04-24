
#include "stm32f10x.h"
#include "TCP_to_RS232_485.h"
#include "RS232_485_to_TCP.h"
#include <includes.h> 
/***********************************************************************
文件名称：SCI.C
功    能：完成对串口的基本操作
编写时间：2012.11.22
编 写 人：
注    意：
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

extern OS_EVENT  *sem_RS232_rec_flag;			//RS232接收完一桢数据信号量声明
extern OS_EVENT  *sem_RS485_rec_flag;			//RS485接收完一桢数据信号量声明
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
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//9位数据
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//1位停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;//偶校验
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制失能
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //发送和接受使能
  USART_Init(USART1, &USART_InitStructure); 

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  /***********************************************************************************
	  void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState)
	  使能或者失能USART外设
	  USARTx：x可以是1，2或者3，来选择USART外设
	  NewState: 外设USARTx的新状态
	  这个参数可以取：ENABLE或者DISABLE
  ***********************************************************************************/
  USART_Cmd(USART1, ENABLE); 
  USART_ClearITPendingBit(USART1, USART_IT_TC);//清除中断TC位



  //以下为串口2配置
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
  USART_InitStructure.USART_WordLength = USART_WordLength_9b;//9位数据
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//1位停止位
  USART_InitStructure.USART_Parity = USART_Parity_Even;//偶校验
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制失能
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //发送和接受使能
  USART_Init(USART2, &USART_InitStructure); 
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART2, ENABLE); 
  USART_ClearITPendingBit(USART2, USART_IT_TC);//清除中断TC位

  /***********************************GPIOD.4，RS485方向控制******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/***********************************************************************
函数名称：void USART1_IRQHandler(void) 
功    能：完成SCI的数据的接收，并做标识
输入参数：
输出参数：
编写时间：2012.11.22
编 写 人：
注    意
***********************************************************************/
void USART1_IRQHandler(void)  
{
	INT8U err;
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();    // 关中断                               
    OSIntNesting++;	   		//中断嵌套层数，通知ucos
    OS_EXIT_CRITICAL();	   	//开中断

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{	
		RS485_rec_buf[RS485_rec_counter] = USART1->DR;//
		RS485_rec_counter ++;
		rs485_start_time_flag = 1;	 //从接受到这个字节开始计算时间
		RS485_REC_time_counter = 0;
		if(RS485_rec_counter > RS485_REC_BUF_SIZE)//超过接收缓冲区大小
		{
			RS485_rec_counter = 0;
			rs485_start_time_flag = 1;
			//err = OSSemPost(sem_RS485_rec_flag);  //抛出一个信号量表示RS485已经接收完成一帧数据
			rs485_start_time_flag = 0;//不启动计算时间
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) 
	{
        USART_ClearITPendingBit(USART1, USART_IT_TXE);           /* Clear the USART transmit interrupt                  */
    }
	OSIntExit();//中断退出，通知ucos，（该句必须加）	
}


	/***********************************************************************
函数名称：void USART2_IRQHandler(void) 
功    能：完成SCI的数据的接收，并做标识
输入参数：
输出参数：
编写时间：2012.11.22
编 写 人：
注    意：
***********************************************************************/
void USART2_IRQHandler(void)  
{
	INT8U err;
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();    // 关中断                               
    OSIntNesting++;	   		//中断嵌套层数，通知ucos
    OS_EXIT_CRITICAL();	   	//开中断

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收到了数据
	{	
		RS232_rec_buf[RS232_rec_counter] = USART2->DR;//
		RS232_rec_counter ++;
		rs232_start_time_flag = 1;	  //从接受到这个字节开始计算时间
		RS232_REC_time_counter = 0;
		if(RS232_rec_counter > RS232_REC_BUF_SIZE)//超过接收缓冲区大小
		{
			RS232_rec_counter = 0;
			rs232_start_time_flag = 1;
			//err = OSSemPost(sem_RS232_rec_flag);  //抛出一个信号量表示 RS232已经接收完成一帧数据
			rs232_start_time_flag = 0;//不启动时间计算
		}		
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
	if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET) 
	{
        USART_ClearITPendingBit(USART2, USART_IT_TXE);           /* Clear the USART transmit interrupt                  */
    }
	OSIntExit();//中断退出，通知ucos，（该句必须加）
}