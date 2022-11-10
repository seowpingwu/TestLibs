#include "stm32f10x_conf.h"

void MCU_SPI_UserConfig(void){

  GPIO_InitTypeDef GPIO_InitStructure;
	
/*********** SPI GPIO pin configuration************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);
	
//SPI1_SCK 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = SPI1_SCK_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_SCK_PORT,&GPIO_InitStructure);
	
//SPI1_MOSI 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = SPI1_MOSI_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_MOSI_PORT,&GPIO_InitStructure);	
	
//SPI1_MISO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //输入
	GPIO_InitStructure.GPIO_Pin = SPI1_MISO_Pin;
	GPIO_Init(SPI1_MISO_PORT,&GPIO_InitStructure);

//SPI1_NSS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin =  SPI1_NSS_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_NSS_PORT,&GPIO_InitStructure); 


}
//202206-18调通，
 unsigned char MCU_SPI_Read_Write(unsigned char  Dat){
#if 1 
	 SPI1_SCK_LOW;
 for (unsigned char  i=0; i<8; i++)//8bit，一位一位发送
	 {
		 if ((Dat&0x80)==0x80)				//逻辑与，判断当前最高位为1还是0，0x80为10000000
			{
				SPI1_MOSI_High;   	 //待将数据1发送出去		
			}
		else
			{
				SPI1_MOSI_LOW ;   	 //待将数据0发送出去
			}
			SPI1_SCK_High;         //相当重要！！！补充202206-18
			Dat<<=1;
			if(SPI1_MISO_State)
			{
				Dat |=0x01;
			}
			
			SPI1_SCK_LOW;         //相当重要！！！
		}
	 
	 return (Dat);
		
#endif
#if 0
	 //SPI1_SCK_LOW;
 for (unsigned char  i=0; i<8; i++)//8bit，一位一位发送
	 {
		 if (Dat&0x80)				//逻辑与，判断当前最高位为1还是0，0x80为10000000
			{
				SPI1_MOSI_High;   	 //待将数据1发送出去		
			}
		else
			{
				SPI1_MOSI_LOW ;   	 //待将数据0发送出去
			}
			SPI1_SCK_High;       //相当重要！！！
			Dat=Dat<<1;
			if(SPI1_MISO_State)
			{
				Dat |=0x01;
			}
			SPI1_SCK_LOW;     //相当重要！！！
		}
	 
	 return (Dat);
		
#endif
 
 }
