#ifndef _LED_H_
#define _LED_H_

#include "common.h"

//##                                      #### ######## ################ LED port map:
#define LED_GPIO	GPIOC
#define LED1_PIN	GPIO_Pin_13

//##                                      #### ######## ################ PROTOTYPES:

void	LED_Config(void);
void	LED_Set(u8 mask, u8 newState, u8 blink);
void	LED_Set(u8 mask, u8 newState, u8 blink);
void	LED_Proc(void);

u8		LED1_isON(void);
void	LED1_ON(void);
void	LED1_OFF(void);
void	LED1_TOGGLE(void);

#endif	// ifndef _LED_H_
