#include <stm32f10x_conf.h>

#include "bmp.h"


extern uint16_t CO2_Raw,TVCO_Raw;
extern float SHT3x_Temp,SHT3x_Hum;


u8 RF24L01RxBuffer[64]={0};


static char SHT3x_TempBuffer[6];////显示浮点数需要
static char SHT3x_HumBuffer[6];////显示浮点数需要



int main()
{	//u8 t=' ';
//uint8_t i;
	USART1_Init(115200);                     // 初始化USART1作为调试信息输出， 保存在magic.c文件中                
  Delay_Init(72); 
	SystemInit();
	
	#if 1
	
	MCU_I2C_UserConfig();
	
  SGP30_Init();
	SHT3x_Init();
	OLED_Init();
	
	OLED_ColorTurn(0);//0正常显示，1 反色显示
  OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
  
#endif
//*****************************
//初始化NRF24L01 
	
#if 1

	SysRuningstatusLED_Init();
	KEY_UserConfig();
	nRF24LO1_UserConfig();
  nRF24L01_ReadConfig();
#endif

	
//**********开机LOGO*********
	

OLED_ShowPicture(0,0,128,64,BMP1,1);
	__delay_ms(500);	
	OLED_Refresh();
	__delay_ms(500);	
OLED_Clear();
	
	while(1){
			
	//*******nRF24L01************************
#if 1
     LED_Runing_LOW;
		 __delay_ms(100);                 // 上面已初始化SysTick, 其后就可以直接使用ms和us的延时函数；
     LED_Runing_High;                // 红色LED 每0.5秒闪灭一次，以监察系统正常工作   
     __delay_ms(100);              
   #if 1
	 if(xNRF24L01.Received==1)
		{                                                             // 重要！！不断轮询此标志值，可判断是否收到新数据；
				printf("<<<NRF24L01接收到数据>>>  %s\r", xNRF24L01.RxData);
				if((xNRF24L01.RxData[0]=='A') && (xNRF24L01.RxData[1]=='1'))   LED_LED_NetWorkLinkErr_High;     //LED_BLUE_TOGGLE;    // 判断收到的数据；以第1字节作简单验证码，第2字节功能设定；
				if((xNRF24L01.RxData[0]=='A') && (xNRF24L01.RxData[1]=='2'))   LED_LED_SensorDataErr_LOW;               
				if((xNRF24L01.RxData[0]=='A') && (xNRF24L01.RxData[1]=='3'))   LED_LED_SensorDataErr_High;      
				xNRF24L01.Received = 0;                                                            // 重要！！数据处理完后，清零，避免下一个while循环又执行
      }  
#endif		
     //printf("<<<中断状态值xNRF24L01.StateIRQ>>>  %d\r", xNRF24L01.StateIRQ);              // 当收发不正常时，启用本条代码，以观察中断状态  
#endif
#if 1
     if(xUSART.USART1RecivedFlag ==1){                                                      // 使用上位机发送数据或命令，只用于测试收发函数的使用
			 nRF24L01_TxData((uint8_t *)xUSART.USART1RecivedBuffer );
			 xUSART.USART1RecivedFlag =0;                                                       // 重要！！当数据处理完了，必须清理标志位，不然下一个while又会执行
        }
#endif
				
		
		
//********************GXHT30**********************************************	
 #if 1

if(-1== SHT3x_Get_Dat()) 
	{
		     printf("--SHT3x Read Fail--\r\n");
	 }
else
	{
			// printf("--GXHT30 Read Success-- \r\n");
	  	 printf("T2 =%.1f  -->H2 = %.1f \r\n\r\n",SHT3x_Temp, SHT3x_Hum);
	}
#endif	
//********************SGP30**********************************************		
#if 1
if(-1== GSP30_Get_Dat()) 
	 {
		     printf("--SGP30 Read Fail--\r\n");
	 }

	 else 
	 {
		//	printf("--SGP30 Read Success-- \r\n");
		  printf("CO2_Raw:%d -->TVCO_Raw:%d\r\n\r\n", CO2_Raw,TVCO_Raw);
	 }
	 
#endif
//****************OLED Display***********************************************
 
		OLED_ShowChinese(24,0,12,16,0);//丹
		OLED_ShowChinese(40,0,13,16,0);//卿
		OLED_ShowChinese(56,0,14,16,0);//实
		OLED_ShowChinese(72,0,15,16,0);//验
		OLED_ShowChinese(88,0,16,16,0);//室
		
		
//void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode)
	 
	  OLED_ShowString(0,16,(u8*)"Tp:",16,1);  //3*8=24 +
	
	 	sprintf(SHT3x_TempBuffer,"%.1f",SHT3x_Temp);
	 	OLED_ShowString(24,24,(u8*)SHT3x_TempBuffer,8,1);//3*8=24
	  OLED_ShowString(48,16,(u8*)"C",8,1);  //3*8=24 +
	  
	 
    OLED_ShowString(68,16,(u8*)"Hm:",16,1);//3*8=24
		
	  sprintf(SHT3x_HumBuffer,"%.1f",SHT3x_Hum);
		OLED_ShowString(92,24,(u8*)SHT3x_HumBuffer,8,1);//24
	  OLED_ShowString(116,16,(u8*)"%",8,1);//3*8=24
		
		
		
	  OLED_ShowString(0,32,(u8*)"C2:",16,1);//96+16
		
		if(CO2_Raw>=1000)
			{
				OLED_ShowNum(24,40,CO2_Raw,4,8,1);
				OLED_ShowString(44,32,(u8*)"ppm",8,1);//24+8*3=48
			}
		else
			{
				OLED_ShowNum(24,40,CO2_Raw,3,8,1);
				OLED_ShowString(44,32,(u8*)"ppm",8,1);//24+8*3=48
			}
			
		OLED_ShowString(68,32,"Tc:",16,1);
		if(TVCO_Raw>=100)
		{
			OLED_ShowNum(92,40,TVCO_Raw,3,8,1);//64+8
			OLED_ShowString(108,32,(u8*)"ppb",8,1);//24+2*8=40/ 16+24 
		}
		else
		{
			OLED_ShowNum(92,40,TVCO_Raw,2,8,1);//64+8
			OLED_ShowString(108,32,(u8*)"ppb",8,1);//24+2*8=40/ 16+24 
		}
		
		OLED_ShowChinese(0,48,2,16,1);//终
		OLED_ShowChinese(16,48,3,16,1);//端
		OLED_ShowChinese(32,48,4,16,1);//连
		OLED_ShowChinese(48,48,5,16,1);//接
		OLED_ShowChinese(64,48,6,16,1);//数
		OLED_ShowString(80,48,(u8*)":1",16,1);
		
		
		OLED_Refresh();
		__delay_ms(200);	


	
	}

}
