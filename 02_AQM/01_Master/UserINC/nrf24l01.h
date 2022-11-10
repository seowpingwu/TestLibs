#ifndef __NRF24L01_H_
#define __NRF24L01_H_

#include "stm32f10x_conf.h"

//******nRF24L01 defeine ***********************

#define nRF24L01_CE_PORT		GPIOC
#define nRF24L01_CE_Pin		  GPIO_Pin_4		 //CE nRF24L01工作模式开启，高电平有效，发射/接收模式控制,CE协同CONFIG寄存器共同决定NRFL2401的状态

#define nRF24L01_IRQ_PORT		GPIOA
#define nRF24L01_IRQ_Pin  	GPIO_Pin_1  //(MCU PA1 EXTI1_IRQn ) IRQ 中断信号，闲时高电平，中断时低电平, 低电平有效，中断输出
#define NRF24L01_IRQ				EXTI15_10_IRQn//


/** 通信控制引脚简化 *******************/
//CE
#define nRF24LO1_CE_LOW  			GPIO_ResetBits(nRF24L01_CE_PORT,nRF24L01_CE_Pin)
#define nRF24L01_CE_High 			GPIO_SetBits(nRF24L01_CE_PORT,nRF24L01_CE_Pin)

//IRQ中断
#define nRF24L01_IRQ_LOW      GPIO_ResetBits(nRF24L01_IRQ_PORT,GPIO_Pin_1) 
#define nRF24L01_IRQ_High     GPIO_SetBits(nRF24L01_IRQ_PORT,GPIO_Pin_1)

#define nRF24L01_READ_IRQ    	GPIO_ReadInputDataBit(nRF24L01_IRQ_PORT,GPIO_Pin_1)


#define TX_ADR_WIDTH    5   // 5 bytes TX(RX) address width
#define TX_PLOAD_WIDTH  20  // 16 bytes TX payload


/*****************************************************************************
 ** 设备参数宏定义 ， 移植不用修改
 ****************************************************************************/
/***  命令字 共8个  **************/
#define R_REGISTER   0X00  // 读配置寄存器, 0x000AAAAA， 000是命令前辍， AAAAA是要读操作的寄存器地址
#define W_REGISTER   0x20  // 写配置寄存器, 0x001AAAAA， 001是命令前辍， AAAAA是要读操作的寄存器地址
#define R_RX_PAYLOAD 0X61  // 读RX有效数据
#define W_TX_PAYLOAD 0XA0  // 写TX有效数据
#define FLUSH_TX     0XE1  // 清除TX FIFO寄存器 应用于发射模式
#define FLUSH_RX     0XE2  // 清除RX FIFO寄存器 应用于接收模式
#define REUSE_TX_PL  0XE3  // 从新使用上一数据包
#define NOP          0XFF  // 空操作
/***  寄存器地址  ****************/
#define CONFIG       0X00  // bit0(PRIM_RX ):1接收模式 0发射模式 bit1(pwr_up):1上电0掉电bit3：1 16位CRC校验 0 八位CRC校验   //bit3:CRC使能 bit4:可屏蔽中断MAX_RT bit5:可屏蔽中断TX_DS bit6可屏蔽中断RX_RD
#define EN_AA        0X01  // 使能0——5通道的自动应答功能
#define EN_RXADDR    0X02  // 接收地址允许 0-5 通道 ，默认通道0通道1启动
#define SETUP_AW     0X03  // 设置地址宽度 00：无效 01：3字节 10：字节 11：5字节
#define SETUP_RETR   0X04  // 建立自动重发 3:0 自动重发计数 7:4 自动重发延时
#define RF_CH        0X05  // 射频通道 6:0 设置SI24R1工作频率
#define RF_SETUP     0X06  // 射频寄存器 0:低噪声放大器增益 2:1 发射功率 3:传输效率
#define STATUS       0X07  // 状态寄存器 0:TX FIFO寄存器满标志 3:1 接收数据通道号 4:达到最大重发中断 5：数据发送完成中断 6：数据接收中断
#define OBSERVE_TX   0X08  // 3:0 重发计数器(发送新数据包时复位) 7:4 数据包丢失计数器(写RF_CH时复位)
#define CD           0X09  // 载波检测
#define RX_ADDR_P0   0X0A  // 数据通道0接收地址 ，最大长度:5字节(先写低字节，所写字节数量覵ETUP_AW设定)
#define RX_ADDR_P1   0X0B  // 数据通道1接收地址 ，最大长度:5字节(先写低字节，所写字节数量覵ETUP_AW设定)
#define RX_ADDR_P2   0X0C  // 数据通道2接收地址 ,最低字节可设置。高字节部分必须与RX_ADDR_P1[39:8]相等
#define RX_ADDR_P3   0X0D  // 数据通道3接收地址 ,最低字节可设置。高字节部分必须与RX_ADDR_P1[39:8]相等
#define RX_ADDR_P4   0X0E  // 数据通道4接收地址 ,最低字节可设置。高字节部分必须与RX_ADDR_P1[39:8]相等
#define RX_ADDR_P5   0X0F  // 数据通道5接收地址 ,最低字节可设置。高字节部分必须与RX_ADDR_P1[39:8]相等
#define TX_ADDR      0X10  // 发送地址 39:0
#define RX_PW_P0     0X11  // 接收数据通道0有效数据宽度(从1到32字节)
#define RX_PW_P1     0X12  // 接收数据通道1有效数据宽度(从1到32字节)
#define RX_PW_P2     0X13  // 接收数据通道2有效数据宽度(从1到32字节)
#define RX_PW_P3     0X14  // 接收数据通道3有效数据宽度(从1到32字节)
#define RX_PW_P4     0X15  // 接收数据通道4有效数据宽度(从1到32字节)
#define RX_PW_P5     0X16  // 接收数据通道5有效数据宽度(从1到32字节)
#define FIFO_STATUS  0X17  // FIFO状态寄存器 0:RX FIFO寄存器空标志 1:RX FIFO寄存器满标志 4:TX FIFO寄存器空标志
/**  0x07寄存器STATUS 中断判断值 *******/
#define STATUS_RX    0x40  // 接收完成
#define STATUS_TX    0x20  // 发送完成
#define STATUS_MAX   0x10  // 最大重发次数
/**  通信相关  ************************/
#define RX_DR           6  // 数据接收完成中断标志位
#define TX_DR           5  // 数据发送完成中断标志位 (状态寄存器位置)
#define IT_TX        0x0E  // 发送模式
#define IT_RX        0x0F  // 接收模式
/** 发送、接收数据长度，字节数  *********/
#define TX_PAYLO_WIDTH 32  // 发送数据长度
#define RX_PAYLO_WIDTH 32  // 接收数据长度
/** 目标、本地地址长度，字节数  *********/
#define TX_ADR_WIDTH    5  // 发送地址宽度
#define RX_ADR_WIDTH    5  // 本地地址宽度

