
#include "stm32f10x_conf.h"



xUSATR_TypeDef  xUSART;         // ����Ϊȫ�ֱ���,�����¼��Ϣ��״̬




//////////////////////////////////////////////////////////////   USART-1   //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************
 * ��  ���� vUSART1_Init
 * ��  �ܣ� ��ʼ��USART1��GPIO��ͨ�Ų������á��ж����ȼ� 
 *          (8λ���ݡ���У�顢1��ֹͣλ)
 * ��  ���� uint32_t baudrate  ͨ�Ų�����
 * ����ֵ�� ��
 ******************************************************************************/  
void USART1_Init(uint32_t baudrate)
{   
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;    
    
    // ʱ��ʹ��
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;                           // ʹ��USART1ʱ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;                             // ʹ��GPIOAʱ��

    // GPIO_TX��������
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;                // TX���ţ�����Ϊ�������칤��ģʽ
    GPIO_Init (GPIOA, &GPIO_InitStructure);
    // GPIO_RX��������
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING  ;        // RX���ţ����ڰ�����Ϊһ����ӵ�·����ѡ���ÿ�©ģʽ
    GPIO_Init (GPIOA, &GPIO_InitStructure);
    

    // �ж�����
    NVIC_InitStructure .NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure .NVIC_IRQChannelPreemptionPriority=2 ;       // ��ռ���ȼ�
    NVIC_InitStructure .NVIC_IRQChannelSubPriority = 2;                // �����ȼ�
    NVIC_InitStructure .NVIC_IRQChannelCmd = ENABLE;                // IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);                              
    
    //USART ��ʼ������
    USART_DeInit(USART1); 
    USART_InitStructure.USART_BaudRate   = baudrate;                // ���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;     // �ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;        // һ��ֹͣλ
    USART_InitStructure.USART_Parity     = USART_Parity_No;         // ����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // ʹ���ա���ģʽ
    USART_Init(USART1, &USART_InitStructure);                       // ��ʼ������
    
    USART_ITConfig(USART1, USART_IT_TXE , DISABLE );
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                  // ʹ�ܽ����ж�
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);                  // ʹ�ܿ����ж�
    
    USART_Cmd(USART1, ENABLE);                                      // ʹ�ܴ���, ��ʼ����  
   
    printf("\r\r\r======MSC΢�ǿ�ʵ���� USARTx �����ʼ����======\r");      
    printf("USART1��ʼ������      �����жϡ������ж�, �����ж�\r");    
}

/******************************************************************************
 * ��  ���� USART1_IRQHandler
 * ��  �ܣ� USART1�Ľ����жϡ������жϡ������ж�
 * ��  ���� ��
 * ����ֵ�� ��
 ******************************************************************************/
static uint8_t U1TxBuffer[256] ;    // �����жϷ��ͣ����λ�������256���ֽ�
static uint8_t U1TxCounter = 0 ;    // �����жϷ��ͣ�����ѷ��͵��ֽ���(����)
static uint8_t U1TxCount   = 0 ;    // �����жϷ��ͣ���ǽ�Ҫ���͵��ֽ���(����)

void USART1_IRQHandler(void)           
{     
    static uint8_t cnt=0;
    static uint8_t RxTemp[256];
    
    // �����ж�
    if(USART1->SR & (1<<5))
    {
        RxTemp[cnt++] = USART1->DR ;                        // ��ȡ���ݼĴ���ֵ��ע�⣺��ȡDRʱ�Զ������ж�λ��
    }
    
    // �����ж�, ������Ͻ����жϣ����ж�һ֡���ݽ������
    if(USART1->SR & (1<<4))                                 // ���IDLE�жϱ�־
    {         
        memcpy(xUSART.USART1RecivedBuffer , RxTemp , 256);  // ��ʱ����ת��Ϊȫ������, �ȴ�����ע�⣺���Ƶ����������飬����0ֵ���Է����ַ�������
        xUSART.USART1RecivedFlag = 1;                       // ��ǣ��ⲿ����ͨ�����xUSARTFlag.USART_2_Recived�Ƿ����1, ���ж��Ƿ�����һ֡����   
        xUSART.USART1RecivedCNT  = cnt;
        cnt=0;  
        memset(RxTemp ,0, 256);                             // ��ʱ���ݿ���,׼����һ�εĽ���           
        USART1 ->SR;  USART1 ->DR;                          // ����IDLE�жϱ�־λ!! �������㣬˳���ܴ�!!   

        /********************************************************************************
         ��ʽ1��������������ⲿ������������յ�����(���Ƽ�)
         ��ʽ2�������ⲿ�ж�USARTxRecivedFla==1��Ȼ����������xUSART.USARTxRecivedBuffer  
         �� ֹ���������ж������printf�Ȳ������뺯��!!!
         ʾ ��: ������һ�д���, ֻ��ʾ��, �������λ���Թ۲����յ�������һ֡����, ��ɾ��
        *********************************************************************************/
        //USART1_printf((char*)xUSART.USART1RecivedBuffer);  // ���д���ֻ��ʾ�������USART1�յ�������һ֡���ݣ���ɾ��
     }     

    // �����ж�
    if ((USART1->SR & 1<<7) && (USART1->CR1 & 1<<7))        // ����ж���Դ:TXE(�������ݼĴ�����)
    {                
        USART1->DR = U1TxBuffer[U1TxCounter++];             // ��ȡ���ݼĴ���ֵ��ע�⣺��ȡDRʱ�Զ������ж�λ��
        
        if(U1TxCounter == U1TxCount )
            USART1->CR1 &= ~(1<<7);                         // �ѷ�����ɣ��رշ��ͻ����������ж� TXEIE
    }    
}  

/******************************************************************************
 * ��  ���� vUSART1_GetBuffer
 * ��  �ܣ� ��ȡUART�����յ�������
 * ��  ���� uint8_t* buffer   ���ݴ�Ż����ַ
 *          uint8_t* cnt      ���յ����ֽ��� 
 * ����ֵ�� ��
 ******************************************************************************/  
