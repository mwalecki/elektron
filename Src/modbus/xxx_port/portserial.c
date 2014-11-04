/*
 * FreeModbus Libary: Atmel AT91SAM3S Demo Application
 * Copyright (C) 2010 Christian Walter <cwalter@embedded-solutions.at>
 *
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * IF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: portserial.c,v 1.2 2010/06/06 13:46:42 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include <stdlib.h>
#include "port.h"
#include "io.h"
#include "usart1.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{

    if( xRxEnable )
    {
        //Enable Rx Data Ready Interrupt
    	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    }
    else
    {
        //Disable Rx Data Ready Interrupt
    	//USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
    }

    if( xTxEnable )
    {
    	//Drive Tx Enable Pin High
    	USART1_TXEN_H();
        //Enable Tx Empty Interrupt
    	USART_ClearITPendingBit(USART1, USART_IT_TXE);
    	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
        //Disable Tx Complete Interrupt
    	//USART_ClearITPendingBit(USART1, USART_IT_TC);
    	USART_ITConfig(USART1, USART_IT_TC, DISABLE);
    }
    else
    {
        //Disable Tx Empty Interrupt
    	//USART_ClearITPendingBit(USART1, USART_IT_TXE);
    	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
        //Enable Tx Complete Interrupt
    	USART_ClearITPendingBit(USART1, USART_IT_TC);
    	USART_ITConfig(USART1, USART_IT_TC, ENABLE);
    }
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    BOOL            bStatus = FALSE;
	USART_InitTypeDef USART_InitStructure;

	bStatus = TRUE;
	switch ( eParity )
	{
	case MB_PAR_NONE:
		//uiMode |= US_MR_PAR_NONE | US_MR_NBSTOP_2_BIT;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_StopBits = USART_StopBits_2;
		break;
	case MB_PAR_ODD:
		//uiMode |= US_MR_PAR_ODD | US_MR_NBSTOP_1_BIT;
		USART_InitStructure.USART_Parity = USART_Parity_Odd;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		break;
	case MB_PAR_EVEN:
		//uiMode |= US_MR_PAR_EVEN | US_MR_NBSTOP_1_BIT;
		USART_InitStructure.USART_Parity = USART_Parity_Even;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		break;
	default:
		bStatus = FALSE;
		break;
	}

	switch ( ucDataBits )
	{
	case 8:
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		break;
	case 7:
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		break;
	default:
		bStatus = FALSE;
		break;
	}

	if( TRUE == bStatus )
	{
		// Init the rest of USART1 param structure
		USART_InitStructure.USART_BaudRate = ulBaudRate;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

		// Do USART1 configuration including Clock, IO and Interrupts init
		USART1_Config(USART_InitStructure);
	}
    return bStatus;
}

void
vMBPortSerialClose( void )
{
	USART_Cmd(USART1, DISABLE);
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
	USART_SendData(USART1,ucByte);
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    *pucByte = USART_ReceiveData(USART1);
    return TRUE;
}

void
vUSARTHandler( void )
{
	// Overrun Error
	while(USART1->SR & USART_SR_ORE)
		USART_ReceiveData(USART1);

    //if( uiCSRMasked & US_CSR_RXRDY )
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		while(USART1->SR & USART_SR_RXNE)
			pxMBFrameCBByteReceived(  );
    }
    //if( uiCSRMasked & US_CSR_TXRDY )
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET){
        pxMBFrameCBTransmitterEmpty(  );
    }
    //if( uiCSRMasked & US_CSR_TXEMPTY )
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET){
    	USART_ITConfig(USART1, USART_IT_TC, DISABLE);
        USART1_TXEN_L();
    }

}

USART1_IRQHandler( void )
{
    vUSARTHandler(  );
}
