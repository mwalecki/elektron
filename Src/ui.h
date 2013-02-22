#ifndef _LCD_H
#define _LCD_H

#include "common.h"

//##                                      #### ######## ################ DEFINES:
#define LCD_XY_VBATT	3*6,0
//#define LCD_XY_V24		4*6,2
#define LCD_XY_V12		3*6,1
#define LCD_XY_V5		3*6,2
#define LCD_XY_VM1		3*6,3
#define LCD_XY_VM2		3*6,4
#define LCD_XY_TM1		9*6,3
#define LCD_XY_TM2		9*6,4
#define LCD_XY_SM1		13*6,3
#define LCD_XY_SM2		13*6,4
#define LCD_XY_EM1		17*6,3
#define LCD_XY_EM2		17*6,4
#define LCD_XY_USB		11*6,0
#define LCD_XY_LINK		11*6,1
#define LCD_XY_CERR		11*6,2
#define LCD_XY_O0		18*6,0
#define LCD_XY_O1		18*6,1
#define LCD_XY_O2		18*6,2
#define LCD_XY_O3		18*6,3
#define LCD_XY_CUR0		20*6,0
#define LCD_XY_CUR1		20*6,1
#define LCD_XY_CUR2		20*6,2
#define LCD_XY_CUR3		20*6,3

#define KEY_Up		0
#define KEY_Down	1
#define KEY_Left	2
#define KEY_Right	3
#define KEY_Off		4

#define KEY_Port		GPIOE
#define KEY_PortAPB2	RCC_APB2Periph_GPIOE
#define KEY_Up_Pin		(1<<2)
#define KEY_Down_Pin	(1<<3)
#define KEY_Left_Pin	(1<<5)
#define KEY_Right_Pin	(1<<6)
#define KEY_Off_Pin		(1<<4)

#define KEYPRESSED(k) ((keyPort & (1<<k)) != 0)

#define KEYS_N 5
#define UI_CURSOR_YMAX 3

#define LS_PERIOD	TIM3->ARR
#define LS_DUTY		TIM3->CCR2

#define LS_MAX_NOTES	32

// Frequencies for equal-tempered scale
// http://www.phy.mtu.edu/~suits/notefreqs.html
#define LS_PAUSE 	0
#define LS_noteC3	131
#define LS_noteC3x	139
#define LS_noteD3	147
#define LS_noteD3x	156
#define LS_noteE3	165
#define LS_noteF3	175
#define LS_noteF3x	185
#define LS_noteG3	196
#define LS_noteG3x	208
#define LS_noteA3	220
#define LS_noteA3x	233
#define LS_noteH3	247
#define LS_noteC4	262
#define LS_noteC4x	277
#define LS_noteD4	294
#define LS_noteD4x	311
#define LS_noteE4	330
#define LS_noteF4	350
#define LS_noteF4x	370
#define LS_noteG4	392
#define LS_noteG4x	415
#define LS_noteA4	440
#define LS_noteA4x	466
#define LS_noteH4	494
#define LS_noteC5	523
#define LS_noteC5x	554
#define LS_noteD5	587
#define LS_noteD5x	622
#define LS_noteE5	659
#define LS_noteF5	698
#define LS_noteF5x	740
#define LS_noteG5	784
#define LS_noteG5x	831
#define LS_noteA5	880
#define LS_noteA5x	932
#define LS_noteH5	988
#define LS_noteC6	1047
#define LS_noteC6x	1109
#define LS_noteD6	1175
#define LS_noteD6x	1245
#define LS_noteE6	1319
#define LS_noteF6	1397
#define LS_noteF6x	1480
#define LS_noteG6	1568
#define LS_noteG6x	1661
#define LS_noteA6	1760
#define LS_noteA6x	1865
#define LS_noteH6	1976
#define LS_noteC7	2093

//##                                      #### ######## ################ TYPEDEFS:

typedef struct{
	uint8_t keyPressed[KEYS_N];
	uint8_t keyCnt[KEYS_N];
	uint8_t keyHold[KEYS_N];
	uint8_t cursorY;
	uint16_t lsSound[LS_MAX_NOTES];
	uint16_t lsSndDuration[LS_MAX_NOTES];
	uint8_t lsSndCnt;
} UI_St;

//##                                      #### ######## ################ PROTOTYPES:

void UI_Config(void);
void UI_LcdPrintAnalogs(void);
void UI_LcdPrintBinaries(void);
void UI_KeyboardProc(void);
void UI_SpeakerConfig(void);
void UI_SpeakerPlay(uint32_t hertz, uint16_t milisec);
void UI_SpeakerProc();
void itoa(int n, char s[]);
void toVolt(int milivolt, char s[]);
void reverse(char s[]);

#endif
