#include "led.h"

//##                                      #### ######## ################ GLOBALS
extern LED_St	Led;

void LED_Config(void){					  
	GPIO_InitTypeDef GPIO_InitStructure;
	// IO Clocks Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	/*	PORT C Push-Pull 10MHz Outputs:	*\
		PC.13	LED1					*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	Led.blink = 0;
	Led.blinkPhase = 0;
}

void LED_Set(u8 mask, u8 newState, u8 blink){
	if(mask & 1<<0){
		GPIO_WriteBit(LED_GPIO, LED1_PIN, ((newState & 1<<0)?Bit_SET:Bit_RESET));
		Led.blink &= (blink | ~(1<<0));
		Led.blink |= (blink & 1<<0);
	}
}

void LED_Proc(void){
	Led.blinkPhase^=1;
	if(Led.blink & 1<<0)
		GPIO_WriteBit(LED_GPIO, LED1_PIN, (Led.blinkPhase?Bit_SET:Bit_RESET));
}

u8		LED1_isON(void)		{return(GPIO_ReadOutputDataBit(LED_GPIO, LED1_PIN)!=0);}
void	LED1_ON(void)		{GPIO_SetBits(LED_GPIO, LED1_PIN);}
void	LED1_OFF(void)		{GPIO_ResetBits(LED_GPIO, LED1_PIN);}
void	LED1_TOGGLE(void)	{if(LED1_isON()) LED1_OFF(); else LED1_ON();}