void USART1_GetBuffer(uint8_t* buffer, uint8_t* cnt)
{    
    if(xUSART.USART1RecivedFlag == 0)    return; 
    
    memcpy(buffer, xUSART.USART1RecivedBuffer, xUSART.USART1RecivedCNT );
    *cnt = xUSART.USART1RecivedCNT;
    
    xUSART.USART1RecivedFlag =0;   
}

/******************************************************************************
 * ��  ���� vUSART1_SendData
 * ��  �ܣ� UARTͨ���жϷ�������,�ʺϸ�����������
 *         ���ʺϳ������������ɷ��͸������ݣ����������ַ�������int,char
 *         ���� �� �ϡ�ע�⻷�λ���������256�ֽڣ��������Ƶ��̫�ߣ�ע�Ⲩ����
 * ��  ���� uint8_t* buffer   �跢�����ݵ��׵�ַ
 *          uint8_t  cnt      ���͵��ֽ��� �������жϷ��͵Ļ�������С�����ܴ���256���ֽ�
 * ����ֵ��
 ******************************************************************************/  
void USART1_SendData(uint8_t* buf, uint8_t cnt)
{
    for(uint8_t i=0; i<cnt; i++) 
        U1TxBuffer[U1TxCount++] = buf[i];
     
    if((USART1->CR1 & 1<<7) == 0 )         // ��鷢�ͻ����������ж�(TXEIE)�Ƿ��Ѵ�
        USART1->CR1 |= 1<<7;             
}

/******************************************************************************
 * ��  ���� vUSART1_printf
 * ��  �ܣ� UARTͨ���жϷ�������ַ���,�����������ݳ���
 *         ���ʺϳ������ַ���������<=256�ֽ�
 *         ���� �� �ϡ�int,float����������
 * ��  ���� char* stringTemp   �跢�����ݵĻ����׵�ַ
 * ����ֵ�� Ԫ
 ******************************************************************************/  
void USART1_printf(char* stringTemp)
{
    u16 num=0;                                   // �ַ�������
    char* t=stringTemp ;                         // ������ϼ��㷢�͵�����    
    while(*t++ !=0)  num++;                      // ����Ҫ���͵���Ŀ���ⲽ�ȽϺ�ʱ�����Է���ÿ��6���ֽڣ�����1us����λ��8λ      
    USART1_SendData((u8*)stringTemp, num+1);     // ���ú�����ɷ��ͣ�num+1���ַ�����0��β����෢һ��:0   
}

/******************************************************************************
 * ��  ���� vUSART1_printfForDMA
 * ��  �ܣ� UARTͨ��DMA�������ݣ�ʡ��ռ���жϵ�ʱ��
 *         ���ʺϳ������ַ������ֽ����ǳ��࣬
 *         ���� �� �ϡ�1:ֻ�ʺϷ����ַ��������ʺϷ��Ϳ��ܺ�0����ֵ������; 2-ʱ����Ҫ�㹻
 * ��  ���� char strintTemp  Ҫ���͵��ַ����׵�ַ
 * ����ֵ�� ��
 ******************************************************************************/  
void USART1_printfForDMA(char* stringTemp) 
{
    static u8 Flag_DmaTxInit=0;                 // ���ڱ���Ƿ�������DMA����
    u32   num = 0;                              // ���͵�������ע�ⷢ�͵ĵ�λ���Ǳ���8λ��    
    char* t =stringTemp ;                       // ������ϼ��㷢�͵�����    
    
    while(*t++ !=0)  num++;                     // ����Ҫ���͵���Ŀ���ⲽ�ȽϺ�ʱ�����Է���ÿ��6���ֽڣ�����1us����λ��8λ           

    while(DMA1_Channel4->CNDTR > 0);            // ��Ҫ�����DMA���ڽ����ϴη��ͣ��͵ȴ�; �ý�����ж����־��F4������ô�鷳���������EN�Զ�����
    if( Flag_DmaTxInit == 0)                    // �Ƿ��ѽ��й�USAART_TX��DMA��������
    {   
        Flag_DmaTxInit  = 1;                    // ���ñ�ǣ��´ε��ñ������Ͳ��ٽ���������
        USART1 ->CR3   |= 1<<7;                 // ʹ��DMA����
        RCC->AHBENR    |= 1<<0;                    // ����DMA1ʱ��  [0]DMA1   [1]DMA2        
 
        DMA1_Channel4->CCR   = 0;               // ʧ�ܣ� ��0�����Ĵ���, DMA����ʧ�ܲ�������
        DMA1_Channel4->CNDTR = num;                // ����������   
        DMA1_Channel4->CMAR  = (u32)stringTemp; // �洢����ַ 
        DMA1_Channel4->CPAR  = (u32)&USART1->DR;// �����ַ      

        DMA1_Channel4->CCR |= 1<<4;              // ���ݴ��䷽��   0:�������   1:�Ӵ洢����
        DMA1_Channel4->CCR |= 0<<5;              // ѭ��ģʽ       0:��ѭ��     1��ѭ��
        DMA1_Channel4->CCR |= 0<<6;             // �����ַ������ģʽ
        DMA1_Channel4->CCR |= 1<<7;              // �洢������ģʽ
        DMA1_Channel4->CCR |= 0<<8;              // �������ݿ��Ϊ8λ
        DMA1_Channel4->CCR |= 0<<10;             // �洢�����ݿ��8λ
        DMA1_Channel4->CCR |= 0<<12;             // �е����ȼ�
        DMA1_Channel4->CCR |= 0<<14;             // �Ǵ洢�����洢��ģʽ    
    }    
    DMA1_Channel4->CCR  &= ~((u32)(1<<0));      // ʧ�ܣ�DMA����ʧ�ܲ�������
    DMA1_Channel4->CNDTR = num;                    // ����������
    DMA1_Channel4->CMAR  = (u32)stringTemp;     // �洢����ַ      
    DMA1_Channel4->CCR  |= 1<<0;                // ����DMA����   
} 






