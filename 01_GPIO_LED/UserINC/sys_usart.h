#ifndef __SYS_USART_H_
#define __SYS_USART_H_

#include "stm32f10x_conf.h"

/*****************************************************************************
 ** ��ֲ����
****************************************************************************/
#define USARTx_DEBUG         USART1    // �����ض���printf, ʹprintfͨ��USARTx��������





/*****************************************************************************
 ** ȫ�ֱ��� 
****************************************************************************/
typedef struct 
{
    uint8_t  USART1InitOK;             // ��ʼ�����; 0=δ��ʼ��, 1=�ѳ�ʼ��
    uint8_t  USART1RecivedFlag;        // ���յ��µ�һ֡���ݣ�0-�ޣ�1-�������ݣ��ڴ������ݺ󣬱��Ϊ0
    uint8_t  USART1RecivedCNT;         // ���յ����ٸ��ֽ�����
    uint8_t  USART1RecivedBuffer[256]; // ���յ����ݵĻ���
    
    uint8_t  USART2InitOK;             // ��ʼ�����; 0=δ��ʼ��, 1=�ѳ�ʼ��
    uint8_t  USART2RecivedFlag;        // ���յ��µ�һ֡���ݣ�0-�ޣ�1-�������ݣ��ڴ������ݺ󣬱��Ϊ0
    uint8_t  USART2RecivedCNT;         // ���յ����ٸ��ֽ�����
    uint8_t  USART2RecivedBuffer[256]; // ���յ����ݵĻ���
    
    uint8_t  USART3InitOK;             // ��ʼ�����; 0=δ��ʼ��, 1=�ѳ�ʼ��
    uint8_t  USART3RecivedFlag;        // ���յ��µ�һ֡���ݣ�0-�ޣ�1-�������ݣ��ڴ������ݺ󣬱��Ϊ0
    uint8_t  USART3RecivedCNT;         // ���յ����ٸ��ֽ�����
    uint8_t  USART3RecivedBuffer[256]; // ���յ����ݵĻ��� 
    
    uint8_t  UART4InitOK;              // ��ʼ�����; 0=δ��ʼ��, 1=�ѳ�ʼ��
    uint8_t  UART4RecivedFlag;         // ���յ��µ�һ֡���ݣ�0-�ޣ�1-�������ݣ��ڴ������ݺ󣬱��Ϊ0
    uint8_t  UART4RecivedCNT;          // ���յ����ٸ��ֽ�����
    uint8_t  UART4RecivedBuffer[256];  // ���յ����ݵĻ���
    
    uint8_t  UART5InitOK;              // ��ʼ�����; 0=δ��ʼ��, 1=�ѳ�ʼ��
    uint8_t  UART5RecivedFlag;         // ���յ��µ�һ֡���ݣ�0-�ޣ�1-�������ݣ��ڴ������ݺ󣬱��Ϊ0
    uint8_t  UART5RecivedCNT;          // ���յ����ٸ��ֽ�����
    uint8_t  UART5RecivedBuffer[256];  // ���յ����ݵĻ���
    
    uint8_t  testCNT;                  // �����ڲ���
    
}xUSATR_TypeDef;

extern xUSATR_TypeDef  xUSART;         // ����Ϊȫ�ֱ���,�����¼��Ϣ��״̬
    




/*****************************************************************************
 ** ����ȫ�ֺ���
****************************************************************************/
// USART1
void  USART1_Init (uint32_t baudrate);                          // ��ʼ�����ڵ�GPIO��ͨ�Ų������á��ж����ȼ�; (�����ʿ��衢8λ���ݡ���У�顢1��ֹͣλ)
void  USART1_GetBuffer (uint8_t* buffer, uint8_t* cnt);         // ��ȡ���յ�������
void  USART1_SendData (uint8_t* buf, uint8_t cnt);              // ͨ���жϷ������ݣ��ʺϸ�������
void  USART1_printf (char* stringTemp);                         // ͨ���жϷ����ַ������ʺ��ַ�����������256�������ڵ�
void  USART1_printfForDMA(char* stringTemp) ;                   // ͨ��DMA�������ݣ��ʺ�һ�ι������������ر����ַ�����ʡ��ռ���жϵ�ʱ��
// USART2
void  USART2_Init (uint32_t baudrate);                          // ��ʼ�����ڵ�GPIO��ͨ�Ų������á��ж����ȼ�; (�����ʿ��衢8λ���ݡ���У�顢1��ֹͣλ)
void  USART2_GetBuffer (uint8_t* buffer, uint8_t* cnt);         // ��ȡ���յ�������
void  USART2_SendData (uint8_t* buf, uint8_t cnt);              // ͨ���жϷ������ݣ��ʺϸ�������
void  USART2_printf (char* stringTemp);                         // ͨ���жϷ����ַ������ʺ��ַ�����������256�������ڵ�
// USART3
void  USART3_Init (uint32_t baudrate);                          // ��ʼ�����ڵ�GPIO��ͨ�Ų������á��ж����ȼ�; (�����ʿ��衢8λ���ݡ���У�顢1��ֹͣλ)
void  USART3_GetBuffer (uint8_t* buffer, uint8_t* cnt);         // ��ȡ���յ�������
void  USART3_SendData (uint8_t* buf, uint8_t cnt);              // ͨ���жϷ������ݣ��ʺϸ�������
void  USART3_printf (char* stringTemp);                         // ͨ���жϷ����ַ������ʺ��ַ�����������256�������ڵ�
// USART4
void  UART4_Init (uint32_t baudrate);                           // ��ʼ�����ڵ�GPIO��ͨ�Ų������á��ж����ȼ�; (�����ʿ��衢8λ���ݡ���У�顢1��ֹͣλ)
void  UART4_GetBuffer (uint8_t* buffer, uint8_t* cnt);          // ��ȡ���յ�������
void  UART4_SendData (uint8_t* buf, uint8_t cnt);               // ͨ���жϷ������ݣ��ʺϸ�������
void  UART4_printf (char* stringTemp);                          // ͨ���жϷ����ַ������ʺ��ַ�����������256�������ڵ�
// USART5
void  UART5_Init (uint32_t baudrate);                           // ��ʼ�����ڵ�GPIO��ͨ�Ų������á��ж����ȼ�; (�����ʿ��衢8λ���ݡ���У�顢1��ֹͣλ)
void  UART5_GetBuffer (uint8_t* buffer, uint8_t* cnt);          // ��ȡ���յ�������
void  UART5_SendData (uint8_t* buf, uint8_t cnt);               // ͨ���жϷ������ݣ��ʺϸ�������
void  UART5_printf (char* stringTemp);                          // ͨ���жϷ����ַ������ʺ��ַ�����������256�������ڵ�

#endif

