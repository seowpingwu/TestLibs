#include <stm32f10x_conf.h>

static u8  us = 0; //0-255   //全局静态变量存储us
static u16 ms = 0; //0-65535 //全局静态变量存储ms

/************************************************************
 *作者：下班后实验室
 *版本：v3.6.0
 *日期：2022/1/4
 *函数：void Delay_Init(u8 MHZ);
 *作用：延时函数初始化函数设置时钟频率
**************************************************************/
void Delay_Init(u8 Mhz){ 
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //设置时钟8分频
	
	us = Mhz/8;                                           //F103ZET6芯片晶振主频72Mhz除以8M分频 = 9Mhz
	ms = (u16)us * 1000;                                  //强制转换类型(u16);ms与us的进制换算进制1000
}
/************************************************************
 *作者：下班后实验室
 *版本：v3.6.0
 *日期：2022/1/4
 *函数：void Delay_ms(u16 dly);
 *作用：形参dly传递需要设置的时间最长计时1.85秒s
**************************************************************/
void __delay_ms(u16 dly){ //1800
	
	u32 temp; 							                    //中间变量
	
	SysTick ->LOAD = (u32)ms*dly;								//设置重载值ms*形参传递值最长记时1.85s
	SysTick ->VAL  = RESET;                     //清空当前值
	SysTick ->CTRL |= SysTick_CTRL_ENABLE_Msk;	//使能递减计数器/设置软件查询模式不开启中断
	
	do{
		
		temp = SysTick ->CTRL;                    //查询计数状态是否完成
		
	}while((temp&0x01) &&! (temp&(1<<16))); 		//等待COUNTFLAG标志位置一计数结束
	
	SysTick ->VAL  = RESET;                     //清空当前值
	SysTick ->CTRL &=~ SysTick_CTRL_ENABLE_Msk;	//失能递减计数器
}
/************************************************************
 *作者：下班后实验室
 *版本：v3.6.0
 *日期：2022/1/4
 *函数：void DelayMicroseconds(u32 dly);
 *作用：形参dly传递需要设置的时间最长计时1.85秒s
**************************************************************/
void __delay_us(u32 dly){
	
	u32 temp;                                   //中间变量
	
	SysTick ->LOAD = us*dly;										//设置重载值us*形参传递值最长记时1.85s
	SysTick ->VAL  = 0; 										    //清空当前值
	SysTick ->CTRL |= SysTick_CTRL_ENABLE_Msk;	//使能递减计数器/设置软件查询模式不开启中断
	
	do{
		
		temp = SysTick ->CTRL;										//查询计数状态是否完成
		
	}while((temp&0x01) &&! (temp&(1<<16))); 		//等待COUNTFLAG标志位置一计数结束
	
	SysTick ->VAL  = RESET;										  //清空当前值
	SysTick ->CTRL &=~ SysTick_CTRL_ENABLE_Msk; //失能递减计数器
}
