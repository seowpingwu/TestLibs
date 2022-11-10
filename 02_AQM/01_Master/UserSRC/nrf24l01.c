
#include "stm32f10x_conf.h"

xNRF24L01_TypeDef   xNRF24L01 ;

static char TempStr[100]; // 临时数据存放




unsigned char nFR20L01Buffer[16]  ; // Define a static TX address
unsigned char TX_ADDRESS[TX_ADR_WIDTH]  = {0x34,0x43,0x10,0x10,0x01}; // Define a static TX address

unsigned char  nRF24LO1_Init(void){
	
GPIO_InitTypeDef GPIO_InitStructure;
	
	  xNRF24L01 .InitOK        = 0;                                     // 初始化状态标志复位，０:失败， 1:成功     
    xNRF24L01 .StateIRQ      = 0;                                     // 中断状态，观察调试时使用
    
    xNRF24L01 .Hz            = NRF24L01_CONFIG_CH;                    // 频率，参数范围：<126, 实际使用频率：2400+x
    xNRF24L01 .crc           = NRF24L01_CONFIG_CRC;                   // CRC校验值(位)， 可选参数：8、16
    xNRF24L01 .radioBaudrate = NRF24L01_CONFIG_RF_DR;                 // 射频波特率： 0：1Mbps, 1:2Mbps, 4:250Kbps
    xNRF24L01 .radioPower    = NRF24L01_CONFIG_RF_PWR;                // 发射功率： 7:7dBm, 6:4dBm, 5:3dBm, 4:1dBm, 3:0dBm, 2:-4dBm, 1-6dBm, 0:-12dBm
    xNRF24L01 .dataLength    = NRF24L01_CONFIG_DATA_LENGTH;           // 每一包数据包含的字节数．最大值:32字节  
	  for(u8 i=0;i<5;i++)   xNRF24L01.targetAddress[i] = aNrfTxAdd[i];  // 发送到目标地址， 举例理解：对方的手机号码。地址宽度：5字节，40位
    for(u8 i=0;i<5;i++)   xNRF24L01.localAddress[i]  = aNrfRxAdd[i];  // 本地的接收地址， 举例理解：自己的手机号码。地址宽度：5字节，40位
	
	
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC ,ENABLE);
	

	
	GPIO_InitStructure.GPIO_Pin = nRF24L01_CE_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(nRF24L01_CE_PORT,&GPIO_InitStructure);

//MCU_INT1(PA1)--IQR

GPIO_SetBits(GPIOA,GPIO_Pin_1);			 //IRQ置高

	
	 if(CheckHardwareConnection()  == 1)   return 1;   
	 
	  xNRF24L01.InitOK = 1;
	  nRF24L01_RxMode();
	  return 0;
}



 void nRF24LO1_UserConfig(void){
	 
	
   MCU_SPI_UserConfig();
	 nRF24LO1_Init();
 }


/*********************************************************************************************************
*函  数：unsigned char nRF24L01_WriteReg(unsigned char Reg,unsigned char Dat)
*功  能：向指定寄存器地址，写一个字节数据
*参  数：reg： 寄存器地址
*       val:  要写入的值
*返回值：status
********************************************************************************************************/
 unsigned char nRF24L01_WriteReg(unsigned char Reg,unsigned char Dat){
	
	unsigned char Sta;
	SPI1_NSS_LOW;
	Sta=MCU_SPI_Read_Write(Reg);
	MCU_SPI_Read_Write(Dat);
	SPI1_NSS_High;
	return (Sta);
}

