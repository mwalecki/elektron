#ifndef _CENTRAL_H
#define _CENTRAL_H

#include "common.h"
#include "systick.h"
#include "io.h"
#include "usart1.h"

typedef struct{
	u8	mode;
	u8	computerLink	:1;
	u8	panelLink		:1;
	u8	shuttingDown	:1;
	u8	batteryLow		:1;
	int16_t batteryVoltageShutDown;
	int16_t batteryVoltageWarn;
	uint16_t batteryWarnCounter;
	uint16_t shutdownCounter;
} MCENTRAL_St;

void modeSwitch(u8 newMode);	
void commandMotors(void);
void commandSensors(void);
void systemMonitor(void);
void internalCommunicationCycle(void);

#endif //_CENTRAL_H
