#include "myscpi/myscpi.h"
#include "common.h"		
#include "led.h"  	
#include "io.h"
#include "adc.h"
#include "nf/nfv2.h"
#include "eebackup.h"
#include "central.h"

extern uint16_t 	serialNumber;
extern NF_STRUCT_ComBuf 	NFComBuf;
extern uint8_t MotorControllerAddr0, MotorControllerAddr1, InputOutputAddr0;
extern MCENTRAL_St			MCentral;

uint8_t MYSCPI_Interpreter(volatile uint8_t *rxBuf, volatile uint8_t *rxPt, volatile uint8_t *txBuf, volatile uint8_t *txCnt)
{
	// Do not modify this section
	//	||	||	||	||	||	||	||
	//	\/	\/	\/	\/	\/	\/	\/
	uint8_t grNameLen = 0;
	uint8_t memNameLen = 0;
	uint8_t ret = 0;

	*txCnt = 0;
	
	// Wrong start byte
	if((*rxPt) == 0 && rxBuf[0]!=':' && rxBuf[0]!='*'){
		return 0;
	}
	// Start byte or data received
	else if(rxBuf[*rxPt]!='\r' && rxBuf[*rxPt]!='\n'){
		// Pointer increment, Buffer size overflow check
		if(++(*rxPt) >= RXBUFSZ){
			(*rxPt) = 0;
		}
		return 0;
	}
	// End byte '\r' or '\n' received.
	else{
		// Increment rxPt to avoid rxBuf modification during interpretation
		(*rxPt)++;
	}
	//	/\	/\	/\	/\	/\	/\	/\
	//	||	||	||	||	||	||	||
	// Do not modify this section

	
	
	// Here you build your own command parser
	//	||	||	||	||	||	||	||
	//	\/	\/	\/	\/	\/	\/	\/

	_IF_MEMBER_THEN("*IDN?")
	_PRINT_STR_RESPONSE(MODULE_NAME)

	else
	_GROUP(":ADDR")
		_GET_SET_MEMBER(MotorControllerAddr0, ":MC0")
		else
		_GET_SET_MEMBER(MotorControllerAddr1, ":MC1")
		else
		_GET_SET_MEMBER(InputOutputAddr0, ":IO0")
	_ENDGROUP

	else
	_GROUP(":BATT")
		_GET_SET_MEMBER(MCentral.batteryVoltageLow, ":LOW")
		else
		_GET_SET_MEMBER(MCentral.batteryVoltageCritical, ":CRIT")
	_ENDGROUP

	else
	_GROUP(":MEM") 
		_IF_MEMBER_THEN(":DEF")
			eebackup_SaveInitialValues();
		else
		_IF_MEMBER_THEN(":STOR")
			eebackup_SaveAll();
		else
		_IF_MEMBER_THEN(":REC")
			eebackup_Recover();
	_ENDGROUP

	else
	_GROUP(":LED") 
		_IF_MEMBER_THEN(":SET")
			switch(rxBuf[9]){
				case '0':
					LED_Set(1<<0, 1<<0, 0);
					break;
				case '1':
					LED_Set(1<<1, 1<<1, 0);
					break;
				case '2':
					LED_Set(1<<2, 1<<2, 0);
					break;
			}
		else  
		_IF_MEMBER_THEN(":RES")
			switch(rxBuf[9]){
				case '0':
					LED_Set(1<<0, 0, 0);
					break;
				case '1':
					LED_Set(1<<1, 0, 0);
					break;
				case '2':
					LED_Set(1<<2, 0, 0);
					break;
			}
	_ENDGROUP

	//	/\	/\	/\	/\	/\	/\	/\
	//	||	||	||	||	||	||	||
	// Here you build your own command parser
	
	
	// Do not modify this section
	//	||	||	||	||	||	||	||
	//	\/	\/	\/	\/	\/	\/	\/
	ret = *rxPt;
	// Reset rxPt for next incoming command
	*rxPt = 0;
	// Return length of received command
	return ret;
}

/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 * Modified to return string length
 */
int my_itoa(int value, char* result, int base) {
	int len = 0;
	// check that the base if valid
	if (base < 2 || base > 36) {
		*result = '\0';
		return 0;
	}

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );

	// Apply negative sign
	if (tmp_value < 0)
		*ptr++ = '-';
	*(ptr) = '\0';
	ptr--;
	// calculate string length
	len = ptr +1 - result;

	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return len;
}
