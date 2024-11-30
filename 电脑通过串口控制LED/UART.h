#ifndef __UART_H__
#define __UART_H__

/**
  * @brief  串口初始化，4800bps@11.0592MHz
  * @param  无
  * @retval 无
  */
void UART_Init();

/**
  * @brief  串口发送一个字节数据
  * @param  Byte要发送的一个字节数据
  * @retval 无
  */
void UART_SendByte(unsigned char Byte);

#endif