/******************************************************************************************************
*函  数：unsigned char nRF24L01_ReadReg(unsigned char Reg)
*功  能：向指定寄存器地址，读出一字节数据
*参  数：reg： 寄存器地址
*返回值：reg_val(第二个读取到的字节)
*******************************************************************************************************/
 unsigned char nRF24L01_ReadReg(unsigned char Reg){
	
	unsigned char RegDat;
	SPI1_NSS_LOW;
	MCU_SPI_Read_Write(Reg);
	RegDat=MCU_SPI_Read_Write(0xFF); //通过写入无效数据0或FF，将从设备上的数据挤出来
	SPI1_NSS_High;
	return (RegDat);
	
}
/******************************************************************************************************
*函  数：unsigned char nRF24L01_WirteBuffer(unsigned char Reg, unsigned char *pBuf, unsigned char Len)
*功  能：写一组数据到寄存器
*参  数：reg： 寄存器地址
*       pBuf： 要写入数据的地址
*        len:  要写入的数据长度
*返回值：status
*备  注：NRF2401代码移植只需把SPI驱动修改成自己的即可
*****************************************************************************************************/
 unsigned char nRF24L01_WirteBuffer(unsigned char Reg, unsigned char *pBuf, unsigned char Len){
	
	unsigned char Sta;
	SPI1_NSS_LOW;
	Sta=MCU_SPI_Read_Write(Reg);
	for(unsigned char i=0;i<Len;i++)
	{
		Sta=MCU_SPI_Read_Write(*pBuf++);
	}
	SPI1_NSS_High;
	return (Sta);
}
/*****************************************************************************************************
*函  数：unsigned char nRF24L01_ReadBuffer(unsigned char Reg, unsigned char *pBuf, unsigned char Len)
*功  能： 向指定寄存器地址，读出指定长度的数据
*参  数： reg :    寄存器地址
*        pBuf :   数据存放缓冲区
*        len  :   读取的字节数量
*返回值： status : 设备状态字
*******************************************************************************************************/
 unsigned char nRF24L01_ReadBuffer(unsigned char Reg, unsigned char *pBuf, unsigned char Len){

	unsigned char Sta;
	SPI1_NSS_LOW;
	Sta=MCU_SPI_Read_Write(Reg);
	for(unsigned char i=0;i<Len;i++)
	{
		pBuf[i]=MCU_SPI_Read_Write(0xFF);
	}
	SPI1_NSS_High;
	return (Sta);
}
/*****************************************************************************
 ** 函  数: NRF24L01_RxMode
 ** 功  能: 使NRF2401进入接收状态
 ** 
 ** 参  数：无 
 ** 返回值: 无  
*****************************************************************************/
void nRF24L01_RxMode(void){
	 
	if(xNRF24L01.InitOK == 0)
    {
        printf("\r无线通信设备：NRF24L01/SI24R1　不可用！\r");
        return;        
    }
		nRF24LO1_CE_LOW;
	
    nRF24L01_WirteBuffer(W_REGISTER + RX_ADDR_P0, (u8*)xNRF24L01 .localAddress, 5);    // 写入数据通道0接收地址， 地址宽度：5字节，40位
    nRF24L01_WriteReg(W_REGISTER + EN_AA,      0x01);                                  // 使能通道0自动应答 
    nRF24L01_WriteReg(W_REGISTER + EN_RXADDR,  0x01);                                  // 使能通道0的接收地址  
    nRF24L01_WriteReg(W_REGISTER + RF_CH,      NRF24L01_CONFIG_CH);                    // 射频通道，即频率(0-125)
    nRF24L01_WriteReg(W_REGISTER + RF_SETUP,   (NRF24L01_CONFIG_RF_DR<<3) | NRF24L01_CONFIG_RF_PWR  );   // 空中传输速率, 发射功率           
    nRF24L01_WriteReg(W_REGISTER + RX_PW_P0,   32);                                    // 设置通道0的有效数据包大小(字节数)，当接收到正确的数据包(正确的地址、CRC校验码，NRF2401自动把字头、地址和CRC校验位移去), STATUS寄存器的RX_DR置位
    nRF24L01_WriteReg(W_REGISTER + CONFIG, 0x03|( (NRF24L01_CONFIG_CRC==16?1:0) <<2)); // 接收模式、CRC16位、使能中断                 
    
    nRF24L01_WriteReg(FLUSH_RX, 0xff);                                                 // 清除RX_FIFO       
    nRF24L01_WriteReg(W_REGISTER+STATUS, 0X7E);                                        // 清除所有中断,防止一进去接收模式就触发中断  .
	
		nRF24LO1_CE_LOW;

	 printf("<<<NRF24L01 状态>>>   接收模式，信号监听中......\r");
}

