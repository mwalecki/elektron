#ifndef _IO_H
#define _IO_H

#include "common.h"

//##                                      #### ######## ################ OUT port map:
#define OUT1_GPIO	GPIOB
#define OUT1_PIN	GPIO_Pin_1
#define OUT2_GPIO	GPIOB
#define OUT2_PIN	GPIO_Pin_2
#define OUT3_GPIO	GPIOE
#define OUT3_PIN	GPIO_Pin_7
#define OUT4_GPIO	GPIOE
#define OUT4_PIN	GPIO_Pin_8
#define OUT5_GPIO	GPIOE
#define OUT5_PIN	GPIO_Pin_9
#define OUT6_GPIO	GPIOE
#define OUT6_PIN	GPIO_Pin_10
#define OUT7_GPIO	GPIOE
#define OUT7_PIN	GPIO_Pin_11
//#define OUT8_GPIO	GPIOE
//#define OUT8_PIN	GPIO_Pin_11

//##                                      #### ######## ################ PROTOTYPES:
void OUT1_ON(void);
void OUT1_OFF(void);
void OUT2_ON(void);
void OUT2_OFF(void);
void OUT3_ON(void);
void OUT3_OFF(void);
void OUT4_ON(void);
void OUT4_OFF(void);
void OUT5_ON(void);
void OUT5_OFF(void);
void OUT6_ON(void);
void OUT6_OFF(void);
void OUT7_ON(void);
void OUT7_OFF(void);
//void OUT8_ON(void);
//void OUT8_OFF(void);

void OUT_Config(void);
void OUT_Set(u8 set);
void REL_Set(u8 set);
void OUT_Control(void);
							
//##                                      #### ######## ################ OUT functionality:

#define REL1_ON()	OUT1_ON()
#define REL1_OFF()	OUT1_OFF()
#define REL2_ON()	OUT3_ON()
#define REL2_OFF()	OUT3_OFF()
#define REL3_ON()	OUT2_ON()
#define REL3_OFF()	OUT2_OFF()
#define REL4_ON()	OUT5_ON()
#define REL4_OFF()	OUT5_OFF()
#define PS_ON_H()	OUT6_ON()
#define PS_ON_L()	OUT6_OFF()
#define PS_EN_H()	OUT7_ON()
#define PS_EN_L()	OUT7_OFF()
#define R_OFF_H()	OUT4_ON()
#define R_OFF_L()	OUT4_OFF()

#endif //_IO_H
