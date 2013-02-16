#include "lcd.h"
#include "KS0108/KS0108.h"
#include "adc.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

extern ADC_St ADC;

void LCD_Config() {
	GLCD_Initialize();
	GLCD_ClearScreen();

		GLCD_GoTo(0, 0);
		GLCD_WriteString("+--     HELLO     --+");
		GLCD_GoTo(0, 7);
		GLCD_WriteString("+-------------------+");
		GLCD_GoTo(2*6, 7);
		GLCD_WriteString(__DATE__);
}

void LCD_PrintVoltageInfo(int channel) {
	char tempBuf[6];

	toVolt(ADC.milivolt[4], tempBuf);
	GLCD_GoTo(1, 1);
	GLCD_WriteString("Aku: ");
	GLCD_WriteString(tempBuf);

	toVolt(ADC.milivolt[2], tempBuf);
	GLCD_GoTo(1, 2);
	GLCD_WriteString("V24: ");
	GLCD_WriteString(tempBuf);

	toVolt(ADC.milivolt[1], tempBuf);
	GLCD_GoTo(1, 3);
	GLCD_WriteString("V12: ");
	GLCD_WriteString(tempBuf);

	toVolt(ADC.milivolt[0], tempBuf);
	GLCD_GoTo(1, 4);
	GLCD_WriteString("V5:  ");
	GLCD_WriteString(tempBuf);
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
