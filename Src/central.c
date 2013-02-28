#include "central.h"
					   
extern STDOWNCNT_St	STDownCnt[ST_Downcounters];	

extern MCENTRAL_St		MCentral;
extern MMOTOR_St		*MMotor;				  
extern REFERENCE_St		Reference;	 
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

void commandMotors(void){
	uint8_t commArray[2];
	uint8_t commCnt;
	uint8_t bytesToSend;

	// Command Drive 1
	commArray[0] = NF_COMMAND_SetDrivesSpeed;
	commCnt = 1;
	bytesToSend = NF_MakeCommandFrame(&NFComBuf, (uint8_t*)Usart1.txBuf, (const uint8_t*)commArray, commCnt, NF_MotorDrv1Address);
	USART1_SendNBytes((char*)Usart1.txBuf, bytesToSend);	 
					  
	// Command Drive 2
	commArray[0] = NF_COMMAND_SetDrivesSpeed;
	commCnt = 1;
	bytesToSend = NF_MakeCommandFrame(&NFComBuf, (uint8_t*)Usart1.txBuf, (const uint8_t*)commArray, commCnt, NF_MotorDrv2Address);
	USART1_SendNBytes((char*)Usart1.txBuf, bytesToSend);
	
	// Command Unimeter 1

	
	// Ponizej stare podejscie z komunikacja tekstowa
	/*
	switch(MCentral.mode){
		case	M_ER_STOP:
		case	M_STOP:
			sprintf((char*) Usart1.txBuf, ":M0:STOP\r\n"); 
			USART1_SendString((char*)Usart1.txBuf);	
			sprintf((char*) Usart1.txBuf, ":M1:STOP\r\n"); 
			USART1_SendString((char*)Usart1.txBuf);	
		break;
		case	M_SPEED:
			sprintf((char*) Usart1.txBuf, ":M0:SP %d\r\n", Reference.leftSpeed); 
			USART1_SendString((char*)Usart1.txBuf);	
			sprintf((char*) Usart1.txBuf, ":M1:SP %d\r\n", Reference.rightSpeed); 
			USART1_SendString((char*)Usart1.txBuf);	
		break;
	}
	*/
}

void commandSensors(void){
	uint8_t commArray[2];
	uint8_t commCnt;
	uint8_t bytesToSend;
						 
	// Command Unimeter 1
	commArray[0] = NF_COMMAND_ReadAnalogInputs;
	commCnt = 1;
	bytesToSend = NF_MakeCommandFrame(&NFComBuf, (uint8_t*)Usart1.txBuf+1, (const uint8_t*)commArray, commCnt, NF_InOut1Address);
	USART1_SendNBytes((char*)Usart1.txBuf+1, bytesToSend);

	//USART1_SendString(":S0?\r\n");	
}

void systemMonitor(void){

}
