#include <stm32f10x_conf.h>


 void MCU_I2C_UserConfig(void)
 {
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin   = SDA|SCL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
 
 }


 
void I2C_SDA_Mode(unsigned char addr){
	
	GPIO_InitTypeDef GPIO_InitStructure;

	if(addr){ //1 OUT
		
		GPIO_InitStructure.GPIO_Pin = SDA;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	  GPIO_Init(GPIOB,&GPIO_InitStructure);	
	}
	else{     //0 INPUT
		
		GPIO_InitStructure.GPIO_Pin   = SDA;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
		GPIO_Init(GPIOB,&GPIO_InitStructure);
	}
} 
 void MCU_I2C_Start(void){
	
		I2C_SDA_Mode(OUT);
		
		SCL_High;
		SDA_High;
		__delay_us(5);
		
		SDA_Low;
		__delay_us(5);
		SCL_Low;
}

 void MCU_I2C_Stop(void){
	
		I2C_SDA_Mode(OUT);
			
		SCL_High;
		SDA_Low;
		__delay_us(5);
		SDA_High;
		__delay_us(5);
}
 

unsigned char MCU_I2C_Wait_Ack(void){
	
	unsigned char TimeAck=0;
		
		I2C_SDA_Mode(INPUT);
		
		SCL_High;
		__delay_us(4);
		
		while(GPIO_ReadInputDataBit(GPIOB,SDA)){
			
			if(++TimeAck > 250){
				
				MCU_I2C_Stop();
				printf("[033]从设备不在线，Sys Err!\r\n"); //没有应答 
							return 1;//非应答
			}
		}
		SCL_Low;                       //应答
		__delay_us(4);
		
		return 0;
	}

void MCU_I2C_Write(unsigned char  Data){
		
		SCL_Low;
			__delay_us(4);
		
		for(unsigned char  i=0;i<8;i++){
			
			I2C_SDA_Mode(OUT); // *
			
			if((Data<<i) & 0x80) 
					{SDA_High;} //H / L  1000 0000 
			else {SDA_Low;}
			
			SCL_High;
					__delay_us(4);
			SCL_Low;
					__delay_us(4);
		}
}
	

unsigned char  MCU_I2C_Read(void){
	
	unsigned char  Data=0;
		
		for(unsigned char  i=0;i<8;i++){
			
			I2C_SDA_Mode(INPUT); // *
			
			SCL_High;
					__delay_us(4);
			
			Data <<= 1; // 0000 0001 1000 0000 << 1
			
			if(GPIO_ReadInputDataBit(GPIOB,SDA) == 1){
				
				Data |= 0x01;
			}
			
			SCL_Low;
					__delay_ms(4);
		}
		
		return Data;
}


void MCU_I2C_Send_Ack(unsigned char ack){
	
		I2C_SDA_Mode(OUT);
		
		if(!ack)  
			{SDA_Low;}//应答“0”
		else
			{SDA_High;} //非应答“1”
		
		SCL_High;
					__delay_us(5);
		SCL_Low;
					__delay_us(5);
}
 
