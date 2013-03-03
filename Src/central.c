#include "central.h"
					   
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

	switch(commandSwitch){
		case 0:
			if(NFComBuf.SetDrivesSpeed.addr[0] != NFComBuf.SetDrivesSpeed.addr[1]){
				commArray[commCnt++] = NF_COMMAND_SetDrivesSpeed;
				commArray[commCnt++] = NF_COMMAND_ReadDrivesPosition;
				deviceAddress = NFComBuf.SetDrivesSpeed.addr[0];
				commandSwitch ++;
				break;
			}
			commandSwitch ++;
			//no break here
		case 1:
			commArray[commCnt++] = NF_COMMAND_SetDrivesSpeed;
			commArray[commCnt++] = NF_COMMAND_ReadDrivesPosition;
			deviceAddress = NFComBuf.SetDrivesSpeed.addr[1];
			commandSwitch = 0;
			break;
		default:
			commandSwitch ++;
			break;
	}

	Usart1.txCnt = NF_MakeCommandFrame(&NFComBuf, (uint8_t*)Usart1.txBuf, (const uint8_t*)commArray, commCnt, deviceAddress);
	USART1_SendNBytes((char*)Usart1.txBuf, Usart1.txCnt);
}

void systemShutDown(uint16_t time100ms){
	MCentral.shutdownCounter = time100ms;
}

void systemMonitor(void){
	MCentral.batteryLow = (NFComBuf.ReadDeviceVitals.data[0] < MCentral.batteryVoltageWarn) ? 1 : 0;

	if(MCentral.shutdownCounter > 0){
		MCentral.shutdownCounter--;
		if(MCentral.shutdownCounter == 0)
			R_OFF_H();
	}
}
