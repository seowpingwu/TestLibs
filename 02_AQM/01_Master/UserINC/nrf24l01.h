#ifndef __NRF24L01_H_
#define __NRF24L01_H_

#include "stm32f10x_conf.h"

//******nRF24L01 defeine ***********************

#define nRF24L01_CE_PORT		GPIOC
#define nRF24L01_CE_Pin		  GPIO_Pin_4		 //CE nRF24L01����ģʽ�������ߵ�ƽ��Ч������/����ģʽ����,CEЭͬCONFIG�Ĵ�����ͬ����NRFL2401��״̬

#define nRF24L01_IRQ_PORT		GPIOA
#define nRF24L01_IRQ_Pin  	GPIO_Pin_1  //(MCU PA1 EXTI1_IRQn ) IRQ �ж��źţ���ʱ�ߵ�ƽ���ж�ʱ�͵�ƽ, �͵�ƽ��Ч���ж����
#define NRF24L01_IRQ				EXTI15_10_IRQn//


/** ͨ�ſ������ż� *******************/
//CE
#define nRF24LO1_CE_LOW  			GPIO_ResetBits(nRF24L01_CE_PORT,nRF24L01_CE_Pin)
#define nRF24L01_CE_High 			GPIO_SetBits(nRF24L01_CE_PORT,nRF24L01_CE_Pin)

//IRQ�ж�
#define nRF24L01_IRQ_LOW      GPIO_ResetBits(nRF24L01_IRQ_PORT,GPIO_Pin_1) 
#define nRF24L01_IRQ_High     GPIO_SetBits(nRF24L01_IRQ_PORT,GPIO_Pin_1)

#define nRF24L01_READ_IRQ    	GPIO_ReadInputDataBit(nRF24L01_IRQ_PORT,GPIO_Pin_1)


#define TX_ADR_WIDTH    5   // 5 bytes TX(RX) address width
#define TX_PLOAD_WIDTH  20  // 16 bytes TX payload


/*****************************************************************************
 ** �豸�����궨�� �� ��ֲ�����޸�
 ****************************************************************************/
