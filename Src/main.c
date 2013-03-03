//##                                      #### ######## ################ INCLUDES
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "clock.h"		
#include "interrupts.h"
#include "led.h"
#include "ui.h"
#include "systick.h"
#include "usart1.h"
#include "usart4.h"
#include "io.h"
#include "adc.h"
#include "central.h"
#include "nf/nfv2.h"

#include "usb.h"

//##                                      #### ######## ################ GLOBALS
USART_St			Usart1, Usart4;
USB_St				USBMySCPI, USBNF;
LED_St				Led;
ADC_St				ADC;
STDOWNCNT_St		STDownCnt[ST_Downcounters];	
REFERENCE_St		Reference;
   
MCENTRAL_St			MCentral;	
MMOTOR_St			MMotor[2];	
MSENSE_St			MSense;	

NF_STRUCT_ComBuf 	NFComBuf;

uint8_t USBcommArray[2];
uint8_t USBcommCnt;

uint8_t commArray[2];
uint8_t commCnt, usbBytesToSend, bufout_iter;

//##                                      #### ######## ################ MAIN
int main(void)
{
	//SystemInit();					// Init system clock (from library)
	//RCC_Configuration();			// Init system clock (hand made)  
	STM32EU_CL_RCC_Configuration();	// Init system clock (stm32.eu for CL)  
	NVIC_Configuration();	// NVIC_Configuration
	SYSTICK_Init(STDownCnt);
	LED_Config();
	UI_Config();
	OUT_Config();
	ADCwithDMA_Config();
	USART1_Config();
	USART4_Config();
			
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	USB_Config();

	LED_Set(1<<0, //mask
			1<<0,	//newState
			1<<0);//blink
	
	NFv2_CrcInit();
	NFv2_Config(&NFComBuf, NF_MainModuleAddress);

	
	USART1_SendString("Hello!\r\n");

	while (1){	
		if(Usart1.rxDataReady){    
		//	strncpy((char*)Usart1.tmpBuf,(const char*)Usart1.rxBuf, 15);
			Usart1.rxDataReady=0;
		//	USART1_Interpreter((u8*)Usart1.tmpBuf);
		}   
		//if(USBBufs.txDataReady){
		//	USBBufs.txDataReady = 0;
		//	VCP_myTransmit((uint8_t*)USBBufs.txBuf, USBBufs.txCnt);
		//}
		if(NFComBuf.dataReceived != 0){ 
			NFComBuf.dataReceived = 0;
			ST_Reset(ST_UsartCmdTo);

			if(NFComBuf.SetDrivesMode.data[0] == NF_DrivesMode_SPEED)
				modeSwitch(M_SPEED);
			else 
				modeSwitch(M_ER_STOP);	

			((NFComBuf.SetDigitalOutputs.data[0] & (1<<0)) ? REL1_ON() : REL1_OFF());
			((NFComBuf.SetDigitalOutputs.data[0] & (1<<1)) ? REL2_ON() : REL2_OFF());
			((NFComBuf.SetDigitalOutputs.data[0] & (1<<2)) ? REL3_ON() : REL3_OFF());
			((NFComBuf.SetDigitalOutputs.data[0] & (1<<3)) ? REL4_ON() : REL4_OFF());

			((NFComBuf.SetDigitalOutputs.data[0] & (1<<7)) ? R_OFF_H() : R_OFF_L());
		}   

		if(STDownCnt[ST_CommCycle].tick){
			internalCommunicationCycle();
			STDownCnt[ST_CommCycle].tick = 0;	  		   
		}
		if(STDownCnt[ST_UsartTxDelay].tick){
			if(Usart1.txDataReady){
		//		USART1_SendString((char*) Usart1.txBuf);
		//		USART1_SendString(USART_TxSuffix);
				Usart1.txDataReady = 0;
			}
			STDownCnt[ST_UsartTxDelay].tick = 0;
		}		   
		if(STDownCnt[ST_StatusLed].tick){
			//LED_Proc();
			STDownCnt[ST_StatusLed].tick = 0;


			LED_Set(1<<0, //mask
					1<<0,	//newState
					0<<0);//blink
			UI_LcdPrintAnalogs();
			LED_Set(1<<0, //mask
					0<<0,	//newState
					0<<0);//blink
		}		   
		if(STDownCnt[ST_UsartCmdTo].tick){
			modeSwitch(M_ER_STOP);
			NFComBuf.SetDrivesSpeed.data[0] = 0;
			NFComBuf.SetDrivesSpeed.data[1] = 0;
			STDownCnt[ST_UsartCmdTo].tick = 0;
		}		   
		if(STDownCnt[ST_RelaysOff].tick){
			PS_ON_L();
			STDownCnt[ST_RelaysOff].tick = 0;
		}
		if(STDownCnt[ST_UiProc].tick){
			UI_SpeakerProc();
			UI_KeyboardProc();
			UI_LcdPrintBinaries();
			STDownCnt[ST_UiProc].tick = 0;
		}
		if(STDownCnt[ST_SysMonitor].tick){
			systemMonitor();
			STDownCnt[ST_SysMonitor].tick = 0;
		}
	}
}