//////////////////////////////////////////////////////////////   USART-2   //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************
 * ��  ���� vUSART2_Init
 * ��  �ܣ� ��ʼ��USART��GPIO��ͨ�Ų������á��ж����ȼ� 
 *          (8λ���ݡ���У�顢1��ֹͣλ)
 * ��  ���� uint32_t baudrate  ͨ�Ų�����
 * ����ֵ�� ��
 ******************************************************************************/  
void USART2_Init(uint32_t baudrate)
{   
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;    
    
    // ʱ��ʹ��
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;                           // ʹ��USART1ʱ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;                             // ʹ��GPIOAʱ��

    // GPIO_TX��������
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;                // TX���ţ�����Ϊ�������칤��ģʽ
    GPIO_Init (GPIOA, &GPIO_InitStructure);
    // GPIO_RX��������
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING ;         // RX���ţ�����Ϊ�������빤��ģʽ
    GPIO_Init (GPIOA, &GPIO_InitStructure);
    
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // �ж�����
    NVIC_InitStructure .NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure .NVIC_IRQChannelPreemptionPriority=2 ;       // ��ռ���ȼ�
    NVIC_InitStructure .NVIC_IRQChannelSubPriority = 2;                // �����ȼ�
    NVIC_InitStructure .NVIC_IRQChannelCmd = ENABLE;                // IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);                              
    
    //USART ��ʼ������
    //USART_DeInit(USART2); 
    USART_InitStructure.USART_BaudRate   = baudrate;                // ���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;     // �ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;        // һ��ֹͣλ
    USART_InitStructure.USART_Parity     = USART_Parity_No;         // ����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    // ʹ���ա���ģʽ
    USART_Init(USART2, &USART_InitStructure);                       // ��ʼ������
    
    USART_ITConfig(USART2, USART_IT_TXE , DISABLE );
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);                  // ʹ�ܽ����ж�
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);                  // ʹ�ܿ����ж�
    
    USART_Cmd(USART2, ENABLE);                                      // ʹ�ܴ���, ��ʼ����  
      
    printf("\rUSART2��ʼ������      �����жϡ������ж�, �����ж�\r");    
}

/******************************************************************************
 * ��  ���� USART2_IRQHandler
 * ��  �ܣ� USART2�Ľ����жϡ������жϡ������ж�
 * ��  ���� ��
 * ����ֵ�� ��
 ******************************************************************************/
static uint8_t U2TxBuffer[256] ;    // �����жϷ��ͣ����λ�������256���ֽ�
static uint8_t U2TxCounter = 0 ;    // �����жϷ��ͣ�����ѷ��͵��ֽ���(����)
static uint8_t U2TxCount   = 0 ;    // �����жϷ��ͣ���ǽ�Ҫ���͵��ֽ���(����)

void USART2_IRQHandler(void)           
{     
    static uint8_t cnt=0;
    static uint8_t RxTemp[256];
    
    // �����ж�
    if(USART2->SR & (1<<5))
    {
        RxTemp[cnt++] = USART2->DR ;                        // ��ȡ���ݼĴ���ֵ��ע�⣺��ȡDRʱ�Զ������ж�λ��
    }
    
    // �����ж�, ������Ͻ����жϣ����ж�һ֡���ݽ������
    if(USART2->SR & (1<<4))                                 // ���IDLE�жϱ�־
    {         
        memcpy(xUSART.USART2RecivedBuffer , RxTemp , 256);  // ��ʱ����ת��Ϊȫ������, �ȴ�����ע�⣺���Ƶ����������飬����0ֵ���Է����ַ�������
        xUSART.USART2RecivedFlag = 1;                       // ��ǣ��ⲿ����ͨ�����xUSARTFlag.USART_2_Recived�Ƿ����1, ���ж��Ƿ�����һ֡����   
        xUSART.USART2RecivedCNT  = cnt;
        cnt=0;  
        memset(RxTemp ,0, 256);                             // ��ʱ���ݿ���,׼����һ�εĽ��� 
        USART2 ->SR;  USART2 ->DR;                          // ����IDLE�жϱ�־λ!! �������㣬˳���ܴ�!!  
        
        /********************************************************************************
         ��ʽ1��������������ⲿ������������յ�����(���Ƽ�)
         ��ʽ2�������ⲿ�ж�USARTxRecivedFla==1��Ȼ����������xUSART.USARTxRecivedBuffer  
         �� ֹ���������ж������printf�Ȳ������뺯��!!!
         ʾ ��: ������һ�д���, ֻ��ʾ��, �������λ���Թ۲����յ�������һ֡����, ��ɾ��
        *********************************************************************************/
        USART1_printf((char*)xUSART.USART2RecivedBuffer);  // ���д���ֻ��ʾ�������USART2�յ�������һ֡���ݣ���ɾ��   
     }     

    // �����ж�
    if ((USART2->SR & 1<<7) && (USART2->CR1 & 1<<7))        // ����ж���Դ:TXE(�������ݼĴ�����)
    {                
        USART2->DR = U2TxBuffer[U2TxCounter++];             // ��ȡ���ݼĴ���ֵ��ע�⣺��ȡDRʱ�Զ������ж�λ��
        
        if(U2TxCounter == U2TxCount )
            USART2->CR1 &= ~(1<<7);                         // �ѷ�����ɣ��رշ��ͻ����������ж� TXEIE
    }    
}  

/******************************************************************************
 * ��  ���� vUSART2_GetBuffer
 * ��  �ܣ� ��ȡUART�����յ�������
 * ��  ���� uint8_t* buffer   ���ݴ�Ż����ַ
 *          uint8_t* cnt      ���յ����ֽ��� 
 * ����ֵ�� ���յ����ֽ���
 ******************************************************************************/  
