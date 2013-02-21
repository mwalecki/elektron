#include "lcd.h"
#include "KS0108/KS0108.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "nf/nfv2.h"
#include "usb.h"

extern NF_STRUCT_ComBuf 	NFComBuf;

void LCD_Config() {
	GLCD_Initialize();
	GLCD_ClearScreen();

	GLCD_GoTo(0, 0);GLCD_WriteString("Power[V]  usb     o0 ");
	GLCD_GoTo(0, 1);GLCD_WriteString("Aku --.-  link    o1 ");
	GLCD_GoTo(0, 2);GLCD_WriteString("V12 --.-          o2 ");
	GLCD_GoTo(0, 3);GLCD_WriteString("V5  --.-          o3 ");
	GLCD_GoTo(0, 4);GLCD_WriteString("                     ");
	GLCD_GoTo(0, 5);GLCD_WriteString("M1 --.-V --C ---     ");
	GLCD_GoTo(0, 6);GLCD_WriteString("M2 --.-V --C ---     ");
	GLCD_GoTo(0, 7);GLCD_WriteString("---------------------");
	GLCD_GoTo(10*6, 7);
	GLCD_WriteString(__DATE__);
}

void LCD_PrintAnalogs(int channel) {
	char tempBuf[6];

	toVolt(NFComBuf.ReadDeviceVitals.data[0], tempBuf);
	GLCD_GoTo(LCD_XY_VBATT);
	GLCD_WriteString(tempBuf);

//	toVolt(ADC.milivolt[2], tempBuf);
//	GLCD_GoTo(LCD_XY_V24);
//	GLCD_WriteStringNegative(tempBuf);

	toVolt(NFComBuf.ReadDeviceVitals.data[2], tempBuf);
	GLCD_GoTo(LCD_XY_V12);
	GLCD_WriteString(tempBuf);

	toVolt(NFComBuf.ReadDeviceVitals.data[3], tempBuf);
	GLCD_GoTo(LCD_XY_V5);
	GLCD_WriteString(tempBuf);

	//itoa(NFComBuf.ReadDeviceVitals.data[4]/1000, tempBuf);
	toVolt(NFComBuf.ReadDeviceVitals.data[4], tempBuf);
	GLCD_GoTo(LCD_XY_VM1);
	GLCD_WriteString(tempBuf);

	//itoa(NFComBuf.ReadDeviceVitals.data[5]/1000, tempBuf);
	toVolt(NFComBuf.ReadDeviceVitals.data[5], tempBuf);
	GLCD_GoTo(LCD_XY_VM2);
	GLCD_WriteString(tempBuf);
}

void LCD_OutputsMenuProcess(void) {
	
	char tempBuf[10];
	static uint8_t cursPos=0, oldCursPos=0,  firstRun=1;

	GLCD_GoTo(LCD_XY_USB);
	USB_IsConfigured() ?
			GLCD_WriteStringNegative("usb") : GLCD_WriteString("usb");

	GLCD_GoTo(LCD_XY_O0);
	(NFComBuf.SetDigitalOutputs.data[0] & (1<<0)) ?
			GLCD_WriteStringNegative("o0") : GLCD_WriteString("o0");
	GLCD_GoTo(LCD_XY_O1);
	(NFComBuf.SetDigitalOutputs.data[0] & (1<<1)) ?
			GLCD_WriteStringNegative("o1") : GLCD_WriteString("o1");
	GLCD_GoTo(LCD_XY_O2);
	(NFComBuf.SetDigitalOutputs.data[0] & (1<<2)) ?
			GLCD_WriteStringNegative("o2") : GLCD_WriteString("o2");
	GLCD_GoTo(LCD_XY_O3);
	(NFComBuf.SetDigitalOutputs.data[0] & (1<<3)) ?
			GLCD_WriteStringNegative("o3") : GLCD_WriteString("o3");




	GLCD_GoTo(LCD_XY_SM1);
	(NFComBuf.SetDrivesMode.data[0]) ?
			GLCD_WriteStringNegative("on ") : GLCD_WriteString("off");
	GLCD_GoTo(LCD_XY_SM2);
	(NFComBuf.SetDrivesMode.data[1]) ?
			GLCD_WriteStringNegative("on ") : GLCD_WriteString("off");
	
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
	s[4] = '\0';
}
