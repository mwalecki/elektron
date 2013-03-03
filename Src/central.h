#ifndef _CENTRAL_H
#define _CENTRAL_H

#include "common.h"
#include "systick.h"
#include "io.h"
#include "usart1.h"

void modeSwitch(u8 newMode);	
void commandMotors(void);
void commandSensors(void);
void systemMonitor(void);
void internalCommunicationCycle(void);

#endif //_CENTRAL_H
