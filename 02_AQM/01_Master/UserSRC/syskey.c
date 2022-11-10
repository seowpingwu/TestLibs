#include "stm32f10x_conf.h"

void KEY_UserConfig(void){

	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = KEY1_Pin|KEY2_Pin|KEY3_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(SYS_KEY_GPIO,&GPIO_InitStructure);
  printf("按键 初始化           配置完成\r");  

}


