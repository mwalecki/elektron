#ifndef MYUSB_H
#define MYUSB_H

#include "usb_lib.h"
#include "usb_desc.h"
#include "hw_config.h"
#include "usb_pwr.h"

//#define USB_CONNECT_PORT 	GPIOC
//#define USB_CONNECT_PIN 	GPIO_Pin_9
//#define USB_CONNECT_APB		RCC_APB2Periph_GPIOC

#define CB_USBRX_BUF_SIZE 512

#define USB_BufSz 256

typedef struct{
	vu8 rxBuf[USB_BufSz];
	vu8 txBuf[USB_BufSz];
	vu8 tmpBuf[USB_BufSz];
	vu8 rxPt, txPt, txCnt;
	volatile uint8_t rxDataReady	:1;
	volatile uint8_t txDataReady	:1;
} USB_St;


void USB_Config(void);
uint8_t USB_IsConfigured(void);
void USB_SendNBytes(uint8_t* buf, uint16_t cnt);
uint8_t USB_ReadOneByte(void);
uint8_t USB_RxBufNotEmpty(void);
void USB_CacheReceivedData(uint8_t* data_buffer, uint8_t Nb_bytes);
void USB_ProcessReceivedData(void);

#endif //MYUSB_H
