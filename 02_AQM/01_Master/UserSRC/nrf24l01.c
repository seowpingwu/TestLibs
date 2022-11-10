
#include "stm32f10x_conf.h"

xNRF24L01_TypeDef   xNRF24L01 ;

static char TempStr[100]; // ��ʱ���ݴ��




unsigned char nFR20L01Buffer[16]  ; // Define a static TX address
unsigned char TX_ADDRESS[TX_ADR_WIDTH]  = {0x34,0x43,0x10,0x10,0x01}; // Define a static TX address

unsigned char  nRF24LO1_Init(void){
	
GPIO_InitTypeDef GPIO_InitStructure;
	
	  xNRF24L01 .InitOK        = 0;                                     // ��ʼ��״̬��־��λ����:ʧ�ܣ� 1:�ɹ�     
    xNRF24L01 .StateIRQ      = 0;                                     // �ж�״̬���۲����ʱʹ��
    
    xNRF24L01 .Hz            = NRF24L01_CONFIG_CH;                    // Ƶ�ʣ�������Χ��<126, ʵ��ʹ��Ƶ�ʣ�2400+x
    xNRF24L01 .crc           = NRF24L01_CONFIG_CRC;                   // CRCУ��ֵ(λ)�� ��ѡ������8��16
    xNRF24L01 .radioBaudrate = NRF24L01_CONFIG_RF_DR;                 // ��Ƶ�����ʣ� 0��1Mbps, 1:2Mbps, 4:250Kbps
    xNRF24L01 .radioPower    = NRF24L01_CONFIG_RF_PWR;                // ���书�ʣ� 7:7dBm, 6:4dBm, 5:3dBm, 4:1dBm, 3:0dBm, 2:-4dBm, 1-6dBm, 0:-12dBm
    xNRF24L01 .dataLength    = NRF24L01_CONFIG_DATA_LENGTH;           // ÿһ�����ݰ������ֽ��������ֵ:32�ֽ�  
	  for(u8 i=0;i<5;i++)   xNRF24L01.targetAddress[i] = aNrfTxAdd[i];  // ���͵�Ŀ���ַ�� ������⣺�Է����ֻ����롣��ַ��ȣ�5�ֽڣ�40λ
    for(u8 i=0;i<5;i++)   xNRF24L01.localAddress[i]  = aNrfRxAdd[i];  // ���صĽ��յ�ַ�� ������⣺�Լ����ֻ����롣��ַ��ȣ�5�ֽڣ�40λ
	
	
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC ,ENABLE);
	

	
	GPIO_InitStructure.GPIO_Pin = nRF24L01_CE_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(nRF24L01_CE_PORT,&GPIO_InitStructure);

//MCU_INT1(PA1)--IQR

