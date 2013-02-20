#include "usb.h"
#include "circbuf.h"
#include "myscpi/myscpi.h"
#include "nf/nfv2.h"

uint8_t cbUSBRxData[CB_USBRX_BUF_SIZE];
CircularBuffer	cbUSBReceived;
extern USB_St	USBMySCPI, USBNF;
extern NF_STRUCT_ComBuf 	NFComBuf;


extern uint8_t USB_Rx_Buffer[VIRTUAL_COM_PORT_DATA_SIZE];
extern uint16_t USB_Rx_Cnt;
extern  uint8_t USART_Rx_Buffer[];
extern uint32_t USART_Rx_ptr_in;

void USB_Config(void){
	// USB Received Data Circular Buffer Init
	cbInit(&cbUSBReceived, cbUSBRxData, CB_USBRX_BUF_SIZE);

#ifdef USB_CONNECT_PORT
	GPIO_InitTypeDef GPIO_InitStructure;
	// IO Clocks Enable
	RCC_APB2PeriphClockCmd(USB_CONNECT_APB, ENABLE);
	/*	USB_CONNECT_PORT Push-Pull 10MHz Outputs:	*\
		USB_CONNECT_PIN	USB_PullUp					*/
	GPIO_InitStructure.GPIO_Pin = USB_CONNECT_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(USB_CONNECT_PORT, &GPIO_InitStructure);
#endif

#ifdef STM32F10X_CL
	USB_OTG_BSP_uDelay_Configure();
#endif

	// USB Init
	Set_USBClock();
	USB_Interrupts_Config();			
	USB_Init();

#ifdef USB_CONNECT_PORT
	// Enable USBDP PullUp
	USB_Cable_Config(ENABLE);
#endif
}

void USB_CacheReceivedData(uint8_t* data_buffer, uint8_t Nb_bytes) {
	uint16_t i;
	for (i = 0; i < Nb_bytes; i++) {
		cbWrite(&cbUSBReceived, &(data_buffer[i]));
	}
}

void USB_ProcessReceivedData(void) {
	uint8_t newByte;
	uint8_t commArray[15], commCnt;
	while(cbIsEmpty(&cbUSBReceived) == 0){
		cbRead(&cbUSBReceived, &newByte);

		// MySCPI
		USBMySCPI.rxBuf[USBMySCPI.rxPt] = newByte;
		if(MYSCPI_Interpreter(USBMySCPI.rxBuf, &USBMySCPI.rxPt, USBMySCPI.txBuf, &USBMySCPI.txCnt) > 0){
			if(USBMySCPI.txCnt > 0){
				USB_SendNBytes((uint8_t*)USBMySCPI.txBuf, USBMySCPI.txCnt);
				USBMySCPI.txCnt = 0;
			}
		}

		// NF
		USBNF.rxBuf[USBNF.rxPt] = newByte;
		if(NF_Interpreter(&NFComBuf, (uint8_t*)USBNF.rxBuf, (uint8_t*)&USBNF.rxPt, commArray, &commCnt) > 0){
			NFComBuf.dataReceived = 1;
			if(commCnt > 0){
				USBNF.txCnt = NF_MakeCommandFrame(&NFComBuf, (uint8_t*)USBNF.txBuf, (const uint8_t*)commArray, commCnt, NFComBuf.myAddress);
				USB_SendNBytes((uint8_t*)USBNF.txBuf, USBNF.txCnt);
			}
		}
	}
}

void USB_SendNBytes(uint8_t* buf, uint16_t cnt){
	uint16_t i;
	for(i = 0; i < cnt; i++){
		USART_Rx_Buffer[USART_Rx_ptr_in] = buf[i];
		USART_Rx_ptr_in++;
		/* To avoid buffer overflow */
		if(USART_Rx_ptr_in == USART_RX_DATA_SIZE)
			USART_Rx_ptr_in = 0;
	}
}

uint8_t USB_ReadOneByte(void){
	uint8_t ret = 0;
	uint32_t i;
	if(USB_Rx_Cnt > 0){
		ret = USB_Rx_Buffer[0];
		for(i=1; i < USB_Rx_Cnt; i++)
			USB_Rx_Buffer[i-1] = USB_Rx_Buffer[i];
		USB_Rx_Cnt --;
	}
	return ret;
}

uint8_t USB_RxBufNotEmpty(void){
	return (USB_Rx_Cnt > 0) ? 1 : 0;
}