void USART2_GetBuffer(uint8_t* buffer, uint8_t* cnt)
{    
    if(xUSART.USART2RecivedFlag == 0)    return ; 
    
    memcpy(buffer, xUSART.USART2RecivedBuffer, xUSART.USART2RecivedCNT );
    *cnt = xUSART.USART2RecivedCNT;
    
    xUSART.USART2RecivedFlag =0;    
}

/******************************************************************************
 * ��  ���� vUSART2_SendData
 * ��  �ܣ� UARTͨ���жϷ�������,�ʺϸ�����������
 *         ���ʺϳ������������ɷ��͸������ݣ����������ַ�������int,char
 *         ���� �� �ϡ�ע�⻷�λ���������256�ֽڣ��������Ƶ��̫�ߣ�ע�Ⲩ����
 * ��  ���� uint8_t* buffer   �跢�����ݵ��׵�ַ
 *          uint8_t  cnt      ���͵��ֽ��� �������жϷ��͵Ļ�������С�����ܴ���256���ֽ�
 * ����ֵ��
 ******************************************************************************/  
void USART2_SendData(uint8_t* buf, uint8_t cnt)
{
    for(uint8_t i=0; i<cnt; i++) 
        U2TxBuffer[U2TxCount++] = buf[i];
     
    if((USART2->CR1 & 1<<7) == 0 )         // ��鷢�ͻ����������ж�(TXEIE)�Ƿ��Ѵ�
        USART2->CR1 |= 1<<7;             
}

/******************************************************************************
 * ��  ���� vUSART2_printf
 * ��  �ܣ� UARTͨ���жϷ�������ַ���,�����������ݳ���
 *         ���ʺϳ������ַ���������<=256�ֽ�
 *         ���� �� �ϡ�int,float����������
 * ��  ���� char* stringTemp   �跢�����ݵĻ����׵�ַ
 * ����ֵ�� Ԫ
 ******************************************************************************/  
void USART2_printf(char* stringTemp)
{
    u16 num=0;                                   // �ַ�������
    char* t=stringTemp ;                         // ������ϼ��㷢�͵�����    
    while(*t++ !=0)  num++;                      // ����Ҫ���͵���Ŀ���ⲽ�ȽϺ�ʱ�����Է���ÿ��6���ֽڣ�����1us����λ��8λ      
    USART2_SendData((u8*)stringTemp, num+1);     // ���ú�����ɷ��ͣ�num+1���ַ�����0��β����෢һ��:0   
}





//////////////////////////////////////////////////////////////   USART-3   //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************
 * ��  ���� vUSART3_Init
 * ��  �ܣ� ��ʼ��USART��GPIO��ͨ�Ų������á��ж����ȼ� 
 *          (8λ���ݡ���У�顢1��ֹͣλ)
 * ��  ���� uint32_t baudrate  ͨ�Ų�����
 * ����ֵ�� ��
 ******************************************************************************/  
void USART3_Init(uint32_t baudrate)
{   
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;    
    
    // ʱ��ʹ��
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;                           // ʹ��USART1ʱ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;                             // ʹ��GPIOAʱ��

    // GPIO_TX��������
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;                // TX���ţ�����Ϊ�������칤��ģʽ
    GPIO_Init (GPIOB, &GPIO_InitStructure);
    // GPIO_RX��������
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING ;         // RX���ţ�����Ϊ�������빤��ģʽ
    GPIO_Init (GPIOB, &GPIO_InitStructure);
    
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // �ж�����
    NVIC_InitStructure .NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure .NVIC_IRQChannelPreemptionPriority=2 ;       // ��ռ���ȼ�
    NVIC_InitStructure .NVIC_IRQChannelSubPriority = 2;             // �����ȼ�
    NVIC_InitStructure .NVIC_IRQChannelCmd = ENABLE;                // IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);                              
    
    //USART ��ʼ������
    USART_DeInit(USART3); 
    USART_InitStructure.USART_BaudRate   = baudrate;                // ���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;     // �ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;        // һ��ֹͣλ
    USART_InitStructure.USART_Parity     = USART_Parity_No;         // ����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // ʹ���ա���ģʽ
    USART_Init(USART3, &USART_InitStructure);                       // ��ʼ������
    
    USART_ITConfig(USART3, USART_IT_TXE , DISABLE );
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);                  // ʹ�ܽ����ж�
    USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);                  // ʹ�ܿ����ж�
    
    USART_Cmd(USART3, ENABLE);                                      // ʹ�ܴ���, ��ʼ����  
      
    printf("\rUSART3��ʼ������      �����жϡ������ж�, �����ж�\r");    
}

/******************************************************************************
 * ��  ���� USART3_IRQHandler
 * ��  �ܣ� USART�Ľ����жϡ������жϡ������ж�
 * ��  ���� ��
 * ����ֵ�� ��
 ******************************************************************************/
static uint8_t U3TxBuffer[256] ;    // �����жϷ��ͣ����λ�������256���ֽ�
static uint8_t U3TxCounter = 0 ;    // �����жϷ��ͣ�����ѷ��͵��ֽ���(����)
static uint8_t U3TxCount   = 0 ;    // �����жϷ��ͣ���ǽ�Ҫ���͵��ֽ���(����)