GPIO_SetBits(GPIOA,GPIO_Pin_1);			 //IRQ�ø�

	
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
*��  ����unsigned char nRF24L01_WriteReg(unsigned char Reg,unsigned char Dat)
*��  �ܣ���ָ���Ĵ�����ַ��дһ���ֽ�����
*��  ����reg�� �Ĵ�����ַ
*       val:  Ҫд���ֵ
*����ֵ��status
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
*��  ����unsigned char nRF24L01_ReadReg(unsigned char Reg)
*��  �ܣ���ָ���Ĵ�����ַ������һ�ֽ�����
*��  ����reg�� �Ĵ�����ַ
*����ֵ��reg_val(�ڶ�����ȡ�����ֽ�)
*******************************************************************************************************/
 unsigned char nRF24L01_ReadReg(unsigned char Reg){
	
	unsigned char RegDat;
	SPI1_NSS_LOW;
	MCU_SPI_Read_Write(Reg);
	RegDat=MCU_SPI_Read_Write(0xFF); //ͨ��д����Ч����0��FF�������豸�ϵ����ݼ�����
	SPI1_NSS_High;
	return (RegDat);
	
}
/******************************************************************************************************
*��  ����unsigned char nRF24L01_WirteBuffer(unsigned char Reg, unsigned char *pBuf, unsigned char Len)
*��  �ܣ�дһ�����ݵ��Ĵ���
*��  ����reg�� �Ĵ�����ַ
*       pBuf�� Ҫд�����ݵĵ�ַ
*        len:  Ҫд������ݳ���
*����ֵ��status
*��  ע��NRF2401������ֲֻ���SPI�����޸ĳ��Լ��ļ���
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
*��  ����unsigned char nRF24L01_ReadBuffer(unsigned char Reg, unsigned char *pBuf, unsigned char Len)
*��  �ܣ� ��ָ���Ĵ�����ַ������ָ�����ȵ�����
*��  ���� reg :    �Ĵ�����ַ
*        pBuf :   ���ݴ�Ż�����
*        len  :   ��ȡ���ֽ�����
*����ֵ�� status : �豸״̬��
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
 ** ��  ��: NRF24L01_RxMode
 ** ��  ��: ʹNRF2401�������״̬
 ** 
 ** ��  ������ 
 ** ����ֵ: ��  
*****************************************************************************/
void nRF24L01_RxMode(void){
	 
	if(xNRF24L01.InitOK == 0)
    {
        printf("\r����ͨ���豸��NRF24L01/SI24R1�������ã�\r");
        return;        
    }
		nRF24LO1_CE_LOW;
	
    nRF24L01_WirteBuffer(W_REGISTER + RX_ADDR_P0, (u8*)xNRF24L01 .localAddress, 5);    // д������ͨ��0���յ�ַ�� ��ַ��ȣ�5�ֽڣ�40λ
    nRF24L01_WriteReg(W_REGISTER + EN_AA,      0x01);                                  // ʹ��ͨ��0�Զ�Ӧ�� 
    nRF24L01_WriteReg(W_REGISTER + EN_RXADDR,  0x01);                                  // ʹ��ͨ��0�Ľ��յ�ַ  
    nRF24L01_WriteReg(W_REGISTER + RF_CH,      NRF24L01_CONFIG_CH);                    // ��Ƶͨ������Ƶ��(0-125)
    nRF24L01_WriteReg(W_REGISTER + RF_SETUP,   (NRF24L01_CONFIG_RF_DR<<3) | NRF24L01_CONFIG_RF_PWR  );   // ���д�������, ���书��           
    nRF24L01_WriteReg(W_REGISTER + RX_PW_P0,   32);                                    // ����ͨ��0����Ч���ݰ���С(�ֽ���)�������յ���ȷ�����ݰ�(��ȷ�ĵ�ַ��CRCУ���룬NRF2401�Զ�����ͷ����ַ��CRCУ��λ��ȥ), STATUS�Ĵ�����RX_DR��λ
    nRF24L01_WriteReg(W_REGISTER + CONFIG, 0x03|( (NRF24L01_CONFIG_CRC==16?1:0) <<2)); // ����ģʽ��CRC16λ��ʹ���ж�                 
    
    nRF24L01_WriteReg(FLUSH_RX, 0xff);                                                 // ���RX_FIFO       
    nRF24L01_WriteReg(W_REGISTER+STATUS, 0X7E);                                        // ��������ж�,��ֹһ��ȥ����ģʽ�ʹ����ж�  .
	
		nRF24LO1_CE_LOW;

	 printf("<<<NRF24L01 ״̬>>>   ����ģʽ���źż�����......\r");
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
*��  ���� u8 NRF24L01_TxData(u8 *txbuf)
*��  �ܣ� NRF����һ�����ݣ�32�ֽ�)
*��  ���� txbuf��Ҫ�������ݵ�ַ
*����ֵ�� �� 
*��  Ҫ:  1��ע��ÿ�η��͵���32���ֽ�
         2: �����������鱣��Ҫ���͵����ݣ��ٴ���������β�����ݶ�Ϊ0��
         3: �������ֱ�ӷ����ַ�������λ�������ж�32���ֽ�����Щ����Ч���ݣ�β�����������롣
            Ҳ�����޸ĺ������涨���͵��ֽ�������
            ���ʹ��NRF24L01ģ�飨U��)�������ݣ�ע����Э�飬��1�ֽ�Ϊ�ֽ���
