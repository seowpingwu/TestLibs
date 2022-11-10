#include "stm32f10x_conf.h"

#define LM75A 0X90
#define ADDRESS LM75A
#define MODEL 1

unsigned char Temp[2];//MS/8BIT LS/3BIT

void LM75A_Write_Byte(u8 addr){ 
	
	MCU_I2C_Start();
	MCU_I2C_Write(ADDRESS|0);
	MCU_I2C_Wait_Ack();
	MCU_I2C_Write(addr); // 0x00 
	MCU_I2C_Wait_Ack();
	MCU_I2C_Stop();
	__delay_ms(100); //10ms
}
void LM75A_Read_Byte(u8 addr){ //Temp Read 0.125f 
	
  MCU_I2C_Start();
	MCU_I2C_Write(ADDRESS|0);
	MCU_I2C_Wait_Ack();
	MCU_I2C_Write(addr); 
	MCU_I2C_Wait_Ack();
	
  MCU_I2C_Start();
	MCU_I2C_Write(ADDRESS|1); //0.91 A0 A1 A2 GND
	MCU_I2C_Wait_Ack();
	
	for(u8 i=0;i<2;i++){
		
		Temp[i] = MCU_I2C_Read();
		
		if(i == 1)
			MCU_I2C_Send_Ack(1);
		else
			MCU_I2C_Send_Ack(0);
	}
	MCU_I2C_Stop();
}
void LM75A_Read_Temp(float *dat){
	
	int data_temp = 0;
	
	LM75A_Write_Byte(0x00);
	LM75A_Read_Byte(0x00);
	
#if MODEL 
	
	data_temp = (Temp[0]<<3)|(Temp[1]>>5);

#else

	data_temp = Temp[0];
	data_temp = data_temp <<8|Temp[1]; 
	data_temp = data_temp >> 5;//11 bit
	
#endif	
	
	if(data_temp & 0x4000){//¸ºÎÂ¶È									
		
		*dat = ~data_temp + 1 * -0.125f;
	}
	else{
		
		*dat = data_temp * 0.125f;
	}
}
