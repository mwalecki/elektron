#include "usart4.h"

	 
extern USART_St		Usart4;	   
extern REFERENCE_St	Reference;	
extern STDOWNCNT_St	STDownCnt[ST_Downcounters];		
extern uint8_t 		crcTable[256]; 	 
extern MSENSE_St	MSense;	  
extern NF_STRUCT_ComBuf 	NFComBuf;



void USART4_Config(void){						   
	GPIO_InitTypeDef GPIO_InitStructure;  
    //USART_ClockInitTypeDef  USART_ClockInitStructure;
	USART_InitTypeDef USART_InitStructure;

	// IO Clocks Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);				  
	// AFIO Clock Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// USARTx Clock Enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	
	// USARTx Pin Remap
	//GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);			
	// USARTx IO Pins
	// PC.10	USARTx_TX	as Push-Pull Alternate Function Output							  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	// PC.11	USARTx_RX	as Floating Input
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	   
	// Px.x	USARTx_TXEN	as Push-Pull General Purpose Output
	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure); */
   
	// USARTx Clock Configuration 
    /*USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable; 
    USART_ClockInit(USART1, &USART_ClockInitStructure); */

	// USARTx Configuration
	USART_InitStructure.USART_BaudRate = 57600; //57600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);

	// Enable USARTx Receive interrupt
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);

	// Enable USARTx
	USART_Cmd(UART4, ENABLE);
}

void USART4_SendParams(void)
{
	u8 i;
	while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET)
			;
		USART_SendData(UART4, '#');
	
	for(i=0; i<=6; i++)
	{
		while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET)
			;
		USART_SendData(UART4, MSense.dist[i] >> 8);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET)
			;
		USART_SendData(UART4, MSense.dist[i] & 0xff);
	}
}

void USART4_SendNBytes(char* buf, uint8_t bytesCnt)
{
	int i=0;
	// Disable UART4 Receive 
//	USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
//	USART4_TXEN_H();
	while(i < bytesCnt)
	{
		while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET)
			;
		USART_SendData(UART4,buf[i]);
		if(++i == USART_BufSz)
			break;
	}		
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET)
		;
//	USART1_TXEN_L();	 
	// Enable USART1 Receive 
//	USART_ITConfig(USART4, USART_IT_RXNE, ENABLE);
}

void UART4_IRQHandler(void)
{
	uint8_t commArray[2];
	uint8_t commCnt;
	uint8_t bytesToSend;

	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		// Read one byte from the receive data register
		Usart4.rxBuf[Usart4.rxPt] = USART_ReceiveData(UART4);      
		// Clear the USARTx Receive interrupt
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
		
		if(NF_Interpreter(&NFComBuf, Usart4.rxBuf, &Usart4.rxPt, commArray, &commCnt) > 0){
			Reference.leftSpeed = NFComBuf.SetDrivesSpeed.data[0];
			Reference.rightSpeed = NFComBuf.SetDrivesSpeed.data[1];
			Reference.srv1pos = NFComBuf.SetServosPosition.data[0];
			Reference.srv2pos = NFComBuf.SetServosPosition.data[1];
			Reference.srv3pos = NFComBuf.SetServosPosition.data[2];
			Reference.srv4pos = NFComBuf.SetServosPosition.data[3];
			Reference.digiOut = NFComBuf.SetDigitalOutputs.data[0];
			NFComBuf.dataReceived = 1;

			if(commCnt > 0){
				bytesToSend = NF_MakeCommandFrame(&NFComBuf, (uint8_t*)Usart4.txBuf+1, (const uint8_t*)commArray, commCnt, NFComBuf.myAddress);
				USART4_SendNBytes((char*)Usart4.txBuf+1, bytesToSend);
			}
		}
	}
}
