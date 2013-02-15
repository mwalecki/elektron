#ifndef _USART1_H
#define _USART1_H  

#include "common.h"	 
#include <stdio.h>		
#include <string.h>		
#include "led.h"  
#include "systick.h"
#include "nf/nfv2.h"
			 
#define USART1_RxFrameSz	16
					
void USART1_Config(void);
void USART1_TXEN_H(void);
void USART1_TXEN_L(void);		
void USART1_Interpreter(u8* buf);
void USART1_SendString(char* buf); 
void USART1_SendNBytes(char* buf, uint8_t bytesCnt);

#endif //_USART1_H