// NRF24L01无线通信参数
#define NRF24L01_CONFIG_CH            30                    // 频率，参数范围：<126, 实际使用频率：2400+x
#define NRF24L01_CONFIG_CRC           16                    // CRC校验值(位)， 可选参数：8、16
#define NRF24L01_CONFIG_RF_DR          0                    // 射频波特率： 0：1Mbps, 1:2Mbps, 4:250Kbps
#define NRF24L01_CONFIG_RF_PWR         7                    // 发射功率： 7:7dBm, 6:4dBm, 5:3dBm, 4:1dBm, 3:0dBm, 2:-4dBm, 1-6dBm, 0:-12dBm
#define NRF24L01_CONFIG_DATA_LENGTH   32                    // 每一包数据包含的字节数．最大值:32字节  
static  u8 aNrfTxAdd[5]={0x0A,0x0B,0x0C,0x0D,0xAA};         // 发送到目标地址， 举例理解：对方的手机号码。地址宽度：5字节，40位
static  u8 aNrfRxAdd[5]={0x0A,0x0B,0x0C,0x0D,0xCC};         // 本地的接收地址， 举例理解：自己的手机号码。地址宽度：5字节，40位

/** 发送、接收数据长度，字节数  *********/
#define TX_PAYLO_WIDTH 32  // 发送数据长度
#define RX_PAYLO_WIDTH 32  // 接收数据长度
/** 目标、本地地址长度，字节数  *********/
#define TX_ADR_WIDTH    5  // 发送地址宽度
#define RX_ADR_WIDTH    5  // 本地地址宽度


/*****************************************************************************
 ** 声明  全局变量
 ** 数量：5个
****************************************************************************/
typedef struct 
{
    // 参数
    u8 Hz;                          // (未用到)频率，参数范围：<126, 实际使用频率：2400+x
    u8 crc;                         // (未用到)CRC校验值(位)， 可选参数：8、16
    u8 radioBaudrate;               // (未用到)射频波特率： 0：1Mbps, 1:2Mbps, 4:250Kbps
    u8 radioPower;                  // (未用到)发射功率： 7:7dBm, 6:4dBm, 5:3dBm, 4:1dBm, 3:0dBm, 2:-4dBm, 1-6dBm, 0:-12dBm
    u8 targetAddress[5];            // 发送到目标地址， 举例理解：对方的手机号码。地址宽度：5字节，40位
    u8 localAddress[5];             // 本地的接收地址， 举例理解：自己的手机号码。地址宽度：5字节，40位
    u8 dataLength;                  // (未用到)每一包数据包含的字节数．为方便代码复用设计，配置为最大值，即32字节  
    // 状态
    u8 InitOK;                      // 设备初始化状态，０:初始化失败，　１:初始化成功
    u8 Busy;                        // 未用到
    u8 Received;                    // 数据接收完成标志，接收到数据后置１，外部函数处理数据后置０
    u8 RxData [50];                 // 数据接收存放，接收到数据后，从设备读到此缓冲区，等待外部函数处理
    u8 StateIRQ;                    // 中断状态, 调试时使用
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
