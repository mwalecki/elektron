//##                                      #### ######## ################ INCLUDES	 
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "clock.h"		
#include "interrupts.h"
#include "led.h"
#include "systick.h"
#include "usart1.h"
#include "usart4.h"
#include "io.h"
#include "adc.h"
#include "central.h"
#include "mycrc.h"
#include "nfv2.h"

#include "usb.h"

//##                                      #### ######## ################ GLOBALS   
uint8_t				crcTable[256]; 
USART_St			Usart1, Usart4, USBBufs;
LED_St				Led;		 
STDOWNCNT_St		STDownCnt[ST_Downcounters];	
REFERENCE_St		Reference;
   
MCENTRAL_St			MCentral;	
MMOTOR_St			MMotor[2];	
MSENSE_St			MSense;	

NF_STRUCT_ComBuf 	NFComBuf;

uint8_t USBcommArray[2];
uint8_t USBcommCnt;

//extern u32 count_out;
//extern u8 buffer_out[VIRTUAL_COM_PORT_DATA_SIZE];

//extern u8 buffer_in[VIRTUAL_COM_PORT_DATA_SIZE];
//extern u32 count_in;

uint8_t commArray[2];
uint8_t commCnt, usbBytesToSend, bufout_iter;	

__ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_dev __ALIGN_END ;

//##                                      #### ######## ################ MAIN
int main(void)
{
	uint8_t commandSwitch = 0;

	//SystemInit();					// Init system clock (from library)
	//RCC_Configuration();			// Init system clock (hand made)  
	STM32EU_CL_RCC_Configuration();	// Init system clock (stm32.eu for CL)  
	NVIC_Configuration();	// NVIC_Configuration
	SYSTICK_Init(STDownCnt);
	LED_Config();
	OUT_Config();
	ADC_Config();
	crcInit();
	USART1_Config();
	USART4_Config();
			
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	Set_USBClock();
//	USB_Interrupts_Config();			
//	USB_Init();

	USBD_Init(&USB_OTG_dev,
#ifdef USE_USB_OTG_HS 
            USB_OTG_HS_CORE_ID,
#else            
            USB_OTG_FS_CORE_ID,
#endif  
            &USR_desc, 
            &USBD_CDC_cb, 
            &USR_cb);

	LED_Set(1<<0, //mask
			1<<0,	//newState
			1<<0);//blink

	
	MSense.dist[0] = 5;
	MSense.dist[1] = 6;
	MSense.dist[2] = 7;
	MSense.dist[3] = 8;
	MSense.dist[4] = 9;
	MSense.dist[5] = 10;
	MSense.dist[6] = 11;
	MSense.dist[7] = 10;
	
	/*NFComBuf.ReadAnalogInputs.data[0] = 5;
	NFComBuf.ReadAnalogInputs.data[1] = 6;
	NFComBuf.ReadAnalogInputs.data[2] = 7;
	NFComBuf.ReadAnalogInputs.data[3] = 8;
	NFComBuf.ReadAnalogInputs.data[4] = 9;
	NFComBuf.ReadAnalogInputs.data[5] = 10;
	NFComBuf.ReadAnalogInputs.data[6] = 11;
	NFComBuf.ReadAnalogInputs.data[7] = 10;*/

	
	NFComBuf.ReadDeviceVitals.data[0] = 24500;
	NFComBuf.ReadDeviceVitals.data[1] = 24000;
	NFComBuf.ReadDeviceVitals.data[2] = 12000;
	NFComBuf.ReadDeviceVitals.data[3] = 5000;
	NFComBuf.ReadDeviceVitals.data[4] = 24500;
	NFComBuf.ReadDeviceVitals.data[5] = 24500;
	NFComBuf.ReadDeviceVitals.data[6] = 0;
	NFComBuf.ReadDeviceVitals.data[7] = 0;
	
	NFComBuf.myAddress = NF_MainModuleAddress;

	NFComBuf.SetDrivesMode.addr[0] = NF_MotorDrv1Address;
	NFComBuf.SetDrivesMode.addr[1] = NF_MotorDrv2Address;

	NFComBuf.SetDrivesSpeed.addr[0] = NF_MotorDrv1Address;
	NFComBuf.SetDrivesSpeed.addr[1] = NF_MotorDrv2Address;

	NFComBuf.ReadDeviceVitals.addr[0] = NF_MainModuleAddress;
	NFComBuf.ReadDeviceVitals.addr[1] = NF_MainModuleAddress;
	NFComBuf.ReadDeviceVitals.addr[2] = NF_MainModuleAddress;
	NFComBuf.ReadDeviceVitals.addr[3] = NF_MainModuleAddress;
	NFComBuf.ReadDeviceVitals.addr[4] = NF_MainModuleAddress;
	NFComBuf.ReadDeviceVitals.addr[5] = NF_MainModuleAddress;
	NFComBuf.ReadDeviceVitals.addr[6] = NF_MainModuleAddress;
	NFComBuf.ReadDeviceVitals.addr[7] = NF_MainModuleAddress;

	NFComBuf.ReadAnalogInputs.addr[0] = NF_UniMeter1Address;
	NFComBuf.ReadAnalogInputs.addr[1] = NF_UniMeter1Address;
	NFComBuf.ReadAnalogInputs.addr[2] = NF_UniMeter1Address;
	NFComBuf.ReadAnalogInputs.addr[3] = NF_UniMeter1Address;
	NFComBuf.ReadAnalogInputs.addr[4] = NF_UniMeter1Address;
	NFComBuf.ReadAnalogInputs.addr[5] = NF_UniMeter1Address;
	NFComBuf.ReadAnalogInputs.addr[6] = NF_UniMeter1Address;
	NFComBuf.ReadAnalogInputs.addr[7] = NF_UniMeter1Address;

	
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
			switch(commandSwitch){
				case 0:
					commandSensors();
					commandSwitch ++;
					break;
				case 5:
					commandMotors();
					commandSwitch = 0;
					break;
				default:
					commandMotors();
					commandSwitch ++;
					break;
			}
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
			LED_Proc();
			STDownCnt[ST_StatusLed].tick = 0;

			NFComBuf.ReadDeviceVitals.data[0] = 55 * (readADC1(4)	/4);	//POWER-BATT
			NFComBuf.ReadDeviceVitals.data[1] = 55 * (readADC1(13)	/4);	//24V
			NFComBuf.ReadDeviceVitals.data[2] = 55 * (readADC1(12)	/4);	//12V
			NFComBuf.ReadDeviceVitals.data[3] = 55 * (readADC1(10)	/4);	//5V
			NFComBuf.ReadDeviceVitals.data[4] = 55 * (readADC1(15)	/4);	//PS1
			NFComBuf.ReadDeviceVitals.data[5] = 55 * (readADC1(8)	/4);	//PS2
			NFComBuf.ReadDeviceVitals.data[6] = 55 * (readADC1(6)	/4);	//Vchg
			NFComBuf.ReadDeviceVitals.data[7] = 55 * (readADC1(0)	/4);	//Ichg
		}		   
		if(STDownCnt[ST_UsartCmdTo].tick){
			modeSwitch(M_ER_STOP);
			Reference.leftSpeed = 0;
			Reference.rightSpeed = 0;
			STDownCnt[ST_UsartCmdTo].tick = 0;
		}		   
		if(STDownCnt[ST_RelaysOff].tick){
			PS_ON_L();
			STDownCnt[ST_RelaysOff].tick = 0;
		}
	}
}
