#include <stm32f10x_conf.h>

static u8  us = 0; //0-255   //ȫ�־�̬�����洢us
static u16 ms = 0; //0-65535 //ȫ�־�̬�����洢ms

/************************************************************
 *���ߣ��°��ʵ����
 *�汾��v3.6.0
 *���ڣ�2022/1/4
 *������void Delay_Init(u8 MHZ);
 *���ã���ʱ������ʼ����������ʱ��Ƶ��
**************************************************************/
void Delay_Init(u8 Mhz){ 
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //����ʱ��8��Ƶ
	
	us = Mhz/8;                                           //F103ZET6оƬ������Ƶ72Mhz����8M��Ƶ = 9Mhz
	ms = (u16)us * 1000;                                  //ǿ��ת������(u16);ms��us�Ľ��ƻ������1000
}
/************************************************************
 *���ߣ��°��ʵ����
 *�汾��v3.6.0
 *���ڣ�2022/1/4
 *������void Delay_ms(u16 dly);
 *���ã��β�dly������Ҫ���õ�ʱ�����ʱ1.85��s
**************************************************************/
void __delay_ms(u16 dly){ //1800
	
	u32 temp; 							                    //�м����
	
	SysTick ->LOAD = (u32)ms*dly;								//��������ֵms*�βδ���ֵ���ʱ1.85s
	SysTick ->VAL  = RESET;                     //��յ�ǰֵ
	SysTick ->CTRL |= SysTick_CTRL_ENABLE_Msk;	//ʹ�ܵݼ�������/���������ѯģʽ�������ж�
	
	do{
		
		temp = SysTick ->CTRL;                    //��ѯ����״̬�Ƿ����
		
	}while((temp&0x01) &&! (temp&(1<<16))); 		//�ȴ�COUNTFLAG��־λ��һ��������
	
	SysTick ->VAL  = RESET;                     //��յ�ǰֵ
	SysTick ->CTRL &=~ SysTick_CTRL_ENABLE_Msk;	//ʧ�ܵݼ�������
}
/************************************************************
 *���ߣ��°��ʵ����
 *�汾��v3.6.0
 *���ڣ�2022/1/4
 *������void DelayMicroseconds(u32 dly);
 *���ã��β�dly������Ҫ���õ�ʱ�����ʱ1.85��s
**************************************************************/
void __delay_us(u32 dly){
	
	u32 temp;                                   //�м����
	
	SysTick ->LOAD = us*dly;										//��������ֵus*�βδ���ֵ���ʱ1.85s
	SysTick ->VAL  = 0; 										    //��յ�ǰֵ
	SysTick ->CTRL |= SysTick_CTRL_ENABLE_Msk;	//ʹ�ܵݼ�������/���������ѯģʽ�������ж�
	
	do{
		
		temp = SysTick ->CTRL;										//��ѯ����״̬�Ƿ����
		
	}while((temp&0x01) &&! (temp&(1<<16))); 		//�ȴ�COUNTFLAG��־λ��һ��������
	
	SysTick ->VAL  = RESET;										  //��յ�ǰֵ
	SysTick ->CTRL &=~ SysTick_CTRL_ENABLE_Msk; //ʧ�ܵݼ�������
}
