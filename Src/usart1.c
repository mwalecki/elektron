#include "usart1.h"
#include "circbuf.h"

extern USART_St		Usart1;

uint8_t cbUsart1RxData[256];
extern CircularBuffer	cbUsart1Received;

void USART1_Config(USART_InitTypeDef USART_InitStructure){
	GPIO_InitTypeDef GPIO_InitStructure;  
    USART_ClockInitTypeDef  USART_ClockInitStructure;

	// Usart Received Data Circular Buffer Init
	cbInit(&cbUsart1Received, cbUsart1RxData, 256);

	// IO Clocks Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD, ENABLE);				  
	// AFIO Clock Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// USART1 Clock Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	
	// USART1 Partial Pin Remap (TX=PB.6, RX=PB.7)
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);			
	// USART1 IO Pins
	// PB.6	USART1_TX	as Push-Pull Alternate Function Output							  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// PB.7	USART1_RX	as Floating Input
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	   
	// PD.7	USART1_TXEN	as Push-Pull General Purpose Output
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
   
	// USART1 Clock Configuration 
    USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable; 
    USART_ClockInit(USART1, &USART_ClockInitStructure); 

	// USART1 Configuration
//	USART_InitStructure.USART_BaudRate = 57600; //57600
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* Enable the USART OverSampling by 8 */
  USART_OverSampling8Cmd(UART4, ENABLE);
	// USART1 Init
	USART_Init(USART1, &USART_InitStructure);

	// Enable USART1 Receive interrupt
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	// Enable USART1
	USART_Cmd(USART1, ENABLE);
}

void USART1_SendString(char* buf)
{
	int i=0;			  
	// Disable USART1 Receive 
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	USART1_TXEN_H();
	while(buf[i]!='\0')
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			;
		USART_SendData(USART1,buf[i]);
		if(++i == USART_BufSz)
			break;
	}		
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		;
	USART1_TXEN_L();	 
	// Enable USART1 Receive 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void USART1_SendNBytes(char* buf, uint8_t bytesCnt)
{
	int i=0;
	// Disable USART1 Receive 
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	USART1_TXEN_H();
	while(i < bytesCnt)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			;
		USART_SendData(USART1,buf[i]);
		if(++i == USART_BufSz)
			break;
	}		
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		;
	USART1_TXEN_L();	 
	// Enable USART1 Receive 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

//void USART1_IRQHandler(void)
//{
//	uint8_t commArray[2];
//	uint8_t commCnt;
//	uint8_t u1ByteReceived;
//
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//	{
//		// Read one byte from the receive data register
//		Usart1.rxBuf[Usart1.rxPt] = USART_ReceiveData(USART1);
//		// Clear the USARTx Receive interrupt
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//
////		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
////			// Clear the USARTx Receive interrupt
////			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
////
////			u1ByteReceived = USART_ReceiveData(USART1);
////			cbWrite(&cbUsart1Received, &u1ByteReceived);
//
////		if(NF_Interpreter(&NFComBuf, Usart1.rxBuf, &Usart1.rxPt, commArray, &commCnt) > 0){
////			Usart1.rxDataReady = 1;
//			// Only Master Mode on USART1
//			//if(commCnt > 0){
//			//	Usart1.txCnt = NF_MakeCommandFrame((uint8_t*)Usart1.txBuf+1, (const uint8_t*)commArray, commCnt, NFComBuf.myAddress);
//			//	USART1_SendNBytes((char*)Usart1.txBuf+1, Usart1.txCnt);
//			//}
//		//}
//	}
//}

void	USART1_TXEN_H(void)		{GPIO_SetBits(GPIOD, GPIO_Pin_7);}
void	USART1_TXEN_L(void)		{GPIO_ResetBits(GPIOD, GPIO_Pin_7);}