void USART3_IRQHandler(void)           
{     
    static uint8_t cnt=0;
    static uint8_t RxTemp[256];
    
    // �����ж�
    if(USART3->SR & (1<<5))
    {
        RxTemp[cnt++] = USART3->DR ;                        // ��ȡ���ݼĴ���ֵ��ע�⣺��ȡDRʱ�Զ������ж�λ��
    }
    
    // �����ж�, ������Ͻ����жϣ����ж�һ֡���ݽ������
    if(USART3->SR & (1<<4))                                 // ���IDLE�жϱ�־
    {         
        memcpy(xUSART.USART3RecivedBuffer , RxTemp , 256);  // ��ʱ����ת��Ϊȫ������, �ȴ�����ע�⣺���Ƶ����������飬����0ֵ���Է����ַ�������
        xUSART.USART3RecivedFlag = 1;                       // ��ǣ��ⲿ����ͨ�����xUSARTFlag.USART_2_Recived�Ƿ����1, ���ж��Ƿ�����һ֡����   
        xUSART.USART3RecivedCNT  = cnt;
        cnt=0;  
        memset(RxTemp ,0, 256);                             // ��ʱ���ݿ���,׼����һ�εĽ���   
        USART3 ->SR;  USART3 ->DR;                          // ����IDLE�жϱ�־λ!! �������㣬˳���ܴ�!! 
        
        /********************************************************************************
         ��ʽ1��������������ⲿ������������յ�����(���Ƽ�)
         ��ʽ2�������ⲿ�ж�USARTxRecivedFla==1��Ȼ����������xUSART.USARTxRecivedBuffer  
         �� ֹ���������ж������printf�Ȳ������뺯��!!!
         ʾ ��: ������һ�д���, ֻ��ʾ��, �������λ���Թ۲����յ�������һ֡����, ��ɾ��
        *********************************************************************************/
        USART1_printf((char*)xUSART.USART3RecivedBuffer);   // ���д���ֻ��ʾ�������USART3�յ�������һ֡���ݣ���ɾ��  
     }     

    // �����ж�
    if ((USART3->SR & 1<<7) && (USART3->CR1 & 1<<7))        // ����ж���Դ:TXE(�������ݼĴ�����)
    {                
        USART3->DR = U3TxBuffer[U3TxCounter++];             // ��ȡ���ݼĴ���ֵ��ע�⣺��ȡDRʱ�Զ������ж�λ��
        
        if(U3TxCounter == U3TxCount )
            USART3->CR1 &= ~(1<<7);                         // �ѷ�����ɣ��رշ��ͻ����������ж� TXEIE
    }    
}  

/******************************************************************************
 * ��  ���� vUSART3_GetBuffer
 * ��  �ܣ� ��ȡUART�����յ�������
 * ��  ���� uint8_t* buffer   ���ݴ�Ż����ַ
 *          uint8_t* cnt      ���յ����ֽ��� 
 * ����ֵ�� ���յ����ֽ���
 ******************************************************************************/  
void USART3_GetBuffer(uint8_t* buffer, uint8_t* cnt)
{    
    if(xUSART.USART3RecivedFlag == 0)    return ; 
    
    memcpy(buffer, xUSART.USART3RecivedBuffer, xUSART.USART3RecivedCNT );
    *cnt = xUSART.USART3RecivedCNT;
    
    xUSART.USART3RecivedFlag =0;    
}

/******************************************************************************
 * ��  ���� vUSART3_SendData
 * ��  �ܣ� UARTͨ���жϷ�������,�ʺϸ�����������
 *         ���ʺϳ������������ɷ��͸������ݣ����������ַ�������int,char
 *         ���� �� �ϡ�ע�⻷�λ���������256�ֽڣ��������Ƶ��̫�ߣ�ע�Ⲩ����
 * ��  ���� uint8_t* buffer   �跢�����ݵ��׵�ַ
 *          uint8_t  cnt      ���͵��ֽ��� �������жϷ��͵Ļ�������С�����ܴ���256���ֽ�
 * ����ֵ��
 ******************************************************************************/  
void USART3_SendData(uint8_t* buf, uint8_t cnt)
{
    for(uint8_t i=0; i<cnt; i++) 
        U3TxBuffer[U3TxCount++] = buf[i];
     
    if((USART3->CR1 & 1<<7) == 0 )         // ��鷢�ͻ����������ж�(TXEIE)�Ƿ��Ѵ�
        USART3->CR1 |= 1<<7;             
}

/******************************************************************************
 * ��  ���� vUSART3_printf
 * ��  �ܣ� UARTͨ���жϷ�������ַ���,�����������ݳ���
 *         ���ʺϳ������ַ���������<=256�ֽ�
 *         ���� �� �ϡ�int,float����������
 * ��  ���� char* stringTemp   �跢�����ݵĻ����׵�ַ
 * ����ֵ�� Ԫ
 ******************************************************************************/  
void USART3_printf(char* stringTemp)
{
    u16 num=0;                                   // �ַ�������
    char* t=stringTemp ;                         // ������ϼ��㷢�͵�����    
    while(*t++ !=0)  num++;                      // ����Ҫ���͵���Ŀ���ⲽ�ȽϺ�ʱ�����Է���ÿ��6���ֽڣ�����1us����λ��8λ      
    USART3_SendData((u8*)stringTemp, num+1);     // ���ú�����ɷ��ͣ�num+1���ַ�����0��β����෢һ��:0   
}




#ifdef STM32F10X_HD  // STM32F103R�������ϣ�����UART4��UART5

//////////////////////////////////////////////////////////////   UART-4   //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************
 * ��  ���� vUART4_Init
 * ��  �ܣ� ��ʼ��USART��GPIO��ͨ�Ų������á��ж����ȼ� 
 *          (8λ���ݡ���У�顢1��ֹͣλ)
 * ��  ���� uint32_t baudrate  ͨ�Ų�����
 * ����ֵ�� ��
 ******************************************************************************/  
