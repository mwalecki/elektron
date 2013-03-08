#ifndef _ADC_H
#define _ADC_H

#include "common.h"

#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define ADC_DMA_TransferCompleteInterrupt

#define ADC_Channels 11
#define ADC_DigitalBytes ((ADC_Channels-1)/8 + 1)

typedef struct{
	vu16 raw[ADC_Channels];
	vu16 milivolt[ADC_Channels];
	vu16 mvMaxTreshold[ADC_Channels];
	vu16 mvMinTreshold[ADC_Channels];
	vu8 overTreshold[ADC_Channels];
//	vu8 digital[ADC_DigitalBytes];
	u32 uVoltsPerUnit;
	u16 unitsOffset;
//	u16 logicZeroMax_mV;
//	u16 logicOneMin_mV;
} ADC_St;


void ADC_Config(void);
void ADCwithDMA_Config(void);
u16 readADC1(u8 channel);	   
void DMA1_Channel1_IRQHandler(void);

#endif //_ADC_H
