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
 * File: $Id: porttimer.c,v 1.2 2010/06/06 13:46:42 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/

/* ----------------------- Modbus includes ----------------------------------*/
#include "port.h"
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/


#define MB_TIMER_PRESCALER                  ( 72UL )
#define MB_TIMER_TICKS                      ( 72000000UL / MB_TIMER_PRESCALER ) // 1MHz
#define MB_50US_TICKS                       ( 20000UL )


/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	uint16_t PrescalerValue = 0;
	uint16_t ccrVal = 0;

	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) MB_TIMER_PRESCALER;

    /* Calculate overflow counter an OCR values for Timer1. */
    ccrVal = ( MB_TIMER_TICKS * usTim1Timerout50us ) / ( MB_50US_TICKS );

	// TIM4 clock enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
	TIM_DeInit(TIM4);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = MB_TIMER_PRESCALER;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	/* Prescaler configuration */
	//TIM_PrescalerConfig(TIM4, PrescalerValue, TIM_PSCReloadMode_Immediate);

	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = ccrVal;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM4, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);

	/* TIM IT enable */
	//TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);	// Not yet :-)
	/* TIM4 enable counter */
	//TIM_Cmd(TIM4, ENABLE);	// Not yet :-)

    return TRUE;
}

void 
vMBPortTimerClose( void )
{
	/* TIM IT disable */
	TIM_ITConfig(TIM4, TIM_IT_CC1, DISABLE);
	/* TIM4 disable counter */
	TIM_Cmd(TIM4, DISABLE);
}

void
vMBPortTimersEnable(  )
{
	GPIO_SetBits(GPIOB, GPIO_Pin_11);
	/* Reset TIM4 Counter */
	TIM4->CNT = 0;
	/* TIM IT enable */
	TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
	TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
	/* TIM4 enable counter */
	TIM_Cmd(TIM4, ENABLE);
}

void
vMBPortTimersDisable(  )
{
	/* TIM IT disable */
	TIM_ITConfig(TIM4, TIM_IT_CC1, DISABLE);
	/* TIM4 disable counter */
	TIM_Cmd(TIM4, DISABLE);
}

void
vMBPortTimersDelay( USHORT usTimeOutMS )
{
    while( 1 ){
    	//WTF?!
    }
}

void
TIM4_IRQHandler( void )
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_11);
	/* TIM IT disable */
	TIM_ITConfig(TIM4, TIM_IT_CC1, DISABLE);
	/* TIM4 disable counter */
	TIM_Cmd(TIM4, DISABLE);
	// Run Timer Expired Routine
	( void )pxMBPortCBTimerExpired(  );
}