*****************************************************************************/
void nRF24L01_TxData(u8 *txbuf){
    if(xNRF24L01.InitOK == 0)
    {
        printf("\r����ͨ���豸��NRF24L01/SI24R1�������ã�\r");
        return;        
    } 
		
		nRF24LO1_CE_LOW;
		
	  nRF24L01_WriteReg(FLUSH_TX, 0XFF);                                  // ���TX FIFO�Ĵ���     
    nRF24L01_WriteReg(FLUSH_RX, 0xff);                                  // ���RX_FIFO       
    nRF24L01_WriteReg(W_REGISTER+STATUS, 0X70);                         // ��������ж�,��ֹһ��ȥ����ģʽ�ʹ����ж�      
       
    nRF24L01_WirteBuffer(W_TX_PAYLOAD, txbuf, 32);                      // д���ݵ�TX_BUFF    
    nRF24L01_WirteBuffer(W_REGISTER + TX_ADDR,    (u8*)xNRF24L01 .targetAddress,  5);  // д��Ҫ���͵�Ŀ���ַ 
    nRF24L01_WirteBuffer(W_REGISTER + RX_ADDR_P0, (u8*)xNRF24L01 .targetAddress,  5);  // ͨ��0�ĵ�ַ��Ϊ��Ŀ���ַһ�£��Խ����Զ��ظ�auto_ack�ź� 
    nRF24L01_WriteReg(W_REGISTER + EN_AA,      0x01);                   // ʹ��ͨ��0�Զ�Ӧ�� 
    nRF24L01_WriteReg(W_REGISTER + EN_RXADDR,  0x01);                   // ʹ��ͨ��0�Ľ��յ�ַ  
    nRF24L01_WriteReg(W_REGISTER + SETUP_RETR, 0x1A);                   // �����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10�� 0x1A    
    nRF24L01_WriteReg(W_REGISTER + RF_CH,      NRF24L01_CONFIG_CH);     // ��Ƶͨ������Ƶ��(0-125)
    nRF24L01_WriteReg(W_REGISTER + RF_SETUP,   (NRF24L01_CONFIG_RF_DR<<3) | NRF24L01_CONFIG_RF_PWR  ); // ���д�������, ���书��           
    nRF24L01_WriteReg(W_REGISTER + CONFIG,     0x02|( (NRF24L01_CONFIG_CRC ==8?0:1) <<2));             // ��Ϊ����ģʽ,���������ж�                                   
		
		nRF24L01_CE_High;                	

}
/*****************************************************************************
*��  ����u8 NRF24L01_RxData(u8 *rxbuf)
*��  �ܣ�NRF����һ������
*��  ����rxbuf���������ݴ洢��ַ
*����ֵ����
*��  ע����
*****************************************************************************/
void nRF24L01_RxData(u8 *rxbuf){
    
	if(xNRF24L01.InitOK == 0)
    {
        printf("\r����ͨ���豸��NRF24L01/SI24R1�������ã�\r");
        return;        
    }
	nRF24LO1_CE_LOW;
		
	nRF24L01_ReadBuffer(R_RX_PAYLOAD, rxbuf, RX_PAYLO_WIDTH); // ��ȡRX����Ч����
	nRF24L01_WriteReg(FLUSH_RX,0xff);                         // ���RX FIFO(ע�⣺��仰�ܱ�Ҫ)  
		
	nRF24L01_CE_High;  

}






 u8 CheckHardwareConnection(void)
{       
                                          // ��������spi, ���������豸ʹ�ù�ͬһspi
    
    u8  i=0;                                          // 
    u16 errNum=0;                                     // ����ʧ�ܴ�������ֹ��ѭ��
    u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};             // �����÷��͵�ַ
    
    printf("NRF24L01������... ");
    do{  
        __delay_us(2000);             
        nRF24L01_WirteBuffer(W_REGISTER+TX_ADDR, buf, 5);      // д��5���ֽڵĵ�ַ.    
        memset (buf,0,5);
        nRF24L01_ReadBuffer(TX_ADDR, buf, 5);                  // ����д��ĵ�ַ   
        for(i=0;i<5;i++){ 
            if(buf[i]!=0XA5) break;     
        }  
        errNum ++;
        if(errNum > 50)  break;                       // ��ֹ����ʱһֱ��ѭ��
    }while(i<5);    
    
    if(errNum>50){                                    // ������һ�β�Ӱ�����
        printf("    ����ʧ��, ����ͨ�Ų�����!!!\r\n");       
        return 1;
    }
    else{        
        printf("    �豸��д�ɹ�\r\n");
        return 0;
    }
}

