#ifndef _LCD_H
#define _LCD_H

#include "common.h"

//##                                      #### ######## ################ DEFINES:
#define LCD_XY_VBATT	4*6,1
//#define LCD_XY_V24		4*6,2
#define LCD_XY_V12		4*6,2
#define LCD_XY_V5		4*6,3
#define LCD_XY_VM1		3*6,5
#define LCD_XY_VM2		3*6,6
#define LCD_XY_TM1		9*6,5
#define LCD_XY_TM2		9*6,6
#define LCD_XY_SM1		13*6,5
#define LCD_XY_SM2		13*6,6
#define LCD_XY_EM1		17*6,5
#define LCD_XY_EM2		17*6,6
#define LCD_XY_USB		10*6,0
#define LCD_XY_LINK		10*6,1
#define LCD_XY_CERR		10*6,2
#define LCD_XY_O0		18*6,0
#define LCD_XY_O1		18*6,1
#define LCD_XY_O2		18*6,2
#define LCD_XY_O3		18*6,3
#define LCD_XY_CUR0		20*6,1
#define LCD_XY_CUR1		20*6,2
#define LCD_XY_CUR2		20*6,3
#define LCD_XY_CUR3		20*6,4

//##                                      #### ######## ################ PROTOTYPES:

void LCD_Config(void);
void printChannelInfo(int channel);
void itoa(int n, char s[]);
void toVolt(int milivolt, char s[]);
void reverse(char s[]);

#endif