#if 0
void nRF24L01_TxMode(void){
	
	nRF24LO1_CE_LOW;

	nRF24L01_WirteBuffer(W_REGISTER + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // Writes TX_Address to nRF24L01
	nRF24L01_WirteBuffer(W_REGISTER + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // RX_Addr0 same as TX_Adr for Auto.Ack
	nRF24L01_WirteBuffer(WR_TX_PLOAD, nFR20L01Buffer,TX_PLOAD_WIDTH); // Writes data to TX payload

	nRF24L01_WriteReg(W_REGISTER + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
	nRF24L01_WriteReg(W_REGISTER + EN_RXADDR, 0x01);  // Enable Pipe0
	nRF24L01_WriteReg(W_REGISTER + SETUP_RETR, 0x1a); // 500us + 86us, 10 retrans...
	nRF24L01_WriteReg(W_REGISTER + RF_CH, 40);        // Select RF channel 40
	nRF24L01_WriteReg(W_REGISTER + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
	nRF24L01_WriteReg(W_REGISTER + CONFIG, 0x0e);     // Set PWR_UP bit, enable CRC(2 bytes) & Prim:TX. MAX_RT & TX_DS enabled..

nRF24LO1_CE_LOW;

}
#endif
/*****************************************************************************
*函  数： u8 NRF24L01_TxData(u8 *txbuf)
*功  能： NRF发送一包数据（32字节)
*参  数： txbuf：要发送数据地址
*返回值： 无 
*重  要:  1：注意每次发送的是32个字节
         2: 用清理后的数组保存要发送的数据，再传给函数，尾部数据都为0；
         3: 如果函数直接发送字符串，上位机不会判断32个字节中哪些是有效数据，尾部很易有乱码。
            也可以修改函数，规定传送的字节数长度
            如何使用NRF24L01模块（U口)接收数据，注意有协议，第1字节为字节数
*****************************************************************************/
void nRF24L01_TxData(u8 *txbuf){
    if(xNRF24L01.InitOK == 0)
    {
        printf("\r无线通信设备：NRF24L01/SI24R1　不可用！\r");
        return;        
    } 
		
		nRF24LO1_CE_LOW;
		
	  nRF24L01_WriteReg(FLUSH_TX, 0XFF);                                  // 清除TX FIFO寄存器     
    nRF24L01_WriteReg(FLUSH_RX, 0xff);                                  // 清除RX_FIFO       
    nRF24L01_WriteReg(W_REGISTER+STATUS, 0X70);                         // 清除所有中断,防止一进去接收模式就触发中断      
       
    nRF24L01_WirteBuffer(W_TX_PAYLOAD, txbuf, 32);                      // 写数据到TX_BUFF    
    nRF24L01_WirteBuffer(W_REGISTER + TX_ADDR,    (u8*)xNRF24L01 .targetAddress,  5);  // 写入要发送的目标地址 
    nRF24L01_WirteBuffer(W_REGISTER + RX_ADDR_P0, (u8*)xNRF24L01 .targetAddress,  5);  // 通道0的地址设为和目标地址一致，以接收自动回复auto_ack信号 
    nRF24L01_WriteReg(W_REGISTER + EN_AA,      0x01);                   // 使能通道0自动应答 
    nRF24L01_WriteReg(W_REGISTER + EN_RXADDR,  0x01);                   // 使能通道0的接收地址  
    nRF24L01_WriteReg(W_REGISTER + SETUP_RETR, 0x1A);                   // 设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次 0x1A    
    nRF24L01_WriteReg(W_REGISTER + RF_CH,      NRF24L01_CONFIG_CH);     // 射频通道，即频率(0-125)
    nRF24L01_WriteReg(W_REGISTER + RF_SETUP,   (NRF24L01_CONFIG_RF_DR<<3) | NRF24L01_CONFIG_RF_PWR  ); // 空中传输速率, 发射功率           
    nRF24L01_WriteReg(W_REGISTER + CONFIG,     0x02|( (NRF24L01_CONFIG_CRC ==8?0:1) <<2));             // 设为发送模式,开启所有中断                                   
		
		nRF24L01_CE_High;                	

}
/*****************************************************************************
*函  数：u8 NRF24L01_RxData(u8 *rxbuf)
*功  能：NRF接收一包数据
*参  数：rxbuf：接收数据存储地址
*返回值：无
*备  注：无
*****************************************************************************/
void nRF24L01_RxData(u8 *rxbuf){
    
	if(xNRF24L01.InitOK == 0)
    {
        printf("\r无线通信设备：NRF24L01/SI24R1　不可用！\r");
        return;        
    }
	nRF24LO1_CE_LOW;
		
	nRF24L01_ReadBuffer(R_RX_PAYLOAD, rxbuf, RX_PAYLO_WIDTH); // 读取RX的有效数据
	nRF24L01_WriteReg(FLUSH_RX,0xff);                         // 清除RX FIFO(注意：这句话很必要)  
		
	nRF24L01_CE_High;  

}






 u8 CheckHardwareConnection(void)
{       
                                          // 重新配置spi, 避免其它设备使用过同一spi
    
    u8  i=0;                                          // 
    u16 errNum=0;                                     // 连接失败次数，防止死循环
    u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};             // 测试用发送地址
    
    printf("NRF24L01测试中... ");
    do{  
        __delay_us(2000);             
        nRF24L01_WirteBuffer(W_REGISTER+TX_ADDR, buf, 5);      // 写入5个字节的地址.    
        memset (buf,0,5);
        nRF24L01_ReadBuffer(TX_ADDR, buf, 5);                  // 读出写入的地址   
        for(i=0;i<5;i++){ 
            if(buf[i]!=0XA5) break;     
        }  
        errNum ++;
        if(errNum > 50)  break;                       // 防止出错时一直死循环
    }while(i<5);    
    
    if(errNum>50){                                    // 下面这一段不影响测试
        printf("    连接失败, 无线通信不可用!!!\r\n");       
        return 1;
    }
    else{        
        printf("    设备读写成功\r\n");
        return 0;
    }
}

void nRF24L01_ReadConfig(void){

		printf("\r>>>>读取NRF24L01配置：\r");
		printf("=寄存器名称=地址====值========备注========\r");
		u8 status=0;
				// 00 config
		status = nRF24L01_ReadReg(R_REGISTER + CONFIG );
		sprintf((char*)TempStr  ,"CONFIG      0x%02X = 0x%02X  %d位CRC校验  Power_UP=%d  Prim_RX=%d", CONFIG,status, ((status&(1<<2))==(1<<2))?16:8, ((status&(1<<1))==(1<<1))?1:0, ((status&1)==1)?1:0 );
		printf("%s\r\n",TempStr );  
		// 01 EN_AA
		status = nRF24L01_ReadReg(R_REGISTER + EN_AA);
		sprintf((char*)TempStr ,"EN_AA       0x%02X = 0x%02X  自动应答通道x使能", EN_AA, status );
		printf("%s\r\n",TempStr );    
		// 02 EN_RXADDR
		status = nRF24L01_ReadReg(R_REGISTER + EN_RXADDR);
		sprintf((char*)TempStr ,"EN_RXADDR   0x%02X = 0x%02X  接收通道x使能", EN_RXADDR, status );
		printf("%s\r\n",TempStr );    
		// 03 SETUP_AW
		status = nRF24L01_ReadReg(R_REGISTER + SETUP_AW);
		sprintf((char*)TempStr ,"SETUP_AW    0x%02X = 0x%02X  地址字节宽度", SETUP_AW, status );
		printf("%s\r\n",TempStr );
		// 04 SETUP_RETR
		status = nRF24L01_ReadReg(R_REGISTER + SETUP_RETR);
		sprintf((char*)TempStr ,"SETUP_RETR  0x%02X = 0x%02X  重发间隔：%dUS  重发次数：%d", SETUP_RETR, status, ((status>>4)+1)*250, status&0x0F );
		printf("%s\r\n",TempStr );    
		// 05 RF_CH        
		status = nRF24L01_ReadReg(R_REGISTER + RF_CH);
		sprintf((char*)TempStr ,"RF_CH       0x%02X = 0x%02X  频率：%d MHz",RF_CH,status, status );
		printf("%s\r\n",TempStr );         
		// 06 RF_SETUP
		status =nRF24L01_ReadReg(R_REGISTER + RF_SETUP);
		u8 rate= (status>>3)&0x05;
		char* sRate="000000000";
		char* sdBm="000000000";
		sRate = rate==4?"250Kbps":(rate==1?"2Mbps":"1Mpbs");
		rate= status&0x07;
		sdBm = rate==7?"7":(rate==6?"4":(rate==5?"3":(rate==4?"1":(rate==3?"0":(rate==2?"-4":(rate==1?"-6":"-12"))))));
		sprintf((char*)TempStr ,"RF_SETUP    0x%02X = 0x%02X  空中传输速率:%s  发射功率：%sdBm  ", RF_SETUP, status, sRate, sdBm );
		printf("%s\r\n",TempStr ); 
		// 07 STATUS
		status = nRF24L01_ReadReg(R_REGISTER + STATUS);
		sprintf((char*)TempStr ,"STATUS      0x%02X = 0x%02X  收到数据=%d  发送成功=%d  重发溢出=%d", STATUS, status, (status&(1<<6))?1:0, (status&(1<<5))?1:0, (status&(1<<4))?1:0);
		printf("%s\r\n",TempStr ); 
		// 0A RX_ADDR_P0
		nRF24L01_ReadBuffer (R_REGISTER +RX_ADDR_P0 ,(u8*)TempStr ,5);
		sprintf((char*)TempStr ,"RX_ADDR_P0  0x%02X         0x%02X  0x%02X  0x%02X  0x%02X  0x%02X", RX_ADDR_P0, TempStr[0],TempStr[1],TempStr[2],TempStr[3],TempStr[4] );
		printf("%s\r\n",TempStr );             
		// 10 TX_ADDR    
		nRF24L01_ReadBuffer (R_REGISTER +TX_ADDR ,(u8*)TempStr ,5);
		sprintf((char*)TempStr ,"TX_ADDR     0x%02X         0x%02X  0x%02X  0x%02X  0x%02X  0x%02X", TX_ADDR, TempStr[0],TempStr[1],TempStr[2],TempStr[3],TempStr[4] );
		printf("%s\r\n",TempStr );    
		// 11RX_PW_P0
		status = nRF24L01_ReadReg(R_REGISTER + RX_PW_P0);
		sprintf((char*)TempStr ,"RX_PW_P0    0x%02X = 0x%02X  接收数据长度：%d 字节", RX_PW_P0, status, status  );
		printf("%s\r\n",TempStr );    
		// 17 FIFO_STATUS
		status = nRF24L01_ReadReg(R_REGISTER + FIFO_STATUS);
		sprintf((char*)TempStr ,"FIFO_STATUS 0x%02X = 0x%02X", FIFO_STATUS, status );
		printf("%s\r\n",TempStr );


}

# if 0
/*****************************************************************************
 ** 函  数：NRF24L01_IRQ_IRQHANDLER
 ** 功  能：中断处理函数，
 **         三种中断可致IRQ引脚被拉低： 发送接收到ack，接收到数据，达到最大重发次数 
 ** 参  数：
 ** 返回值：
 ** 备  注: 
*****************************************************************************/
void nRF24L01_IRQ_IRQHANDLER(void)
{       
    static u8 status=0 ;       

    //printf("中断: ");    
    xNRF24L01 .StateIRQ =1;                                 // 中断动作记录，调试使用，0-没发过中断; 1-进入中断;  2-接收中断;  3-发送完成中断;  4最大重发次数中断;
    
    if((EXTI->PR & nRF24L01_READ_IRQ)){                      // 不知道为什么IRQ引脚有时能读到PR值，有时又读出为0
       
        nRF24LO1_CE_LOW;                                        // 拉低CE，以便读取NRF中STATUS中的数据    
       // spiInit ();                                         // 重新配置spi, 避免其它设备使用过同一spi
        status = nRF24L01_ReadReg(R_REGISTER + STATUS);              // 读取STATUS中的数据，以便判断是由什么中断源触发的IRQ中断
        //printf("NRF_status=0x%X  EXTI->PR=0x%X\r", status, EXTI->PR);

        // 接收完成中断 
        if(status & STATUS_RX){ 
            xNRF24L01 .StateIRQ = 2;                        // 中断动作记录，调试使用，0-没发过中断; 1-进入中断;  2-接收中断;  3-发送完成中断;  4最大重发次数中断;
            memset (xNRF24L01.RxData, 0, 32);      
     
            nRF24L01_ReadBuffer(R_RX_PAYLOAD, xNRF24L01.RxData, 32); // 读数据  RX_PAYLO_WIDTH
             nRF24L01_WriteReg(FLUSH_RX, 0xFF);                       // 清除RX FIFO(注意：这句话很必要)   
               
            xNRF24L01.Received=1;                           // 重要！！在外部不断轮询此标志值，判断是否收到数据；接收数据自动置１，外部函数处理完数据后手动置０
            printf("<<<NRF24L01 接收到数据>>>%s\r" , xNRF24L01.RxData);     // <<<测试时使用，删除不影响运行；不建议在中断里调用printf>>>   
        }
    
        // 发送完成中断 
        if(status & STATUS_TX){     
            xNRF24L01 .StateIRQ = 3;                        // 中断动作记录，调试使用，0-没发过中断; 1-进入中断;  2-接收中断;  3-发送完成中断;  4最大重发次数中断;
             nRF24L01_WriteReg(FLUSH_TX, 0xff);                       // 清发送缓冲区：TX_FIFO               
            printf("<<<NRF24L01接收到数据>>>  发送成功!\r");// <<<测试时使用，删除不影响运行；不建议在中断里调用printf>>>       
        }
    
        // 最大重发次数中断 
        if(status & STATUS_MAX){      
            xNRF24L01 .StateIRQ = 4;                        // 中断动作记录，调试使用，0-没发过中断; 1-进入中断;  2-接收中断;  3-发送完成中断;  4最大重发次数中断;  
             nRF24L01_WriteReg(FLUSH_TX, 0xFF);                       // 清除TX_FIFO             
            printf("<<<NRF24L01接收到数据>>>  发送失败，重发次数溢出!\r\r");  // <<<测试时使用，删除不影响运行；不建议在中断里调用printf>>>  
        }   
    
         nRF24L01_WriteReg(W_REGISTER+STATUS, 0x7E);                  // 清零NRF寄存器中的中断标志：发送完成 STATUS_TX
        nRF24L01_RxMode ();                                 // 中断完成后，自动进入接收状态
    }    
    EXTI->PR |= nRF24L01_READ_IRQ;                           // 清零引脚的中断标志位           
}
#endif
