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
#include "eeprom.h"
#include "central.h"
#include "circbuf.h"

#include "usb.h"
#include "mb.h"

//##                                      #### ######## ################ GLOBALS
USART_St			Usart1, Usart4;
CircularBuffer		cbUsart1Received;
USB_St				USBMySCPI, USBNF;
LED_St				Led;
ADC_St				ADC;
STDOWNCNT_St		STDownCnt[ST_Downcounters];	
REFERENCE_St		Reference;
   
MCENTRAL_St			MCentral;	
MMOTOR_St			MMotor[2];	
MSENSE_St			MSense;
DEVICE_DIAGNOSTICS_St   DevDiagnostics;
DEVICE_STATE_St         DevState;
DEVICE_CONTROL_St       DevControl;
DEVICE_CONFIGURATION_St DevConfiguration;

uint8_t USBcommArray[2];
uint8_t USBcommCnt;

uint8_t commArray[2];
uint8_t commCnt, usbBytesToSend, bufout_iter;

MODBUS_St       ModBus;

//##                                      #### ######## ################ MAIN
int main(void)
{
	uint8_t temp8 = 0;

	// Init system clock (stm32.eu for CL)
	STM32EU_CL_RCC_Configuration();

	// Init nonvolatile memory and recover saved values of peripherals data buffers
  EEPROM_Init(0);
  eebackup_Recover();
  // Configure peripherals
  eMBSynchronizeFromHoldingRegs();

  // Then init peripherals
	LED_Config();
	UI_Config();
	OUT_Config();
	IN_Config();
	ADCwithDMA_Config();
//	USART1_Config();
//	USART4_Config();

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  MODBUS_Config( 0x01 , DevConfiguration.serialInterfaceBitrate);

  NVIC_Configuration();
	

	USB_Config();

	LED_Set(1<<0,	//mask
			1<<0,	//newState
			1<<0);	//blink

	SYSTICK_Init(STDownCnt);

	USART1_SendString("Test\r\n");

  //#### MAIN LOOP ####//
	while (1){
	  //#### #### MODBUS POLL
    if(eMBPoll(  ) == MB_ENOERR_MBEVENT)
        ST_Reset(ST_CommandWD);
    // Check whether new data appeared in registers
    if(ModBus.usRegHoldingFlags.dataReceivedGlobal == 1){
      eMBSynchronizeFromHoldingRegs();
      ModBus.usRegHoldingFlags.dataReceivedGlobal = 0;
    }
    if(STDownCnt[ST_MBHiPrioUpd].tick){
      //#### #### Register Update
      eMBSynchronizeToHoldingRegs(MB_HR_RANGE_State);
      STDownCnt[ST_MBHiPrioUpd].tick = 0;
    }

    if(STDownCnt[ST_MBLowPrioUpd].tick){
      eMBSynchronizeToHoldingRegs(MB_HR_RANGE_Diagnostics | MB_HR_RANGE_Control | MB_HR_RANGE_Config);
      STDownCnt[ST_MBLowPrioUpd].tick = 0;
    }

		if(STDownCnt[ST_StatusLed].tick){
			LED_Proc();
#ifdef LCD_ON
			UI_LcdPrintAnalogs();
      UI_LcdPrintBinaries();
#endif
			STDownCnt[ST_StatusLed].tick = 0;
		}		   
		if(STDownCnt[ST_CommandWD].tick){
			DevControl.mode = 0;
			STDownCnt[ST_CommandWD].tick = 0;
		}		   
		if(STDownCnt[ST_RelaysOff].tick){
			PS_ON_L();
			STDownCnt[ST_RelaysOff].tick = 0;
		}
		if(STDownCnt[ST_UiProc].tick){
			UI_SpeakerProc();
			UI_KeyboardProc();
			STDownCnt[ST_UiProc].tick = 0;
		}
		if(STDownCnt[ST_SysMonitor].tick){
			systemMonitor();
			STDownCnt[ST_SysMonitor].tick = 0;
		}

		OUT_Control();
		modeControl();
	}
}
