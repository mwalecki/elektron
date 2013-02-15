#ifndef _ADC_H
#define _ADC_H

#include "common.h"

void ADC_Config(void);
void ADCwithDMA_Config(void);
u16 readADC1(u8 channel);

#endif //_ADC_H