void UART4_Init(uint32_t baudrate)
{   
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;    
    
    // ʱ��ʹ��
    RCC->APB1ENR |= RCC_APB1ENR_UART4EN;                            // ʹ��USARTʱ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;                             // ʹ��GPIOAʱ��

    // GPIO_TX��������
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;                // TX���ţ�����Ϊ�������칤��ģʽ
    GPIO_Init (GPIOC, &GPIO_InitStructure);
    // GPIO_RX��������
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING ;         // RX���ţ�����Ϊ�������빤��ģʽ
    GPIO_Init (GPIOC, &GPIO_InitStructure);
    
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // �ж�����
    NVIC_InitStructure .NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure .NVIC_IRQChannelPreemptionPriority=2 ;       // ��ռ���ȼ�
    NVIC_InitStructure .NVIC_IRQChannelSubPriority = 2;                // �����ȼ�
    NVIC_InitStructure .NVIC_IRQChannelCmd = ENABLE;                // IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);                              
    
    //USART ��ʼ������
    USART_DeInit(UART4); 
    USART_InitStructure.USART_BaudRate   = baudrate;                // ���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;     // �ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;        // һ��ֹͣλ
    USART_InitStructure.USART_Parity     = USART_Parity_No;         // ����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    // ʹ���ա���ģʽ
    USART_Init(UART4, &USART_InitStructure);                        // ��ʼ������
    
    USART_ITConfig(UART4, USART_IT_TXE , DISABLE );
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);                   // ʹ�ܽ����ж�
    USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);                   // ʹ�ܿ����ж�
    
    USART_Cmd(UART4, ENABLE);                                       // ʹ�ܴ���, ��ʼ����  
      
    printf("\rUART4 ��ʼ������      �����жϡ������ж�, �����ж�\r");    
}

/******************************************************************************
 * ��  ���� UART4_IRQHandler
 * ��  �ܣ� USART2�Ľ����жϡ������жϡ������ж�
 * ��  ���� ��
 * ����ֵ�� ��
 ******************************************************************************/
static uint8_t U4TxBuffer[256] ;    // �����жϷ��ͣ����λ�������256���ֽ�
static uint8_t U4TxCounter = 0 ;    // �����жϷ��ͣ�����ѷ��͵��ֽ���(����)
static uint8_t U4TxCount   = 0 ;    // �����жϷ��ͣ���ǽ�Ҫ���͵��ֽ���(����)

void UART4_IRQHandler(void)           
{     
    static uint8_t cnt=0;
    static uint8_t RxTemp[256];
    
    // �����ж�
    if(UART4->SR & (1<<5))
    {
        RxTemp[cnt++] = UART4->DR ;                         // ��ȡ���ݼĴ���ֵ��ע�⣺��ȡDRʱ�Զ������ж�λ��
    }
    
    // �����ж�, ������Ͻ����жϣ����ж�һ֡���ݽ������
    if(UART4->SR & (1<<4))                                  // ���IDLE�жϱ�־
    {         
        memcpy(xUSART.UART4RecivedBuffer , RxTemp , 256);   // ��ʱ����ת��Ϊȫ������, �ȴ�����ע�⣺���Ƶ����������飬����0ֵ���Է����ַ�������
        xUSART.UART4RecivedFlag = 1;                        // ��ǣ��ⲿ����ͨ�����xUSARTFlag.USART_2_Recived�Ƿ����1, ���ж��Ƿ�����һ֡����   
        xUSART.UART4RecivedCNT  = cnt;
        cnt=0;  
        memset(RxTemp ,0, 256);                             // ��ʱ���ݿ���,׼����һ�εĽ���   
        UART4 ->SR;  UART4 ->DR;                            // ����IDLE�жϱ�־λ!! �������㣬˳���ܴ�!! 
        
        /********************************************************************************
         ��ʽ1��������������ⲿ������������յ�����(���Ƽ�)
         ��ʽ2�������ⲿ�ж�USARTxRecivedFla==1��Ȼ����������xUSART.USARTxRecivedBuffer  
         �� ֹ���������ж������printf�Ȳ������뺯��!!!
         ʾ ��: ������һ�д���, ֻ��ʾ��, �������λ���Թ۲����յ�������һ֡����, ��ɾ��
        *********************************************************************************/
        USART1_printf((char*)xUSART.UART4RecivedBuffer);    // ���д���ֻ��ʾ�������UART4�յ�������һ֡���ݣ���ɾ��    
     }     

    // �����ж�
    if ((UART4->SR & 1<<7) && (UART4->CR1 & 1<<7))          // ����ж���Դ:TXE(�������ݼĴ�����)
    {                
        UART4->DR = U4TxBuffer[U4TxCounter++];              // ��ȡ���ݼĴ���ֵ��ע�⣺��ȡDRʱ�Զ������ж�λ��
        
        if(U4TxCounter == U4TxCount )
            UART4->CR1 &= ~(1<<7);                          // �ѷ�����ɣ��رշ��ͻ����������ж� TXEIE
    }    
}  

/******************************************************************************
 * ��  ���� vUART4_GetBuffer
 * ��  �ܣ� ��ȡUART�����յ�������
 * ��  ���� uint8_t* buffer   ���ݴ�Ż����ַ
 *          uint8_t* cnt      ���յ����ֽ��� 
 * ����ֵ�� ���յ����ֽ���
 ******************************************************************************/  
void UART4_GetBuffer(uint8_t* buffer, uint8_t* cnt)
{    
    if(xUSART.UART4RecivedFlag == 0)    return ; 
    
    memcpy(buffer, xUSART.UART4RecivedBuffer, xUSART.UART4RecivedCNT );
    *cnt = xUSART.UART4RecivedCNT;
    
    xUSART.UART4RecivedFlag =0;    
}

/******************************************************************************
 * ��  ���� vUART4_SendData
 * ��  �ܣ� UARTͨ���жϷ�������,�ʺϸ�����������
 *         ���ʺϳ������������ɷ��͸������ݣ����������ַ�������int,char
 *         ���� �� �ϡ�ע�⻷�λ���������256�ֽڣ��������Ƶ��̫�ߣ�ע�Ⲩ����
 * ��  ���� uint8_t* buffer   �跢�����ݵ��׵�ַ
 *          uint8_t  cnt      ���͵��ֽ��� �������жϷ��͵Ļ�������С�����ܴ���256���ֽ�
 * ����ֵ��
 ******************************************************************************/  
