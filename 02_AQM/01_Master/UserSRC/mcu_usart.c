#include <stm32f10x_conf.h>


void USARTX_UserConfig(u32 BaudRate){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USARTX_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = TXD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(USART_Prot,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = RXD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART_Prot,&GPIO_InitStructure);
	
	USART_DeInit(USART1);
	USARTX_InitStructure.USART_BaudRate = BaudRate;
	USARTX_InitStructure.USART_WordLength = USART_WordLength_8b;
	USARTX_InitStructure.USART_StopBits = USART_StopBits_1;
	USARTX_InitStructure.USART_Parity = USART_Parity_No;
	USARTX_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USARTX_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART1,&USARTX_InitStructure);
	USART_ClearFlag(USART1,USART_FLAG_RXNE);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0X01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0X00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void USART1_IRQHandler(void){
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET){
		
		USART_SendData(USART1,USART_ReceiveData(USART1));
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		
	  while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	}
}
int fputc(int data,FILE *BUF){

	USART_SendData(USART1,(u32) data);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	
	return data;
}
