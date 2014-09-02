#include "central.h"
#include "ui.h"
#include "systick.h"
					   
extern STDOWNCNT_St	STDownCnt[ST_Downcounters];	

extern MCENTRAL_St		MCentral;
extern USART_St			Usart1;
extern NF_STRUCT_ComBuf 	NFComBuf;
																				 
void modeSwitch(u8 newMode){
	if(newMode == MCentral.mode)
		return;
	else
		switch(newMode){
			case M_STOP:
			case M_SPEED:
				PS_EN_H();
				ST_Reset(ST_RelaysOff);
				NFComBuf.SetDrivesMode.data[0] = NF_DrivesMode_SPEED;
				NFComBuf.SetDrivesMode.data[1] = NF_DrivesMode_SPEED;
				PS_ON_H();
				break;
			case M_PWM:
				PS_EN_H();
				ST_Reset(ST_RelaysOff);
				NFComBuf.SetDrivesMode.data[0] = NF_DrivesMode_PWM;
				NFComBuf.SetDrivesMode.data[1] = NF_DrivesMode_PWM;
				PS_ON_H();
				break;
			case M_ER_STOP:
				PS_EN_L();
				NFComBuf.SetDrivesMode.data[0] = NF_DrivesMode_ERROR;
				NFComBuf.SetDrivesMode.data[1] = NF_DrivesMode_ERROR;
				break;
		}
	MCentral.mode = newMode;
}

void internalCommunicationCycle(void){
	uint8_t commArray[4];
	uint8_t commCnt = 0;
	uint8_t deviceAddress = 0;
	static uint8_t commandSwitch = 0;
	static int speedRegulatorsSet=0;
	static int maxSpeedSet=0;

	switch(commandSwitch){
		case 0:
			if(NFComBuf.SetDrivesSpeed.addr[0] != NFComBuf.SetDrivesSpeed.addr[1]){
				commArray[commCnt++] = NF_COMMAND_SetDrivesMode;
				switch(NFComBuf.SetDrivesMode.data[0]){
				case NF_DrivesMode_SPEED:
					commArray[commCnt++] = NF_COMMAND_SetDrivesSpeed;
					break;
				case NF_DrivesMode_PWM:
					commArray[commCnt++] = NF_COMMAND_SetDrivesPWM;
					break;
				}
				commArray[commCnt++] = NF_COMMAND_ReadDrivesPosition;
				// Speed Regulator
//				if(NFComBuf.SetSpeedRegulator.updated){
//					commArray[commCnt++] = NF_COMMAND_SetSpeedRegulator;
//					if((++speedRegulatorsSet) == 2){
//						NFComBuf.SetSpeedRegulator.updated = 0;
//						speedRegulatorsSet = 0;
//					}
//				}
				// Max Speed
				if(NFComBuf.SetDrivesMaxSpeed.updated){
					commArray[commCnt++] = NF_COMMAND_SetDrivesMaxSpeed;
					if(++maxSpeedSet == 2){
						NFComBuf.SetDrivesMaxSpeed.updated = 0;
						maxSpeedSet = 0;
					}
				}
				deviceAddress = NFComBuf.SetDrivesSpeed.addr[0];
				commandSwitch ++;
				break;
			}
			commandSwitch ++;
			//no break here
		case 1:
			commArray[commCnt++] = NF_COMMAND_SetDrivesMode;
			switch(NFComBuf.SetDrivesMode.data[1]){
			case NF_DrivesMode_SPEED:
				commArray[commCnt++] = NF_COMMAND_SetDrivesSpeed;
				break;
			case NF_DrivesMode_PWM:
				commArray[commCnt++] = NF_COMMAND_SetDrivesPWM;
				break;
			}
			commArray[commCnt++] = NF_COMMAND_ReadDrivesPosition;
			commArray[commCnt++] = NF_COMMAND_ReadDigitalInputs;
			// Speed Regulator
//			if(NFComBuf.SetSpeedRegulator.updated){
//				commArray[commCnt++] = NF_COMMAND_SetSpeedRegulator;
//				if((++speedRegulatorsSet) == 2){
//					NFComBuf.SetSpeedRegulator.updated = 0;
//					speedRegulatorsSet = 0;
//				}
//			}
			// Max Speed
			if(NFComBuf.SetDrivesMaxSpeed.updated){
				commArray[commCnt++] = NF_COMMAND_SetDrivesMaxSpeed;
				if(++maxSpeedSet == 2){
					NFComBuf.SetDrivesMaxSpeed.updated = 0;
					maxSpeedSet = 0;
				}
			}
			deviceAddress = NFComBuf.SetDrivesSpeed.addr[1];
			commandSwitch ++;
			break;
		case 2:
			commArray[commCnt++] = NF_COMMAND_SetServosPosition;
			commArray[commCnt++] = NF_COMMAND_SetDigitalOutputs;
			commArray[commCnt++] = NF_COMMAND_ReadAnalogInputs;
			commArray[commCnt++] = NF_COMMAND_ReadDigitalInputs;
			deviceAddress = NF_InOut1Address;
			commandSwitch = 0;
			break;
		default:
			commandSwitch ++;
			break;
	}

	Usart1.txCnt = NF_MakeCommandFrame(&NFComBuf, (uint8_t*)Usart1.txBuf, (const uint8_t*)commArray, commCnt, deviceAddress);
	USART1_SendNBytes((char*)Usart1.txBuf, Usart1.txCnt);
}

void systemShutDown(uint16_t seconds){
	MCentral.shutdownCounter = (10000 / STDownCnt[ST_SysMonitor].period) * seconds;
}

void systemBatteryWarn(uint16_t seconds){
	MCentral.batteryWarnCounter = (10000 / STDownCnt[ST_SysMonitor].period) * seconds;
}

void systemMonitor(void){
	static uint16_t battLowCnt;
	static uint16_t battCritCnt;

	MCentral.batteryLow = (NFComBuf.ReadDeviceVitals.data[0] < MCentral.batteryVoltageLow) ? 1 : 0;
	MCentral.batteryCritical = (NFComBuf.ReadDeviceVitals.data[0] < MCentral.batteryVoltageCritical) ? 1 : 0;

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
}