void UART4_SendData(uint8_t* buf, uint8_t cnt)
{
    for(uint8_t i=0; i<cnt; i++) 
        U4TxBuffer[U4TxCount++] = buf[i];
     
    if((UART4->CR1 & 1<<7) == 0 )         // ��鷢�ͻ����������ж�(TXEIE)�Ƿ��Ѵ�
        UART4->CR1 |= 1<<7;             
}

/******************************************************************************
 * ��  ���� vUART4_printf
 * ��  �ܣ� UARTͨ���жϷ�������ַ���,�����������ݳ���
 *         ���ʺϳ������ַ���������<=256�ֽ�
 *         ���� �� �ϡ�int,float����������
 * ��  ���� char* stringTemp   �跢�����ݵĻ����׵�ַ
 * ����ֵ�� Ԫ
 ******************************************************************************/  
void UART4_printf(char* stringTemp)
{
    u16 num=0;                                   // �ַ�������
    char* t = stringTemp ;                       // ������ϼ��㷢�͵�����    
    while(*t++ !=0)  num++;                      // ����Ҫ���͵���Ŀ���ⲽ�ȽϺ�ʱ�����Է���ÿ��6���ֽڣ�����1us����λ��8λ      
    UART4_SendData((u8*)stringTemp, num+1);      // ���ú�����ɷ��ͣ�num+1���ַ�����0��β����෢һ��:0   
}




//////////////////////////////////////////////////////////////   UART-4   //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************
 * ��  ���� vUART5_Init
 * ��  �ܣ� ��ʼ��USART��GPIO��ͨ�Ų������á��ж����ȼ� 
 *          (8λ���ݡ���У�顢1��ֹͣλ)
 * ��  ���� uint32_t baudrate  ͨ�Ų�����
 * ����ֵ�� ��
 ******************************************************************************/  
void UART5_Init(uint32_t baudrate)
{   
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;    
    
    // ʱ��ʹ��
    RCC->APB1ENR |= RCC_APB1ENR_UART5EN;                            // ʹ��USART1ʱ��
    RCC->APB2ENR |= RCC_APB2ENR_IOPDEN | RCC_APB2ENR_IOPCEN;        // ʹ��GPIOʱ��

    // GPIO_TX��������
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;                // TX���ţ�����Ϊ�������칤��ģʽ
    GPIO_Init (GPIOC, &GPIO_InitStructure);
    // GPIO_RX��������
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING ;         // RX���ţ�����Ϊ�������빤��ģʽ
    GPIO_Init (GPIOD, &GPIO_InitStructure);
    
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // �ж�����
    NVIC_InitStructure .NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure .NVIC_IRQChannelPreemptionPriority=2 ;       // ��ռ���ȼ�
    NVIC_InitStructure .NVIC_IRQChannelSubPriority = 2;                // �����ȼ�
    NVIC_InitStructure .NVIC_IRQChannelCmd = ENABLE;                // IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);                              
    
    //USART ��ʼ������
    USART_DeInit(UART5); 
    USART_InitStructure.USART_BaudRate   = baudrate;                // ���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;     // �ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;        // һ��ֹͣλ
    USART_InitStructure.USART_Parity     = USART_Parity_No;         // ����żУ��λ
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    // ʹ���ա���ģʽ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(UART5, &USART_InitStructure);                        // ��ʼ������
    
    USART_ITConfig(UART5, USART_IT_TXE , DISABLE );
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);                   // ʹ�ܽ����ж�
    USART_ITConfig(UART5, USART_IT_IDLE, ENABLE);                   // ʹ�ܿ����ж�
    
    USART_Cmd(UART5, ENABLE);                                       // ʹ�ܴ���, ��ʼ����  
      
    printf("\rUART5 ��ʼ������      �����жϡ������ж�, �����ж�\r");    
}

/******************************************************************************
 * ��  ���� UART5_IRQHandler
 * ��  �ܣ� USART2�Ľ����жϡ������жϡ������ж�
 * ��  ���� ��
 * ����ֵ�� ��
 ******************************************************************************/
static uint8_t U5TxBuffer[256] ;    // �����жϷ��ͣ����λ�������256���ֽ�
static uint8_t U5TxCounter = 0 ;    // �����жϷ��ͣ�����ѷ��͵��ֽ���(����)
static uint8_t U5TxCount   = 0 ;    // �����жϷ��ͣ���ǽ�Ҫ���͵��ֽ���(����)

void UART5_IRQHandler(void)           
{     
    static uint8_t cnt=0;
    static uint8_t RxTemp[256];
    
    // �����ж�
    if(UART5->SR & (1<<5))
    {
        RxTemp[cnt++] = UART5->DR ;                        // ��ȡ���ݼĴ���ֵ��ע�⣺��ȡDRʱ�Զ������ж�λ��
    }
    
    // �����ж�, ������Ͻ����жϣ����ж�һ֡���ݽ������
    if(UART5->SR & (1<<4))                                 // ���IDLE�жϱ�־
    {         
        memcpy(xUSART.UART5RecivedBuffer , RxTemp , 256);  // ��ʱ����ת��Ϊȫ������, �ȴ�����ע�⣺���Ƶ����������飬����0ֵ���Է����ַ�������
        xUSART.UART5RecivedFlag = 1;                       // ��ǣ��ⲿ����ͨ�����xUSARTFlag.USART_2_Recived�Ƿ����1, ���ж��Ƿ�����һ֡����   
        xUSART.UART5RecivedCNT  = cnt;
        cnt=0;  
        memset(RxTemp ,0, 256);                            // ��ʱ���ݿ���,׼����һ�εĽ���   
        UART5 ->SR;  UART5 ->DR;                           // ����IDLE�жϱ�־λ!! �������㣬˳���ܴ�!! 
        
        /********************************************************************************
         ��ʽ1��������������ⲿ������������յ�����(���Ƽ�)
         ��ʽ2�������ⲿ�ж�USARTxRecivedFla==1��Ȼ����������xUSART.USARTxRecivedBuffer  
         �� ֹ���������ж������printf�Ȳ������뺯��!!!
         ʾ ��: ������һ�д���, ֻ��ʾ��, �������λ���Թ۲����յ�������һ֡����, ��ɾ��
        *********************************************************************************/
        USART1_printf((char*)xUSART.UART5RecivedBuffer);  // ���д���ֻ��ʾ�������UART5�յ�������һ֡���ݣ���ɾ�� 
     }     

    // �����ж�
    if ((UART5->SR & 1<<7) && (UART5->CR1 & 1<<7))         // ����ж���Դ:TXE(�������ݼĴ�����)
    {                
        UART5->DR = U5TxBuffer[U5TxCounter++];             // ��ȡ���ݼĴ���ֵ��ע�⣺��ȡDRʱ�Զ������ж�λ��
        
        if(U5TxCounter == U5TxCount )
            UART5->CR1 &= ~(1<<7);                         // �ѷ�����ɣ��رշ��ͻ����������ж� TXEIE
    }    
}  

