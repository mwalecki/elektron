#ifndef _COMMON_H_
#define _COMMON_H_

#include <inttypes.h>
#include "stm32f10x_conf.h"

#define MODULE_NAME			"MW1106 Sedes Main Module mw@mwalecki.pl"
#define USART_BufSz			256	 
#define USART_TxSuffix		"\r\n\n"

#define M_STOP		      0
#define M_SMOOTH_STOP	1
#define M_ACTIVE        2

#define SERIAL_DEF_BITRATE  115200
#define SERIAL_DEF_ADDRESS  1

typedef struct{
	vu8 blink;
	vu8 blinkPhase;
} LED_St;

typedef struct{
  vu32 val;
  vu32 period;
  vu8 tick;
} STDOWNCNT_St;

typedef struct{
  uint16_t deviceInfo_L;
  uint16_t deviceInfo_H;
  uint16_t deviceStatus;
  uint16_t voltage5V;
  uint16_t voltage12V;
  uint16_t voltage24V;
  uint16_t voltageBatt;
  uint16_t voltagePowerStage1;
  uint16_t voltagePowerStage2;
} DEVICE_DIAGNOSTICS_St;

typedef struct{
  uint16_t status_L;
  uint16_t status_H;
  uint16_t digitalInputs;
} DEVICE_STATE_St;

typedef struct{
  uint16_t mode;
  uint16_t digitalOutputs;
} DEVICE_CONTROL_St;

typedef struct{
  uint16_t serialNumber;
  uint16_t dataLinkTimeout;
  uint16_t motorPoweroffScheme;
  uint16_t MotorPoweroffDelay;
  uint16_t batteryVoltageLow;
  uint16_t batteryVoltageCritical;
  uint16_t shutdownDelay;
  uint32_t serialInterfaceBitrate;
  uint32_t mileage;
} DEVICE_CONFIGURATION_St;

typedef struct{
  uint32_t bitrate;
	u8 rxBuf[USART_BufSz];
	u8 txBuf[USART_BufSz];
	u8 tmpBuf[USART_BufSz];
	u8 rxPt, txPt, txCnt;
	volatile uint8_t rxDataReady	:1;
	volatile uint8_t txDataReady	:1;
} USART_St;	

typedef struct{
	u8 mode;
	s8 leftSpeed, rightSpeed;
	s8 srv4;
	u16 srv1pos, srv2pos, srv3pos, srv4pos;
	u8 digiOut;
	u16 slope;
	s16 enc1, enc2;
	u16	pidP;
	u16	pidI;
	u16	pidD;
	u16 pidT;
	u16 srv1Min;
	u16 srv1Max;
	u16 srv2Min;
	u16 srv2Max;
} REFERENCE_St;

typedef struct{
	u8	mode;
	s16	refSpeed;
} MMOTOR_St;

typedef struct{
	u16	dist[8];
} MSENSE_St;

#endif //_COMMON_H_
