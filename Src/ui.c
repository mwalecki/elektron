#include "ui.h"
#include "KS0108/KS0108.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "usb.h"
#include "io.h"
#include "central.h"
#include "mb.h"

extern MODBUS_St ModBus;
extern MCENTRAL_St		        MCentral;
extern DEVICE_CONFIGURATION_St  DevConfiguration;
extern DEVICE_DIAGNOSTICS_St  DevDiagnostics;
extern DEVICE_STATE_St        DevState;
extern DEVICE_CONTROL_St      DevControl;

UI_St ui;

void UI_Config() {
	GPIO_InitTypeDef GPIO_InitStructure;

	// Keyboard
	RCC_APB2PeriphClockCmd(KEY_PortAPB2, ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin   =  KEY_Up_Pin | KEY_Down_Pin | KEY_Left_Pin | KEY_Right_Pin | KEY_Off_Pin;
	GPIO_Init(KEY_Port, &GPIO_InitStructure);

	// LCD
	GLCD_Initialize();
	GLCD_ClearScreen();

	GLCD_GoTo(0, 0);GLCD_WriteString("Bt --.-V   usb    o0 ");
	GLCD_GoTo(0, 1);GLCD_WriteString("12 --.-V   link   o1 ");
	GLCD_GoTo(0, 2);GLCD_WriteString("05 --.-V          o2 ");
	GLCD_GoTo(0, 3);GLCD_WriteString("M1 --.-V bitrate  o3 ");
	GLCD_GoTo(0, 4);GLCD_WriteString("M2 --.-V             ");
	GLCD_GoTo(0, 5);GLCD_WriteString("                     ");
	GLCD_GoTo(0, 6);GLCD_WriteString("                     ");
	GLCD_GoTo(0, 7);GLCD_WriteString("+-------------------+");
	GLCD_GoTo(7*6, 7);
	GLCD_WriteString(__DATE__);

	UI_SpeakerConfig();
}

void UI_LcdPrintAnalogs(void) {
	char tempBuf[6];

  itoa(DevConfiguration.serialInterfaceBitrate, tempBuf);
  GLCD_GoTo(LCD_XY_BITRATE);
  GLCD_WriteString(tempBuf);

  toVolt(DevDiagnostics.voltageBatt, tempBuf);
  GLCD_GoTo(LCD_XY_VBATT);
  GLCD_WriteString(tempBuf);

//	toVolt(ADC.milivolt[2], tempBuf);
//	GLCD_GoTo(LCD_XY_V24);
//	GLCD_WriteStringNegative(tempBuf);

	toVolt(DevDiagnostics.voltage12V, tempBuf);
	GLCD_GoTo(LCD_XY_V12);
	GLCD_WriteString(tempBuf);

	toVolt(DevDiagnostics.voltage5V, tempBuf);
	GLCD_GoTo(LCD_XY_V5);
	GLCD_WriteString(tempBuf);

	//itoa(NFComBuf.ReadDeviceVitals.data[4]/1000, tempBuf);
	toVolt(DevDiagnostics.voltagePowerStage1, tempBuf);
	GLCD_GoTo(LCD_XY_VM1);
	GLCD_WriteString(tempBuf);

	//itoa(NFComBuf.ReadDeviceVitals.data[5]/1000, tempBuf);
	toVolt(DevDiagnostics.voltagePowerStage2, tempBuf);
	GLCD_GoTo(LCD_XY_VM2);
	GLCD_WriteString(tempBuf);
}

void UI_LcdPrintBinaries(void) {
	static uint8_t oldCursPos=0,  firstRun=1;

	GLCD_GoTo(LCD_XY_USB);
	USB_IsConfigured() ?
			GLCD_WriteStringNegative("usb") : GLCD_WriteString("usb");
	GLCD_GoTo(LCD_XY_LINK);
	MCentral.computerLink ?
			GLCD_WriteStringNegative("link") : GLCD_WriteString("link");
	GLCD_GoTo(LCD_XY_O0);
	(DevControl.digitalOutputs & (1<<0)) ?
			GLCD_WriteStringNegative("o0") : GLCD_WriteString("o0");
	GLCD_GoTo(LCD_XY_O1);
	(DevControl.digitalOutputs & (1<<1)) ?
			GLCD_WriteStringNegative("o1") : GLCD_WriteString("o1");
	GLCD_GoTo(LCD_XY_O2);
	(DevControl.digitalOutputs & (1<<2)) ?
			GLCD_WriteStringNegative("o2") : GLCD_WriteString("o2");
	GLCD_GoTo(LCD_XY_O3);
	(DevControl.digitalOutputs & (1<<3)) ?
			GLCD_WriteStringNegative("o3") : GLCD_WriteString("o3");

	GLCD_GoTo(LCD_XY_MESSAGE0);
	(MCentral.batteryLow) ?
			GLCD_WriteStringNegative(" battery low ") : GLCD_WriteString("             ");
	GLCD_GoTo(LCD_XY_MESSAGE1);
	(MCentral.shutdownCounter) ?
			GLCD_WriteStringNegative("shutting down") : GLCD_WriteString("             ");


	GLCD_GoTo(LCD_XY_PS);
	(DevControl.mode) ?
			GLCD_WriteStringNegative("PS") : GLCD_WriteString("PS");

	if(firstRun)
		firstRun = 0;
	else if(ui.cursorY == oldCursPos)
		return;

	oldCursPos = ui.cursorY;

	GLCD_GoTo(LCD_XY_CUR0);
	(ui.cursorY == 0) ?
			GLCD_WriteStringNegative(" ") : GLCD_WriteString(" ");

	GLCD_GoTo(LCD_XY_CUR1);
	(ui.cursorY == 1) ?
			GLCD_WriteStringNegative(" ") : GLCD_WriteString(" ");

	GLCD_GoTo(LCD_XY_CUR2);
	(ui.cursorY == 2) ?
			GLCD_WriteStringNegative(" ") : GLCD_WriteString(" ");

	GLCD_GoTo(LCD_XY_CUR3);
	(ui.cursorY == 3) ?
			GLCD_WriteStringNegative(" ") : GLCD_WriteString(" ");
}

void UI_KeyboardProc(void)
{
	uint16_t keyPort;
	uint8_t i;
	uint8_t resetCombinationPressed = 1;

	keyPort = GPIO_ReadInputData(KEY_Port);
	ui.keyPressed[KEY_Up] = ((keyPort & KEY_Up_Pin) == 0);
	ui.keyPressed[KEY_Down] = ((keyPort & KEY_Down_Pin) == 0);
	ui.keyPressed[KEY_Left] = ((keyPort & KEY_Left_Pin) == 0);
	ui.keyPressed[KEY_Right] = ((keyPort & KEY_Right_Pin) == 0);
	ui.keyPressed[KEY_Off] = ((keyPort & KEY_Off_Pin) == 0);

  if(    ui.keyPressed[KEY_Up]
        && ui.keyPressed[KEY_Down]
        && ui.keyPressed[KEY_Left]
        && ui.keyPressed[KEY_Right]){
    ui.keyCnt[KEY_Up]++;
    ui.keyCnt[KEY_Down]++;
    ui.keyCnt[KEY_Left]++;
    ui.keyCnt[KEY_Right]++;
  }
  else {
    if(ui.keyPressed[KEY_Up] && !ui.keyPressed[KEY_Down])
      ui.keyCnt[KEY_Up]++;
    else
    {
      ui.keyCnt[KEY_Up] = 0;
      ui.keyHold[KEY_Up] = 2;
    }

    if(ui.keyPressed[KEY_Down] && !ui.keyPressed[KEY_Up])
      ui.keyCnt[KEY_Down]++;
    else
    {
      ui.keyCnt[KEY_Down] = 0;
      ui.keyHold[KEY_Down] = 2;
    }

    if(ui.keyPressed[KEY_Left] && !ui.keyPressed[KEY_Right])
      ui.keyCnt[KEY_Left]++;
    else
    {
      ui.keyCnt[KEY_Left] = 0;
      ui.keyHold[KEY_Left] = 2;
    }

    if(ui.keyPressed[KEY_Right] && !ui.keyPressed[KEY_Left])
      ui.keyCnt[KEY_Right]++;
    else
    {
      ui.keyCnt[KEY_Right] = 0;
      ui.keyHold[KEY_Right] = 2;
    }

    if(ui.keyPressed[KEY_Off])
      ui.keyCnt[KEY_Off]++;
    else
    {
      ui.keyCnt[KEY_Off] = 0;
      ui.keyHold[KEY_Off] = 2;
    }
  }

	for(i = 0; i < KEYS_N; i++)
	{
		if((ui.keyHold[i]==0) && ui.keyCnt[i]>=30)
			ui.keyCnt[i]=0;
		else if(ui.keyCnt[i]>=75)
			ui.keyCnt[i]=0;

		if(ui.keyCnt[i]==5)
		{
			switch(i)
			{
			case KEY_Up:
				if(ui.cursorY > 0)
					ui.cursorY--;
				else
					ui.cursorY = UI_CURSOR_YMAX;
				if(ui.keyHold[KEY_Up] > 0)
					ui.keyHold[KEY_Up] --;
				UI_SND_KEY_UP;
				break;
			case KEY_Down:
				if(ui.cursorY < UI_CURSOR_YMAX)
					ui.cursorY++;
				else
					ui.cursorY = 0;
				if(ui.keyHold[KEY_Down] > 0)
					ui.keyHold[KEY_Down] --;
				UI_SND_KEY_DOWN;
				break;
			case KEY_Left:
				if(ui.keyHold[KEY_Left]!=0)
				{
					switch(ui.cursorY){
					case 0:
						if((DevControl.digitalOutputs & (1<<0)) != 0){
							DevControl.digitalOutputs &= ~(1<<0);
							UI_SND_DEACTIVATE;
						}
						break;
					case 1:
						if((DevControl.digitalOutputs & (1<<1)) != 0){
							DevControl.digitalOutputs &= ~(1<<1);
							UI_SND_DEACTIVATE;
						}
						break;
					case 2:
						if((DevControl.digitalOutputs & (1<<2)) != 0){
							DevControl.digitalOutputs &= ~(1<<2);
							UI_SND_DEACTIVATE;
						}
						break;
					case 3:
						if((DevControl.digitalOutputs & (1<<3)) != 0){
							DevControl.digitalOutputs &= ~(1<<3);
							UI_SND_DEACTIVATE;
						}
						break;
					}
					ui.keyHold[KEY_Left] = 0;
				}
				break;
			case KEY_Right:
				if(ui.keyHold[KEY_Right]!=0)
				{
					switch(ui.cursorY){
					case 0:
						if((DevControl.digitalOutputs & (1<<0)) == 0){
							DevControl.digitalOutputs |= (1<<0);
							UI_SND_ACTIVATE;
						}
						break;
					case 1:
						if((DevControl.digitalOutputs & (1<<1)) == 0){
							DevControl.digitalOutputs |= (1<<1);
							UI_SND_ACTIVATE;
						}
						break;
					case 2:
						if((DevControl.digitalOutputs & (1<<2)) == 0){
							DevControl.digitalOutputs |= (1<<2);
							UI_SND_ACTIVATE;
						}
						break;
					case 3:
						if((DevControl.digitalOutputs & (1<<3)) == 0){
							DevControl.digitalOutputs |= (1<<3);
							UI_SND_ACTIVATE;
						}
						break;
					}
					ui.keyHold[KEY_Right] = 0;
				}
				break;
			case KEY_Off:
				//UI_SND_POWER_OFF;
				break;
			}
		}
	}
	if(    ui.keyHold[KEY_Left] == 0
	    && ui.keyHold[KEY_Right] == 0
	    && ui.keyHold[KEY_Up] == 0
	    && ui.keyHold[KEY_Down] == 0){
	  UI_SND_WARNING;
	  eebackup_ResetCommunicationSettings();
	  eebackup_SaveAll();

    ui.keyHold[KEY_Left] = 2;
    ui.keyHold[KEY_Right] = 2;
    ui.keyHold[KEY_Up] = 2;
    ui.keyHold[KEY_Down] = 2;
	}
}

void UI_SpeakerConfig(void){
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* GPIOC */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	/*GPIO Configuration: TIM3 channel1, 2, 3 and 4 */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(72) - 1; // counter clk = 1MHz
	TIM_TimeBaseStructure.TIM_Period = 65535;	// for 1kHz test sound (TIM3->ARR)
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	  /* Output Compare Toggle Mode configuration: Channel1 */
	  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_Pulse = 1000;
	  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	  TIM_OC2Init(TIM3, &TIM_OCInitStructure);


	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);

	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);

	UI_SND_POWER_UP;
}