/******************************************************************************
 * ��  ���� vUART5_GetBuffer
 * ��  �ܣ� ��ȡUART�����յ�������
 * ��  ���� uint8_t* buffer   ���ݴ�Ż����ַ
 *          uint8_t* cnt      ���յ����ֽ��� 
 * ����ֵ�� ���յ����ֽ���
 ******************************************************************************/  
void UART5_GetBuffer(uint8_t* buffer, uint8_t* cnt)
{    
    if(xUSART.UART5RecivedFlag == 0)    return ; 
    
    memcpy(buffer, xUSART.UART5RecivedBuffer, xUSART.UART5RecivedCNT );
    *cnt = xUSART.UART5RecivedCNT;
    
    xUSART.UART5RecivedFlag =0;    
}

/******************************************************************************
 * ��  ���� vUART5_SendData
 * ��  �ܣ� UARTͨ���жϷ�������,�ʺϸ�����������
 *         ���ʺϳ������������ɷ��͸������ݣ����������ַ�������int,char
 *         ���� �� �ϡ�ע�⻷�λ���������256�ֽڣ��������Ƶ��̫�ߣ�ע�Ⲩ����
 * ��  ���� uint8_t* buffer   �跢�����ݵ��׵�ַ
 *          uint8_t  cnt      ���͵��ֽ��� �������жϷ��͵Ļ�������С�����ܴ���256���ֽ�
 * ����ֵ��
 ******************************************************************************/  
void UART5_SendData(uint8_t* buf, uint8_t cnt)
{
    for(uint8_t i=0; i<cnt; i++) 
        U5TxBuffer[U5TxCount++] = buf[i];
     
    if((UART5->CR1 & 1<<7) == 0 )         // ��鷢�ͻ����������ж�(TXEIE)�Ƿ��Ѵ�
        UART5->CR1 |= 1<<7;             
}

/******************************************************************************
 * ��  ���� vUART5_printf
 * ��  �ܣ� UARTͨ���жϷ�������ַ���,�����������ݳ���
 *         ���ʺϳ������ַ���������<=256�ֽ�
 *         ���� �� �ϡ�int,float����������
 * ��  ���� char* stringTemp   �跢�����ݵĻ����׵�ַ
 * ����ֵ�� Ԫ
 ******************************************************************************/  
void UART5_printf(char* stringTemp)
{
    u16 num=0;                                   // �ַ�������
    char* t=stringTemp ;                         // ������ϼ��㷢�͵�����    
    while(*t++ !=0)  num++;                      // ����Ҫ���͵���Ŀ���ⲽ�ȽϺ�ʱ�����Է���ÿ��6���ֽڣ�����1us����λ��8λ      
    UART5_SendData((u8*)stringTemp, num+1);      // ���ú�����ɷ��ͣ�num+1���ַ�����0��β����෢һ��:0   
}

#endif





//////////////////////////////////////////////////////////////  printf   //////////////////////////////////////////////////////////////
/******************************************************************************
 * ��  �ܣ� printf����֧�ִ���
 *         ���ر�ע�⡿�������´���, ʹ��printf����ʱ, ������Ҫѡ��use MicroLIB     
 * ��  ���� 
 * ����ֵ��
 * ��  ע�� MSCʵ����      ����޸�_2022��06��17��
 ******************************************************************************/  
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB     
#pragma import(__use_no_semihosting)                
struct __FILE       { int handle; };         // ��׼����Ҫ��֧�ֺ���
FILE __stdout;                               // FILE ��stdio.h�ļ�
void _sys_exit(int x) {    x = x; }             // ����_sys_exit()�Ա���ʹ�ð�����ģʽ

int fputc(int ch, FILE *f)                   // �ض���fputc������ʹprintf���������fputc�����UART,  ����ʹ�ô���1(USART1)
{ 
    #if 1                                    // ��ʽ1-ʹ�ó��õ�poll��ʽ�������ݣ��Ƚ�������⣬���ȴ���ʱ��  
        while((USARTx_DEBUG->SR & 0X40)==0); // �ȴ���һ�δ������ݷ������ 
        USARTx_DEBUG->DR = (u8) ch;          // дDR,����1���������� 
        return ch;
    #else                                    // ��ʽ2-ʹ��queue+�жϷ�ʽ��������; ������ʽ1�����ȴ���ʱ����Ҫ������д�õĺ��������λ���
        uint8_t c[1]={(uint8_t)ch};    
        if(USARTx_DEBUG == USART1)    vUSART1_SendData (c, 1);
        if(USARTx_DEBUG == USART2)    vUSART2_SendData (c, 1);
        if(USARTx_DEBUG == USART3)    vUSART3_SendData (c, 1);
        if(USARTx_DEBUG == UART4)     vUART4_SendData  (c, 1);
        if(USARTx_DEBUG == UART5)     vUART5_SendData  (c, 1);
        return ch;
    #endif    
}

