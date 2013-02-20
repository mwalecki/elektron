#include "lcd.h"
#include "KS0108/KS0108.h"
#include "adc.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "nf/nfv2.h"
#include "usb.h"

extern ADC_St ADC;
extern NF_STRUCT_ComBuf 	NFComBuf;
extern DEVICE_INFO *pInformation;

void LCD_Config() {
	GLCD_Initialize();
	GLCD_ClearScreen();

		GLCD_GoTo(0, 0);
		GLCD_WriteString("+- ElektroSedes ----+");
		GLCD_GoTo(0, 7);
		GLCD_WriteString("+-              ----+");
		GLCD_GoTo(3*6, 7);
		GLCD_WriteString(__DATE__);
}

void LCD_PrintVoltageInfo(int channel) {
	char tempBuf[6];

	toVolt(ADC.milivolt[4], tempBuf);
	GLCD_GoTo(0, 1);
	GLCD_WriteString("Aku: ");
	GLCD_WriteString(tempBuf);

	toVolt(ADC.milivolt[2], tempBuf);
	GLCD_GoTo(0, 2);
	GLCD_WriteString("V24: ");
	GLCD_WriteStringNegative(tempBuf);

	toVolt(ADC.milivolt[1], tempBuf);
	GLCD_GoTo(0, 3);
	GLCD_WriteString("V12: ");
	GLCD_WriteString(tempBuf);

	toVolt(ADC.milivolt[0], tempBuf);
	GLCD_GoTo(0, 4);
	GLCD_WriteString("V5:  ");
	GLCD_WriteString(tempBuf);
}

void LCD_OutputsMenuProcess(void) {
	
	char tempBuf[10];
	static uint8_t cursPos=0, oldCursPos=0,  firstRun=1;

	GLCD_GoTo(0*6, 6);
	itoa(pInformation->ControlState, tempBuf);
	GLCD_WriteString(tempBuf);
	GLCD_GoTo(4*6, 6);
	itoa(pInformation->Current_Configuration, tempBuf);
	GLCD_WriteString(tempBuf);
	

	GLCD_GoTo(16*6, 1);
	(NFComBuf.SetDigitalOutputs.data[0] & (1<<0)) ?
			GLCD_WriteStringNegative("Z.0") : GLCD_WriteString("Z.0");
	GLCD_GoTo(16*6, 2);
	(NFComBuf.SetDigitalOutputs.data[0] & (1<<1)) ?
			GLCD_WriteStringNegative("Z.1") : GLCD_WriteString("Z.1");
	GLCD_GoTo(16*6, 3);
	(NFComBuf.SetDigitalOutputs.data[0] & (1<<2)) ?
			GLCD_WriteStringNegative("Z.2") : GLCD_WriteString("Z.2");
	GLCD_GoTo(16*6, 4);
	(NFComBuf.SetDigitalOutputs.data[0] & (1<<3)) ?
			GLCD_WriteStringNegative("Z.3") : GLCD_WriteString("Z.3");
	GLCD_GoTo(16*6, 5);
	(NFComBuf.SetDrivesMode.data[0] && NFComBuf.SetDrivesMode.data[1]) ?
			GLCD_WriteStringNegative("Moc") : GLCD_WriteString("Moc");
	
	if(firstRun)
		firstRun = 0;
	else if(cursPos == oldCursPos)
		return;
	
	cursPos = oldCursPos;
	
	GLCD_GoTo(20*6, 1);
	(cursPos == 0) ? 
			GLCD_WriteStringNegative(" ") : GLCD_WriteString(" ");

	GLCD_GoTo(20*6, 2);
	(cursPos == 1) ? 
			GLCD_WriteStringNegative(" ") : GLCD_WriteString(" ");

	GLCD_GoTo(20*6, 3);
	(cursPos == 2) ? 
			GLCD_WriteStringNegative(" ") : GLCD_WriteString(" ");

	GLCD_GoTo(20*6, 4);
	(cursPos == 3) ? 
			GLCD_WriteStringNegative(" ") : GLCD_WriteString(" ");

	GLCD_GoTo(20*6, 5);
	(cursPos == 4) ? 
			GLCD_WriteStringNegative(" ") : GLCD_WriteString(" ");
}

/* reverse:  reverse string s in place */
void reverse(char s[]) {
	int c, i, j;

	for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

/* itoa:  convert n to characters in s */
void itoa(int n, char s[]) {
	int i;

	i = 0;
	do { /* generate digits in reverse order */
		s[i++] = n % 10 + '0'; /* get next digit */
	} while ((n /= 10) > 0); /* delete it */

	s[i] = '\0';
	reverse(s);
}

void toVolt(int milivolt, char s[]) {
	int i;
	char tmp[5];
	itoa(milivolt, tmp);
	i = strlen(tmp);
	if (i >= 3) {
		if (i == 5) {
			s[0] = tmp[0];
			s[1] = tmp[1];
			s[2] = '.';
			s[3] = tmp[2];
		} else if (i == 4) {
			s[0] = ' ';
			s[1] = tmp[0];
			s[2] = '.';
			s[3] = tmp[1];
		} else {
			s[0] = ' ';
			s[1] = '0';
			s[2] = '.';
			s[3] = tmp[0];
		}
	} else {
		s[0] = ' ';
		s[1] = '0';
		s[2] = '.';
		s[3] = '0';
	}
	s[4] = 'V';
	s[5] = '\0';
}