void nRF24L01_ReadConfig(void){

		printf("\r>>>>��ȡNRF24L01���ã�\r");
		printf("=�Ĵ�������=��ַ====ֵ========��ע========\r");
		u8 status=0;
				// 00 config
		status = nRF24L01_ReadReg(R_REGISTER + CONFIG );
		sprintf((char*)TempStr  ,"CONFIG      0x%02X = 0x%02X  %dλCRCУ��  Power_UP=%d  Prim_RX=%d", CONFIG,status, ((status&(1<<2))==(1<<2))?16:8, ((status&(1<<1))==(1<<1))?1:0, ((status&1)==1)?1:0 );
		printf("%s\r\n",TempStr );  
		// 01 EN_AA
		status = nRF24L01_ReadReg(R_REGISTER + EN_AA);
		sprintf((char*)TempStr ,"EN_AA       0x%02X = 0x%02X  �Զ�Ӧ��ͨ��xʹ��", EN_AA, status );
		printf("%s\r\n",TempStr );    
		// 02 EN_RXADDR
		status = nRF24L01_ReadReg(R_REGISTER + EN_RXADDR);
		sprintf((char*)TempStr ,"EN_RXADDR   0x%02X = 0x%02X  ����ͨ��xʹ��", EN_RXADDR, status );
		printf("%s\r\n",TempStr );    
		// 03 SETUP_AW
		status = nRF24L01_ReadReg(R_REGISTER + SETUP_AW);
		sprintf((char*)TempStr ,"SETUP_AW    0x%02X = 0x%02X  ��ַ�ֽڿ��", SETUP_AW, status );
		printf("%s\r\n",TempStr );
		// 04 SETUP_RETR
		status = nRF24L01_ReadReg(R_REGISTER + SETUP_RETR);
		sprintf((char*)TempStr ,"SETUP_RETR  0x%02X = 0x%02X  �ط������%dUS  �ط�������%d", SETUP_RETR, status, ((status>>4)+1)*250, status&0x0F );
		printf("%s\r\n",TempStr );    
		// 05 RF_CH        
		status = nRF24L01_ReadReg(R_REGISTER + RF_CH);
		sprintf((char*)TempStr ,"RF_CH       0x%02X = 0x%02X  Ƶ�ʣ�%d MHz",RF_CH,status, status );
		printf("%s\r\n",TempStr );         
		// 06 RF_SETUP
		status =nRF24L01_ReadReg(R_REGISTER + RF_SETUP);
		u8 rate= (status>>3)&0x05;
		char* sRate="000000000";
		char* sdBm="000000000";
		sRate = rate==4?"250Kbps":(rate==1?"2Mbps":"1Mpbs");
		rate= status&0x07;
		sdBm = rate==7?"7":(rate==6?"4":(rate==5?"3":(rate==4?"1":(rate==3?"0":(rate==2?"-4":(rate==1?"-6":"-12"))))));
		sprintf((char*)TempStr ,"RF_SETUP    0x%02X = 0x%02X  ���д�������:%s  ���书�ʣ�%sdBm  ", RF_SETUP, status, sRate, sdBm );
		printf("%s\r\n",TempStr ); 
		// 07 STATUS
		status = nRF24L01_ReadReg(R_REGISTER + STATUS);
		sprintf((char*)TempStr ,"STATUS      0x%02X = 0x%02X  �յ�����=%d  ���ͳɹ�=%d  �ط����=%d", STATUS, status, (status&(1<<6))?1:0, (status&(1<<5))?1:0, (status&(1<<4))?1:0);
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
		sprintf((char*)TempStr ,"RX_PW_P0    0x%02X = 0x%02X  �������ݳ��ȣ�%d �ֽ�", RX_PW_P0, status, status  );
		printf("%s\r\n",TempStr );    
		// 17 FIFO_STATUS
		status = nRF24L01_ReadReg(R_REGISTER + FIFO_STATUS);
		sprintf((char*)TempStr ,"FIFO_STATUS 0x%02X = 0x%02X", FIFO_STATUS, status );
		printf("%s\r\n",TempStr );


}

