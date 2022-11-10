#ifndef __MCU_SPI_H_
#define __MCU_SPI_H_

#include <stm32f10x_conf.h>

//SPI1引脚定义


#define SPI1_SCK_PORT			GPIOA
#define SPI1_SCK_Pin		  GPIO_Pin_5		 //SCK (MCU PA5)

#define SPI1_MOSI_PORT		GPIOA
#define SPI1_MOSI_Pin			GPIO_Pin_7			//SDA	(MCU PA7)

#define SPI1_MISO_PORT		GPIOA
#define SPI1_MISO_Pin		  GPIO_Pin_6			 //Input Pin (MCU PA6)

#define SPI1_NSS_PORT			GPIOA             
#define SPI1_NSS_Pin		  GPIO_Pin_4		 //NESS (MCU PC4, soft CS(CSN) 





#define SPI1_SCK_LOW  		GPIO_ResetBits(SPI1_SCK_PORT,SPI1_SCK_Pin)  //SCK=0;
#define SPI1_SCK_High 		GPIO_SetBits(SPI1_SCK_PORT,SPI1_SCK_Pin)    //SCK=1;


#define SPI1_MOSI_LOW  		GPIO_ResetBits(SPI1_MOSI_PORT,SPI1_MOSI_Pin)  //SDA=0;
#define SPI1_MOSI_High 		GPIO_SetBits(SPI1_MOSI_PORT,SPI1_MOSI_Pin)    //SDA=1;

#define SPI1_MISO_State 	GPIO_ReadInputDataBit(SPI1_MISO_PORT,SPI1_MISO_Pin)  //读MISO输入电平

#define SPI1_NSS_LOW  		GPIO_ResetBits(SPI1_NSS_PORT,SPI1_NSS_Pin)  //CE=0;
#define SPI1_NSS_High 		GPIO_SetBits(SPI1_NSS_PORT,SPI1_NSS_Pin)    //CE=1;





void MCU_SPI_UserConfig(void);
unsigned char MCU_SPI_Read_Write(unsigned char  Dat);

#endif