void UI_SpeakerPlay(uint32_t hertz, uint16_t milisec){
	if(ui.lsSndCnt < LS_MAX_NOTES){
		ui.lsSound[ui.lsSndCnt] = hertz;
		ui.lsSndDuration[ui.lsSndCnt] = milisec/10;
		ui.lsSndCnt ++;
	}
}

void UI_SpeakerProc(){
	static uint16_t timeLeft = 0;
	uint8_t i;

	if(timeLeft == 0){
		if(ui.lsSndCnt > 0){
			if(ui.lsSound[0] > 0){
				LS_PERIOD = (uint16_t)(1000000 / ui.lsSound[0]);
				LS_DUTY = (uint16_t)((1000000 / ui.lsSound[0])/2);
				TIM_GenerateEvent(TIM3, TIM_EventSource_Update);
			}
			else{
				LS_PERIOD = 0;
				TIM_GenerateEvent(TIM3, TIM_EventSource_Update);
			}
			timeLeft = ui.lsSndDuration[0];
			for(i=0; i<(ui.lsSndCnt-1); i++){
				ui.lsSound[i] = ui.lsSound[i+1];
				ui.lsSndDuration[i] = ui.lsSndDuration[i+1];
			}
			ui.lsSndCnt --;
		}
		else
			LS_PERIOD = 0;
			TIM_GenerateEvent(TIM3, TIM_EventSource_Update);
	}
	else
		timeLeft --;
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