/***  ������ ��8��  **************/
#define R_REGISTER   0X00  // �����üĴ���, 0x000AAAAA�� 000������ǰꡣ� AAAAA��Ҫ�������ļĴ�����ַ
#define W_REGISTER   0x20  // д���üĴ���, 0x001AAAAA�� 001������ǰꡣ� AAAAA��Ҫ�������ļĴ�����ַ
#define R_RX_PAYLOAD 0X61  // ��RX��Ч����
#define W_TX_PAYLOAD 0XA0  // дTX��Ч����
#define FLUSH_TX     0XE1  // ���TX FIFO�Ĵ��� Ӧ���ڷ���ģʽ
#define FLUSH_RX     0XE2  // ���RX FIFO�Ĵ��� Ӧ���ڽ���ģʽ
#define REUSE_TX_PL  0XE3  // ����ʹ����һ���ݰ�
#define NOP          0XFF  // �ղ���
/***  �Ĵ�����ַ  ****************/
#define CONFIG       0X00  // bit0(PRIM_RX ):1����ģʽ 0����ģʽ bit1(pwr_up):1�ϵ�0����bit3��1 16λCRCУ�� 0 ��λCRCУ��   //bit3:CRCʹ�� bit4:�������ж�MAX_RT bit5:�������ж�TX_DS bit6�������ж�RX_RD
#define EN_AA        0X01  // ʹ��0����5ͨ�����Զ�Ӧ����
#define EN_RXADDR    0X02  // ���յ�ַ���� 0-5 ͨ�� ��Ĭ��ͨ��0ͨ��1����
#define SETUP_AW     0X03  // ���õ�ַ��� 00����Ч 01��3�ֽ� 10���ֽ� 11��5�ֽ�
#define SETUP_RETR   0X04  // �����Զ��ط� 3:0 �Զ��ط����� 7:4 �Զ��ط���ʱ
#define RF_CH        0X05  // ��Ƶͨ�� 6:0 ����SI24R1����Ƶ��
#define RF_SETUP     0X06  // ��Ƶ�Ĵ��� 0:�������Ŵ������� 2:1 ���书�� 3:����Ч��
#define STATUS       0X07  // ״̬�Ĵ��� 0:TX FIFO�Ĵ�������־ 3:1 ��������ͨ���� 4:�ﵽ����ط��ж� 5�����ݷ�������ж� 6�����ݽ����ж�
#define OBSERVE_TX   0X08  // 3:0 �ط�������(���������ݰ�ʱ��λ) 7:4 ���ݰ���ʧ������(дRF_CHʱ��λ)
#define CD           0X09  // �ز����
#define RX_ADDR_P0   0X0A  // ����ͨ��0���յ�ַ ����󳤶�:5�ֽ�(��д���ֽڣ���д�ֽ������SETUP_AW�趨)
#define RX_ADDR_P1   0X0B  // ����ͨ��1���յ�ַ ����󳤶�:5�ֽ�(��д���ֽڣ���д�ֽ������SETUP_AW�趨)
#define RX_ADDR_P2   0X0C  // ����ͨ��2���յ�ַ ,����ֽڿ����á����ֽڲ��ֱ�����RX_ADDR_P1[39:8]���
#define RX_ADDR_P3   0X0D  // ����ͨ��3���յ�ַ ,����ֽڿ����á����ֽڲ��ֱ�����RX_ADDR_P1[39:8]���
#define RX_ADDR_P4   0X0E  // ����ͨ��4���յ�ַ ,����ֽڿ����á����ֽڲ��ֱ�����RX_ADDR_P1[39:8]���
#define RX_ADDR_P5   0X0F  // ����ͨ��5���յ�ַ ,����ֽڿ����á����ֽڲ��ֱ�����RX_ADDR_P1[39:8]���
#define TX_ADDR      0X10  // ���͵�ַ 39:0
#define RX_PW_P0     0X11  // ��������ͨ��0��Ч���ݿ��(��1��32�ֽ�)
#define RX_PW_P1     0X12  // ��������ͨ��1��Ч���ݿ��(��1��32�ֽ�)
#define RX_PW_P2     0X13  // ��������ͨ��2��Ч���ݿ��(��1��32�ֽ�)
#define RX_PW_P3     0X14  // ��������ͨ��3��Ч���ݿ��(��1��32�ֽ�)
#define RX_PW_P4     0X15  // ��������ͨ��4��Ч���ݿ��(��1��32�ֽ�)
#define RX_PW_P5     0X16  // ��������ͨ��5��Ч���ݿ��(��1��32�ֽ�)
#define FIFO_STATUS  0X17  // FIFO״̬�Ĵ��� 0:RX FIFO�Ĵ����ձ�־ 1:RX FIFO�Ĵ�������־ 4:TX FIFO�Ĵ����ձ�־
/**  0x07�Ĵ���STATUS �ж��ж�ֵ *******/
#define STATUS_RX    0x40  // �������
#define STATUS_TX    0x20  // �������
#define STATUS_MAX   0x10  // ����ط�����
/**  ͨ�����  ************************/
#define RX_DR           6  // ���ݽ�������жϱ�־λ
#define TX_DR           5  // ���ݷ�������жϱ�־λ (״̬�Ĵ���λ��)
#define IT_TX        0x0E  // ����ģʽ
#define IT_RX        0x0F  // ����ģʽ
/** ���͡��������ݳ��ȣ��ֽ���  *********/
#define TX_PAYLO_WIDTH 32  // �������ݳ���
#define RX_PAYLO_WIDTH 32  // �������ݳ���
/** Ŀ�ꡢ���ص�ַ���ȣ��ֽ���  *********/
#define TX_ADR_WIDTH    5  // ���͵�ַ���
#define RX_ADR_WIDTH    5  // ���ص�ַ���

