#include "usart1.h"

	 
extern USART_St		Usart1;
extern STDOWNCNT_St	STDownCnt[ST_Downcounters];	
extern MSENSE_St	MSense;			 

//vu8 crcbuf[20];
//crc  crcTable[256];

void USART1_Config(void){						   
	GPIO_InitTypeDef GPIO_InitStructure;  
    USART_ClockInitTypeDef  USART_ClockInitStructure;
	USART_InitTypeDef USART_InitStructure;

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
	USART_InitStructure.USART_BaudRate = 57600; //57600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
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

/*void USART1_IRQHandler(void)
{	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		// Read one byte from the receive data register
		Usart1.rxBuf[Usart1.rxPt] = USART_ReceiveData(USART1);      
		// Clear the USARTx Receive interrupt
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
																										 
		// Unknown start byte detected
		if(Usart1.rxPt==0 && Usart1.rxBuf[Usart1.rxPt]!='%' && Usart1.rxBuf[Usart1.rxPt]!='*' && Usart1.rxBuf[Usart1.rxPt]!=':')
			Usart1.rxPt=0;		 
		// Data mode, full frame received
		else if(Usart1.rxBuf[Usart1.rxPt]=='%' && Usart1.rxPt==USART1_RxFrameSz)
		{
		//	if(crcFast(((const u8*)Usart1.rxBuf) + 1,  11)!=Usart1.rxBuf[12])
			if(0)
			{
				Usart1.rxPt = 0;
			}
			else
			{
				MSense.dist[0] = Usart1.rxBuf[1]<<8 | Usart1.rxBuf[2];
				MSense.dist[1] = Usart1.rxBuf[3]<<8 | Usart1.rxBuf[4];
				MSense.dist[2] = Usart1.rxBuf[5]<<8 | Usart1.rxBuf[6];
				MSense.dist[3] = Usart1.rxBuf[7]<<8 | Usart1.rxBuf[8];
				MSense.dist[4] = Usart1.rxBuf[9]<<8 | Usart1.rxBuf[10];
				MSense.dist[5] = Usart1.rxBuf[11]<<8 | Usart1.rxBuf[12];
				MSense.dist[6] = Usart1.rxBuf[13]<<8 | Usart1.rxBuf[14];
				MSense.dist[7] = Usart1.rxBuf[15]<<8 | Usart1.rxBuf[16];
				Usart1.rxPt = 0;
			}
		}  
		// Text mode, 'CR' or 'LF' detected
		else if((Usart1.rxBuf[0]==':' || Usart1.rxBuf[0]=='*')
				&& (Usart1.rxBuf[Usart1.rxPt]==13 || Usart1.rxBuf[Usart1.rxPt]==10)) 
		{
			Usart1.rxDataReady=1;
			Usart1.rxPt=0;
		}
		else
			Usart1.rxPt++;
	}
}*/

void USART1_IRQHandler(void)
{
	uint8_t commArray[2];
	uint8_t commCnt;
//	uint8_t bytesToSend;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		// Read one byte from the receive data register
		Usart1.rxBuf[Usart1.rxPt] = USART_ReceiveData(USART1);      
		// Clear the USARTx Receive interrupt
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		
		if(NF_Interpreter(Usart1.rxBuf, &Usart1.rxPt, commArray, &commCnt) > 0){
			Usart1.rxDataReady = 1;
			// Only Master Mode on USART1
			//if(commCnt > 0){
			//	bytesToSend = NF_MakeCommandFrame((uint8_t*)Usart1.txBuf+1, (const uint8_t*)commArray, commCnt, NFComBuf.myAddress);
			//	USART1_SendNBytes((char*)Usart1.txBuf+1, bytesToSend);
			//}
		}
	}
}

void USART1_Interpreter(u8* buf)
{
	_IF_MEMBER_THEN(buf, "*IDN?", 5, 0)
    {
		sprintf((char*) Usart1.txBuf, "%s, %s %s", MODULE_NAME, __DATE__, __TIME__);
		ST_Reset(ST_UsartTxDelay);
		Usart1.txDataReady = 1;
	}
	else
	_GROUP(buf, ":LED", 4) 
		_IF_MEMBER_THEN(buf, ":SET", 4, 4)
			switch(buf[9]){
				case '0':
					LED_Set(1<<0, 1<<0, 0);
					break;
				case '1':
					LED_Set(1<<1, 1<<1, 0);
					break;
				case '2':
					LED_Set(1<<2, 1<<2, 0);
					break;
			}
		else  
		_IF_MEMBER_THEN(buf, ":RES", 4, 4)
			switch(buf[9]){
				case '0':
					LED_Set(1<<0, 0, 0);
					break;
				case '1':
					LED_Set(1<<1, 0, 0);
					break;
				case '2':
					LED_Set(1<<2, 0, 0);
					break;
			}
	_ENDGROUP
}

void	USART1_TXEN_H(void)		{GPIO_SetBits(GPIOD, GPIO_Pin_7);}
void	USART1_TXEN_L(void)		{GPIO_ResetBits(GPIOD, GPIO_Pin_7);}
