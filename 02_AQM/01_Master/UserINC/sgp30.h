/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __SGP30_H_
#define	__SGP30_H_


#include <stm32f10x_conf.h>

#define SGP30_read  0xb1  //SGP30的读地址
#define SGP30_write 0xb0  //SGP30的写地址
#define CRC8_POLYNOMIAL 0x31






void SGP30_Init(void);
void SGP30_Write(unsigned char CmdMsb, unsigned char CmdLsb);
void SGP30_Read(unsigned int *p);
u32 SGP30_Read_Value(void);
//void GSP30_Get_Dat(void);
int GSP30_Get_Dat(void);
static unsigned char Checksum(unsigned  *Check_Dat,unsigned char Num,unsigned char Check_CRC);
void GSP30_Multiple_Read_Byte(unsigned char len,uint8_t* GSP30RecBuffer );
uint8_t CheckCrc8(uint8_t * const message, uint8_t  initial_value);

#endif	/* __SGP30_H_ */

