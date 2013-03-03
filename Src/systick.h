#ifndef _SYSTICK_H
#define _SYSTICK_H

#include "common.h"

#define ST_StatusLed	0
#define ST_Period0			5000 // all values in [100us]
#define ST_UiProc		1
#define ST_Period1			100
#define ST_EncSpeed		2
#define ST_Period2			100
#define ST_UsartCmdTo	3
#define ST_Period3			10000
#define ST_UsartTxDelay	4
#define ST_Period4			10
#define ST_CommCycle	5
#define ST_Period5			1500
#define ST_ADC			6
#define ST_Period6			1500
#define ST_RelaysOff	7
#define ST_Period7			5000
#define ST_SysMonitor	8
#define ST_Period8			1000
#define ST_Downcounters	9

#define ST_Period(n)	ST_Period ## n
#define ST_SetPeriod(n,i)	{STDownCnt[n].period = i; STDownCnt[n].val=i;}	  
#define ST_Reset(n)			STDownCnt[n].val=STDownCnt[n].period

// Only 0..9 counters initialized automatically.
// Need more -> see SYSTICK_Config().

void SYSTICK_Init(STDOWNCNT_St*	STDownCnt);

#endif //_SYSTICK_H
