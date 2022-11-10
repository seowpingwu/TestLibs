
#include "stm32f10x_conf.h"
//******SHT3x Addr NOTE **************
//***ADDR--to--VSS:0X44
//***ADDR--to--VDD:0x45 



#define SHT3x_ERROR                             0x01
#define SHT3x_NO_ERROR                          0x00



#define SHT3x_ADDR  0x44
#define SHT3x_Write 0x88  //0100 0100 <<1 |0
#define SHT3x_Read  0x89  //0100 0100 <<1 |1

#define SHT3x_Tepm_Cmd     0xe3
#define SHT3x_RelHum_Cmd   0xe5

float SHT3x_Temp,SHT3x_Hum;




void SHT3x_Init()
{
    MCU_I2C_Start();
    MCU_I2C_Write(SHT3x_ADDR<<1|0x00);
    MCU_I2C_Wait_Ack();
    SHT3x_Write_Byte(0x2c,0x0d);  //1s重复1次
      __delay_ms(500);  

}

void SHT3x_Write_Byte(unsigned char CmdMSB,unsigned char CmdLSB)
{
    
    MCU_I2C_Start();
    MCU_I2C_Write(SHT3x_ADDR<<1|0x00);
    MCU_I2C_Wait_Ack();
    MCU_I2C_Write(CmdMSB);
    MCU_I2C_Wait_Ack();
    MCU_I2C_Write(CmdLSB);
    MCU_I2C_Wait_Ack();
    MCU_I2C_Stop();
    __delay_ms(500);  

}

//*******************************************************************
int SHT3x_Get_Dat(){
	
	uint16_t rawTemp, rawRH;
	uint8_t  buffer[6];
	SHT3x_Write_Byte(0x2c,0x06);
	MCU_I2C_Start();
	MCU_I2C_Write(SHT3x_ADDR<<1|0x01); 
	MCU_I2C_Wait_Ack();
	 for (uint8_t i=0; i<6; i++)
  	{
			buffer[i] =MCU_I2C_Read();
			if(i==5)
				{    
					MCU_I2C_Send_Ack(1); //最后一个字节不应答
				}
			else
				{
						MCU_I2C_Send_Ack(0);
				}
			}
	MCU_I2C_Stop();	
	


		
	rawTemp = buffer[0];
	rawTemp <<= 8;
	rawTemp |= buffer[1];
		

		if ((buffer[2]!= CRC8(buffer,2))||(buffer[5] != CRC8(buffer+0x03, 2)))
		{
				return -1;
		}
    rawRH = buffer[3];
    rawRH <<= 8;
    rawRH |= buffer[4];	
		
	  float temp = rawTemp;
    temp = -45.0 + (175.0 * rawTemp / 65535.0)-1.20;//修正-1.20(DIY 20220704)
    SHT3x_Temp = temp;
    
    float hum = rawRH;
    hum = 100.0 * (rawRH / 65535.0)+0.0;//修正+0.0%(DIY 20220704)
   SHT3x_Hum = hum;
    
    return 1;	
		
}

//  TempValue=(175.0*(float)GXHT30Buffer[0]/65535.0 -45.0 );
//  HumValue =(100*(float)GXHT30Buffer[1]/65535.0);

uint8_t CRC8(const uint8_t *idata, int len)
{
    /*
        Polynomial: 0x31 (x8 + x5 + x4 + 1)
        Initialization: 0xFF
        Final XOR: 0x00
        Example: CRC (0xBEEF) = 0x92
    */
 uint8_t bt;        // bit mask
 uint8_t crc = 0xFF; // calculated checksum
 uint8_t byteCtr;    // byte counter
	for(byteCtr = 0; byteCtr <len; byteCtr++) {
     crc ^= (idata[byteCtr]);
 	//crc校验，最高位是1就^0x31
     for(bt = 8; bt > 0; --bt) {
         if(crc & 0x80) 
					 {
             crc = (crc << 1) ^ 0x31;
            } 
					 else 
						 {
             crc = (crc << 1);
            }
     }
 }
 return crc;
}

//*******************************************************************

unsigned char SHT3x_Calculate_Crc8(unsigned char *value) 
{
    signed char i = 0x00;
    signed char j = 0x00;
    unsigned char crc_value = 0xFF;

    for(i = 0x00; i < 0x02; i++)
    {
        crc_value ^= value[i];

        for(j = 0x08; j > 0x00; --j)
        {
           if(crc_value & 0x80)
           {
               crc_value = ((crc_value << 1) ^ 0x131);
           }
           else
           {
               crc_value <<= 1;
           }
        }
    }

    return crc_value;
}


unsigned char SHT3x_Read_Temp_Humidity(float *RH_value, float *T_value)
{
    unsigned char buffer[6];

    signed long long t_tmp = 0x000000000;
    signed long long h_tmp = 0x000000000;
		
		SHT3x_Write_Byte(0x2c,0x06);
		MCU_I2C_Start();
		MCU_I2C_Write(SHT3x_ADDR<<1|0x01); 
		MCU_I2C_Wait_Ack();
		for (uint8_t i=0; i<6; i++)
			{
					buffer[i] =MCU_I2C_Read();
				if(i==5)
					{    
							MCU_I2C_Send_Ack(1); //最后一个字节不应答
					}
				else
					{
							MCU_I2C_Send_Ack(0);
					}
			}
		MCU_I2C_Stop();	
		
	if((buffer[2] != SHT3x_Calculate_Crc8(buffer)) || (buffer[5] != SHT3x_Calculate_Crc8((buffer + 0x03))))
		{
				return SHT3x_ERROR;
		}

		t_tmp = Make_Word(buffer[0],buffer[1]);
		t_tmp = (((4375 * t_tmp) >> 14) - 4500.0);
		*T_value = (((float)t_tmp) / 100.0);

		h_tmp = Make_Word(buffer[3],buffer[4]);
		h_tmp = ((625 * h_tmp) >> 12);
		*RH_value = (((float)h_tmp) / 100.0);
		
		return SHT3x_NO_ERROR;
}


unsigned long Make_Word(unsigned char hb, unsigned char lb)
{
    unsigned long value = 0x0000;

    value = hb;
    value <<= 0x08;
    value |= lb;

    return value;
}

