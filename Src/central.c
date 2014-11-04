#include "central.h"
#include "ui.h"
#include "adc.h"
#include "systick.h"
#include "mb.h"
					   
extern STDOWNCNT_St	STDownCnt[ST_Downcounters];	
extern DEVICE_CONTROL_St      DevControl;
extern DEVICE_CONFIGURATION_St DevConfiguration;

extern MCENTRAL_St		MCentral;
extern USART_St			Usart1;
extern ADC_St        ADC;
extern MODBUS_St       ModBus;
																				 
void modeControl(){
  static uint16_t mode;

	if(DevControl.mode == mode)
		return;

	mode = DevControl.mode;

  switch(mode){
    case M_STOP:
      PS_EN_L();
      break;
    case M_SMOOTH_STOP:
      systemDelayedPowerStageOff(DevConfiguration.MotorPoweroffDelay);
      break;
    default:
      PS_EN_H();
      ST_Reset(ST_RelaysOff);
      PS_ON_H();
      break;
		}
}

void systemShutDown(uint16_t seconds){
	MCentral.shutdownCounter = (10000 / STDownCnt[ST_SysMonitor].period) * seconds;
}

void systemBatteryWarn(uint16_t seconds){
	MCentral.batteryWarnCounter = (10000 / STDownCnt[ST_SysMonitor].period) * seconds;
}

void systemDelayedPowerStageOff(uint16_t milisec100){
  MCentral.PSOffCounter = (1000 / STDownCnt[ST_SysMonitor].period) * milisec100;
}

void systemMonitor(void){
	static uint16_t battLowCnt;
	static uint16_t battCritCnt;

	MCentral.batteryLow = (ADC.milivolt[4] < MCentral.batteryVoltageLow) ? 1 : 0;
	MCentral.batteryCritical = (ADC.milivolt[4] < MCentral.batteryVoltageCritical) ? 1 : 0;

	// Battery Low
	if(MCentral.batteryLow != 0  && MCentral.batteryCritical == 0){
		if(MCentral.batteryWarnCounter == 0)
			systemBatteryWarn(BATTERY_WARN_T);
	}
	else{
		systemBatteryWarn(0);
	}

	// Battery Critical
	if(MCentral.batteryCritical){
		if(MCentral.shutdownCounter == 0)
			systemShutDown(BATT_CRIT_SHUTDOWN_T);
	}
	else{
		systemShutDown(0);
	}


	// Battery Warn Counter
	if(MCentral.batteryWarnCounter > 0){
		MCentral.batteryWarnCounter--;
		if(MCentral.batteryWarnCounter == 0) {
			UI_SND_WARNING;
			systemBatteryWarn(BATTERY_WARN_T);
		}
	}

	// ShutDown Counter
	if(MCentral.shutdownCounter > 0){
		MCentral.shutdownCounter--;
		if(MCentral.shutdownCounter == 0)
			R_OFF_H();
	}
	if(MCentral.shutdownCounter == (7000 / STDownCnt[ST_SysMonitor].period) * BATT_CRIT_SHUTDOWN_T){
		UI_SND_POWER_OFF;
	}

  // Power Stage Off Counter
  if(MCentral.PSOffCounter > 0){
    MCentral.PSOffCounter--;
    if(MCentral.PSOffCounter == 0) {
      PS_EN_L();
    }
  }
}
