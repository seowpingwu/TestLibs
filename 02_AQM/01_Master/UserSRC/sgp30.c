#include <stm32f10x_conf.h>

uint16_t CO2_Raw;
uint16_t TVCO_Raw;
#define GSP30_DATA_FRAME_SIZE  6
uint8_t RawBuffer[GSP30_DATA_FRAME_SIZE];



typedef enum SGP30_CmdStr
	{
		SGP30_ADDR=0x58,
		SGP30_Feature_Set =0x0020,
		SGP30_Init_air_quality =0x2003,
		SGP30_Measure_air_quality =0x2008,
		SGP30_Get_baseline =0x2015,
		SGP30_Set_baseline =0x201e,
		SGP30_Set_humidity=0x2061,
		SGP30_Measure_test =0x2032,
		SGP30_Get_feature_set_version=0x202f,
		SGP30_Measure_raw_signals=0x2050,
	}SGP30_Cmd;






void SGP30_Init(void)
{
   MCU_I2C_Start();
   MCU_I2C_Write(SGP30_write);
   MCU_I2C_Wait_Ack();
   SGP30_Write(0x20, 0x03);
   MCU_I2C_Stop();
}



void SGP30_Write(unsigned char CmdMsb, unsigned char CmdLsb)
	{
				
		MCU_I2C_Start();
		MCU_I2C_Write(SGP30_ADDR<<1|0x00); //发送器件地址
		MCU_I2C_Wait_Ack();
		MCU_I2C_Write(CmdMsb);		//发送控制字节
		MCU_I2C_Wait_Ack();
		MCU_I2C_Write(CmdLsb);
		MCU_I2C_Wait_Ack();
		MCU_I2C_Stop();
			__delay_ms(1000);
			
	}

void GSP30_Multiple_Read_Byte(unsigned char len,uint8_t* GSP30RecBuffer )
{
	  unsigned char  i=0;
	
	  SGP30_Write(0x20,0x08);
		MCU_I2C_Start();
		MCU_I2C_Write(SGP30_ADDR<<1|0x01);
		MCU_I2C_Wait_Ack();
		for(i=0; i<len; i++)
		{
			GSP30RecBuffer[i]=MCU_I2C_Read(); 
			if(i==len-1)
			{    
					MCU_I2C_Send_Ack(1); //最后一个字节不应答
			}
			else
			{
					MCU_I2C_Send_Ack(0);
			}
		}
	MCU_I2C_Stop();	
			
}



uint8_t CheckCrc8(uint8_t * const message, uint8_t initial_value)
{
    uint8_t remainder;	    //余数
    uint8_t   i = 0, j = 0;  //循环变量

    /* 初始化 */
    remainder = initial_value;

    for(j = 0; j < 2;j++)
    {
        remainder ^= message[j];

        /* 从最高位开始依次计算  */
        for (i = 0; i < 8; i++)
        {
            if (remainder & 0x80)
            {
                remainder = (remainder << 1)^CRC8_POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }
    }

    /* 返回计算的CRC码 */
    return remainder;
}




int GSP30_Get_Dat(void)
{
   GSP30_Multiple_Read_Byte(GSP30_DATA_FRAME_SIZE,RawBuffer);
	
	
	///* 校验接收的测量数据 */
	  if (CheckCrc8(&RawBuffer[0], 0xFF) != RawBuffer[2]) 
			{
        printf("co2 recv data crc check fail\r\n");
        return -1;
      }
    if (CheckCrc8(&RawBuffer[3], 0xFF) != RawBuffer[5]) 
		{
        printf("tvoc recv data crc check fail\r\n");
        return -1;
    }
		
		////* 转换测量数据 */
   CO2_Raw=RawBuffer[0]<<8|RawBuffer[1];
   TVCO_Raw=RawBuffer[3]<<8|RawBuffer[4];
		
	return 0;
}


