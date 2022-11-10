#ifndef __SYS_USART_H_
#define __SYS_USART_H_

#include "stm32f10x_conf.h"

/*****************************************************************************
 ** 移植配置
****************************************************************************/
#define USARTx_DEBUG         USART1    // 用于重定向printf, 使printf通过USARTx发送数据





/*****************************************************************************
 ** 全局变量 
****************************************************************************/
typedef struct 
{
    uint8_t  USART1InitOK;             // 初始化标记; 0=未初始化, 1=已初始化
    uint8_t  USART1RecivedFlag;        // 接收到新的一帧数据；0-无，1-有新数据；在处理数据后，标记为0
    uint8_t  USART1RecivedCNT;         // 接收到多少个字节数据
    uint8_t  USART1RecivedBuffer[256]; // 接收到数据的缓存
    
    uint8_t  USART2InitOK;             // 初始化标记; 0=未初始化, 1=已初始化
    uint8_t  USART2RecivedFlag;        // 接收到新的一帧数据；0-无，1-有新数据；在处理数据后，标记为0
    uint8_t  USART2RecivedCNT;         // 接收到多少个字节数据
    uint8_t  USART2RecivedBuffer[256]; // 接收到数据的缓存
    
    uint8_t  USART3InitOK;             // 初始化标记; 0=未初始化, 1=已初始化
    uint8_t  USART3RecivedFlag;        // 接收到新的一帧数据；0-无，1-有新数据；在处理数据后，标记为0
    uint8_t  USART3RecivedCNT;         // 接收到多少个字节数据
    uint8_t  USART3RecivedBuffer[256]; // 接收到数据的缓存 
    
    uint8_t  UART4InitOK;              // 初始化标记; 0=未初始化, 1=已初始化
    uint8_t  UART4RecivedFlag;         // 接收到新的一帧数据；0-无，1-有新数据；在处理数据后，标记为0
    uint8_t  UART4RecivedCNT;          // 接收到多少个字节数据
    uint8_t  UART4RecivedBuffer[256];  // 接收到数据的缓存
    
    uint8_t  UART5InitOK;              // 初始化标记; 0=未初始化, 1=已初始化
    uint8_t  UART5RecivedFlag;         // 接收到新的一帧数据；0-无，1-有新数据；在处理数据后，标记为0
    uint8_t  UART5RecivedCNT;          // 接收到多少个字节数据
    uint8_t  UART5RecivedBuffer[256];  // 接收到数据的缓存
    
    uint8_t  testCNT;                  // 仅用于测试
    
}xUSATR_TypeDef;

extern xUSATR_TypeDef  xUSART;         // 声明为全局变量,方便记录信息、状态
    




/*****************************************************************************
 ** 声明全局函数
****************************************************************************/
// USART1
void  USART1_Init (uint32_t baudrate);                          // 初始化串口的GPIO、通信参数配置、中断优先级; (波特率可设、8位数据、无校验、1个停止位)
void  USART1_GetBuffer (uint8_t* buffer, uint8_t* cnt);         // 获取接收到的数据
void  USART1_SendData (uint8_t* buf, uint8_t cnt);              // 通过中断发送数据，适合各种数据
void  USART1_printf (char* stringTemp);                         // 通过中断发送字符串，适合字符串，长度在256个长度内的
void  USART1_printfForDMA(char* stringTemp) ;                   // 通过DMA发送数据，适合一次过发送数据量特别大的字符串，省了占用中断的时间
// USART2
void  USART2_Init (uint32_t baudrate);                          // 初始化串口的GPIO、通信参数配置、中断优先级; (波特率可设、8位数据、无校验、1个停止位)
void  USART2_GetBuffer (uint8_t* buffer, uint8_t* cnt);         // 获取接收到的数据
void  USART2_SendData (uint8_t* buf, uint8_t cnt);              // 通过中断发送数据，适合各种数据
void  USART2_printf (char* stringTemp);                         // 通过中断发送字符串，适合字符串，长度在256个长度内的
// USART3
void  USART3_Init (uint32_t baudrate);                          // 初始化串口的GPIO、通信参数配置、中断优先级; (波特率可设、8位数据、无校验、1个停止位)
void  USART3_GetBuffer (uint8_t* buffer, uint8_t* cnt);         // 获取接收到的数据
void  USART3_SendData (uint8_t* buf, uint8_t cnt);              // 通过中断发送数据，适合各种数据
void  USART3_printf (char* stringTemp);                         // 通过中断发送字符串，适合字符串，长度在256个长度内的
// USART4
void  UART4_Init (uint32_t baudrate);                           // 初始化串口的GPIO、通信参数配置、中断优先级; (波特率可设、8位数据、无校验、1个停止位)
void  UART4_GetBuffer (uint8_t* buffer, uint8_t* cnt);          // 获取接收到的数据
void  UART4_SendData (uint8_t* buf, uint8_t cnt);               // 通过中断发送数据，适合各种数据
void  UART4_printf (char* stringTemp);                          // 通过中断发送字符串，适合字符串，长度在256个长度内的
// USART5
void  UART5_Init (uint32_t baudrate);                           // 初始化串口的GPIO、通信参数配置、中断优先级; (波特率可设、8位数据、无校验、1个停止位)
void  UART5_GetBuffer (uint8_t* buffer, uint8_t* cnt);          // 获取接收到的数据
void  UART5_SendData (uint8_t* buf, uint8_t cnt);               // 通过中断发送数据，适合各种数据
void  UART5_printf (char* stringTemp);                          // 通过中断发送字符串，适合字符串，长度在256个长度内的

#endif

