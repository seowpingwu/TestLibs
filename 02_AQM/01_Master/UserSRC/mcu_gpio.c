#include <stm32f10x_conf.h>

void GPIO_Userconfig(void){

	//***system sataus leds  config**/
	
	GPIO_InitTypeDef GPIO_InitStructure; //���ýṹ�����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //ʹ��APB2����ʱ��
	GPIO_InitStructure.GPIO_Pin = SysRuningStaLed;//ѡ������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  //��������߿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //��������ʽ���ģʽ
	GPIO_Init(GPIOA,&GPIO_InitStructure);             //��ʼ��ָ������
	GPIO_SetBits(GPIOA,SysRuningStaLed);              //ָ����������ߵ�ƽ
	
}