# if 0
/*****************************************************************************
 ** ��  ����NRF24L01_IRQ_IRQHANDLER
 ** ��  �ܣ��жϴ�������
 **         �����жϿ���IRQ���ű����ͣ� ���ͽ��յ�ack�����յ����ݣ��ﵽ����ط����� 
 ** ��  ����
 ** ����ֵ��
 ** ��  ע: 
*****************************************************************************/
void nRF24L01_IRQ_IRQHANDLER(void)
{       
    static u8 status=0 ;       

    //printf("�ж�: ");    
    xNRF24L01 .StateIRQ =1;                                 // �ж϶�����¼������ʹ�ã�0-û�����ж�; 1-�����ж�;  2-�����ж�;  3-��������ж�;  4����ط������ж�;
    
    if((EXTI->PR & nRF24L01_READ_IRQ)){                      // ��֪��ΪʲôIRQ������ʱ�ܶ���PRֵ����ʱ�ֶ���Ϊ0
       
        nRF24LO1_CE_LOW;                                        // ����CE���Ա��ȡNRF��STATUS�е�����    
       // spiInit ();                                         // ��������spi, ���������豸ʹ�ù�ͬһspi
        status = nRF24L01_ReadReg(R_REGISTER + STATUS);              // ��ȡSTATUS�е����ݣ��Ա��ж�����ʲô�ж�Դ������IRQ�ж�
        //printf("NRF_status=0x%X  EXTI->PR=0x%X\r", status, EXTI->PR);

        // ��������ж� 
        if(status & STATUS_RX){ 
            xNRF24L01 .StateIRQ = 2;                        // �ж϶�����¼������ʹ�ã�0-û�����ж�; 1-�����ж�;  2-�����ж�;  3-��������ж�;  4����ط������ж�;
            memset (xNRF24L01.RxData, 0, 32);      
     
            nRF24L01_ReadBuffer(R_RX_PAYLOAD, xNRF24L01.RxData, 32); // ������  RX_PAYLO_WIDTH
             nRF24L01_WriteReg(FLUSH_RX, 0xFF);                       // ���RX FIFO(ע�⣺��仰�ܱ�Ҫ)   
               
            xNRF24L01.Received=1;                           // ��Ҫ�������ⲿ������ѯ�˱�־ֵ���ж��Ƿ��յ����ݣ����������Զ��ã����ⲿ�������������ݺ��ֶ��ã�
            printf("<<<NRF24L01 ���յ�����>>>%s\r" , xNRF24L01.RxData);     // <<<����ʱʹ�ã�ɾ����Ӱ�����У����������ж������printf>>>   
        }
    
        // ��������ж� 
        if(status & STATUS_TX){     
            xNRF24L01 .StateIRQ = 3;                        // �ж϶�����¼������ʹ�ã�0-û�����ж�; 1-�����ж�;  2-�����ж�;  3-��������ж�;  4����ط������ж�;
             nRF24L01_WriteReg(FLUSH_TX, 0xff);                       // �巢�ͻ�������TX_FIFO               
            printf("<<<NRF24L01���յ�����>>>  ���ͳɹ�!\r");// <<<����ʱʹ�ã�ɾ����Ӱ�����У����������ж������printf>>>       
        }
    
        // ����ط������ж� 
        if(status & STATUS_MAX){      
            xNRF24L01 .StateIRQ = 4;                        // �ж϶�����¼������ʹ�ã�0-û�����ж�; 1-�����ж�;  2-�����ж�;  3-��������ж�;  4����ط������ж�;  
             nRF24L01_WriteReg(FLUSH_TX, 0xFF);                       // ���TX_FIFO             
            printf("<<<NRF24L01���յ�����>>>  ����ʧ�ܣ��ط��������!\r\r");  // <<<����ʱʹ�ã�ɾ����Ӱ�����У����������ж������printf>>>  
        }   
    
         nRF24L01_WriteReg(W_REGISTER+STATUS, 0x7E);                  // ����NRF�Ĵ����е��жϱ�־��������� STATUS_TX
        nRF24L01_RxMode ();                                 // �ж���ɺ��Զ��������״̬
    }    
    EXTI->PR |= nRF24L01_READ_IRQ;                           // �������ŵ��жϱ�־λ           
}
#endif
