#ifndef _USART4_H
#define _USART4_H  

#include "common.h"	 
#include <stdio.h>		
#include <string.h>		
#include "led.h"  
#include "systick.h"
#include "nf/nfv2.h"
			 
#define USART4_RxFrameSz	11
					
void USART4_Config(void);
void USART4_SendNBytes(char* buf, uint8_t bytesCnt);	

#endif //_USART4_H