// NRF24L01����ͨ�Ų���
#define NRF24L01_CONFIG_CH            30                    // Ƶ�ʣ�������Χ��<126, ʵ��ʹ��Ƶ�ʣ�2400+x
#define NRF24L01_CONFIG_CRC           16                    // CRCУ��ֵ(λ)�� ��ѡ������8��16
#define NRF24L01_CONFIG_RF_DR          0                    // ��Ƶ�����ʣ� 0��1Mbps, 1:2Mbps, 4:250Kbps
#define NRF24L01_CONFIG_RF_PWR         7                    // ���书�ʣ� 7:7dBm, 6:4dBm, 5:3dBm, 4:1dBm, 3:0dBm, 2:-4dBm, 1-6dBm, 0:-12dBm
#define NRF24L01_CONFIG_DATA_LENGTH   32                    // ÿһ�����ݰ������ֽ��������ֵ:32�ֽ�  
static  u8 aNrfTxAdd[5]={0x0A,0x0B,0x0C,0x0D,0xAA};         // ���͵�Ŀ���ַ�� ������⣺�Է����ֻ����롣��ַ��ȣ�5�ֽڣ�40λ
static  u8 aNrfRxAdd[5]={0x0A,0x0B,0x0C,0x0D,0xCC};         // ���صĽ��յ�ַ�� ������⣺�Լ����ֻ����롣��ַ��ȣ�5�ֽڣ�40λ

/** ���͡��������ݳ��ȣ��ֽ���  *********/
#define TX_PAYLO_WIDTH 32  // �������ݳ���
#define RX_PAYLO_WIDTH 32  // �������ݳ���
/** Ŀ�ꡢ���ص�ַ���ȣ��ֽ���  *********/
#define TX_ADR_WIDTH    5  // ���͵�ַ���
#define RX_ADR_WIDTH    5  // ���ص�ַ���


/*****************************************************************************
 ** ����  ȫ�ֱ���
 ** ������5��
****************************************************************************/
typedef struct 
{
    // ����
    u8 Hz;                          // (δ�õ�)Ƶ�ʣ�������Χ��<126, ʵ��ʹ��Ƶ�ʣ�2400+x
    u8 crc;                         // (δ�õ�)CRCУ��ֵ(λ)�� ��ѡ������8��16
    u8 radioBaudrate;               // (δ�õ�)��Ƶ�����ʣ� 0��1Mbps, 1:2Mbps, 4:250Kbps
    u8 radioPower;                  // (δ�õ�)���书�ʣ� 7:7dBm, 6:4dBm, 5:3dBm, 4:1dBm, 3:0dBm, 2:-4dBm, 1-6dBm, 0:-12dBm
    u8 targetAddress[5];            // ���͵�Ŀ���ַ�� ������⣺�Է����ֻ����롣��ַ��ȣ�5�ֽڣ�40λ
    u8 localAddress[5];             // ���صĽ��յ�ַ�� ������⣺�Լ����ֻ����롣��ַ��ȣ�5�ֽڣ�40λ
    u8 dataLength;                  // (δ�õ�)ÿһ�����ݰ������ֽ�����Ϊ������븴����ƣ�����Ϊ���ֵ����32�ֽ�  
    // ״̬
    u8 InitOK;                      // �豸��ʼ��״̬����:��ʼ��ʧ�ܣ�����:��ʼ���ɹ�
    u8 Busy;                        // δ�õ�
    u8 Received;                    // ���ݽ�����ɱ�־�����յ����ݺ��ã����ⲿ�����������ݺ��ã�
    u8 RxData [50];                 // ���ݽ��մ�ţ����յ����ݺ󣬴��豸�����˻��������ȴ��ⲿ��������
    u8 StateIRQ;                    // �ж�״̬, ����ʱʹ��
}xNRF24L01_TypeDef;
extern xNRF24L01_TypeDef  xNRF24L01;


unsigned char  nRF24LO1_Init(void);
void nRF24LO1_UserConfig(void);

 unsigned char nRF24L01_WriteReg(unsigned char Reg,unsigned char Dat);
 unsigned char nRF24L01_ReadReg(unsigned char Reg);
 unsigned char nRF24L01_WirteBuffer(unsigned char Reg, unsigned char *pBuf, unsigned char Len);
 unsigned char nRF24L01_ReadBuffer(unsigned char Reg, unsigned char *pBuf, unsigned char Len);
void nRF24L01_TxMode(void);
void nRF24L01_RxMode(void);
extern unsigned char TX_ADDRESS[TX_ADR_WIDTH];

void nRF24L01_ReadConfig(void);
	
void nRF24L01_TxData(u8 *txbuf);
void nRF24L01_RxData(u8 *rxbuf);

 u8 CheckHardwareConnection(void);
void nRF24L01_IRQ_IRQHANDLER(void);
#endif
