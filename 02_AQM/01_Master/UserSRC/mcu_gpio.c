#include <stm32f10x_conf.h>

void GPIO_Userconfig(void){

	//***system sataus leds  config**/
	
	GPIO_InitTypeDef GPIO_InitStructure; //设置结构体变量
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //使能APB2外设时钟
	GPIO_InitStructure.GPIO_Pin = SysRuningStaLed;//选择引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  //设置输出线口速率
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //设置推挽式输出模式
	GPIO_Init(GPIOA,&GPIO_InitStructure);             //初始化指定分组
	GPIO_SetBits(GPIOA,SysRuningStaLed);              //指定引脚输出高电平
	
}

