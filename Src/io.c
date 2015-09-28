#include "io.h"
#include "systick.h"

extern DEVICE_CONTROL_St      DevControl;
																				 
void OUT1_ON(void)		{GPIO_SetBits(OUT1_GPIO, OUT1_PIN);}
void OUT1_OFF(void)		{GPIO_ResetBits(OUT1_GPIO, OUT1_PIN);}					 
void OUT2_ON(void)		{GPIO_SetBits(OUT2_GPIO, OUT2_PIN);}
void OUT2_OFF(void)		{GPIO_ResetBits(OUT2_GPIO, OUT2_PIN);}					 
void OUT3_ON(void)		{GPIO_SetBits(OUT3_GPIO, OUT3_PIN);}
void OUT3_OFF(void)		{GPIO_ResetBits(OUT3_GPIO, OUT3_PIN);}					 
void OUT4_ON(void)		{GPIO_SetBits(OUT4_GPIO, OUT4_PIN);}
void OUT4_OFF(void)		{GPIO_ResetBits(OUT4_GPIO, OUT4_PIN);}				 
void OUT5_ON(void)		{GPIO_SetBits(OUT5_GPIO, OUT5_PIN);}
void OUT5_OFF(void)		{GPIO_ResetBits(OUT5_GPIO, OUT5_PIN);}				 
void OUT6_ON(void)		{GPIO_SetBits(OUT6_GPIO, OUT6_PIN);}
void OUT6_OFF(void)		{GPIO_ResetBits(OUT6_GPIO, OUT6_PIN);}				 
void OUT7_ON(void)		{GPIO_SetBits(OUT7_GPIO, OUT7_PIN);}
void OUT7_OFF(void)		{GPIO_ResetBits(OUT7_GPIO, OUT7_PIN);}				 
//void OUT8_ON(void)		{GPIO_SetBits(OUT8_GPIO, OUT8_PIN);}
//void OUT8_OFF(void)		{GPIO_ResetBits(OUT8_GPIO, OUT8_PIN);}

void OUT_Config(void){					  
	GPIO_InitTypeDef GPIO_InitStructure;
	// IO Clocks Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE);
	/*	PORT B Push-Pull 10MHz Outputs:	*\
		PB.1	OUT1					
		PB.2	OUT2					*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/*	PORT E Push-Pull 10MHz Outputs:	*\
		PE.7	OUT3					
		PE.8	OUT4					
		PE.9	OUT5					
		PE.10	OUT6					
		PE.11	OUT7					*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void OUT_Set(u8 set){
	if(set & (1<<0)) OUT1_ON();	else OUT1_OFF();
	if(set & (1<<1)) OUT2_ON();	else OUT2_OFF();
	if(set & (1<<2)) OUT3_ON();	else OUT3_OFF();
	if(set & (1<<3)) OUT4_ON();	else OUT4_OFF();
	if(set & (1<<4)) OUT5_ON();	else OUT5_OFF();
	if(set & (1<<5)) OUT6_ON();	else OUT6_OFF();
//	if(set & (1<<6)) OUT7_ON();	else OUT7_OFF();
//	if(set & (1<<7)) OUT8_ON();	else OUT8_OFF();
}

void OUT_Control(void){
  static uint16_t outputs;

  if(DevControl.digitalOutputs == outputs)
    return;

  outputs = DevControl.digitalOutputs;

  ((outputs & (1<<0)) ? REL1_ON() : REL1_OFF());
  ((outputs & (1<<1)) ? REL2_ON() : REL2_OFF());
  ((outputs & (1<<2)) ? REL3_ON() : REL3_OFF());
  ((outputs & (1<<3)) ? REL4_ON() : REL4_OFF());
  ((outputs & (1<<7)) ? R_OFF_H() : R_OFF_L());
}

void IO_PinConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOSpeed_TypeDef GPIO_Speed, GPIOMode_TypeDef GPIO_Mode){
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
  GPIO_Init(GPIOx, &GPIO_InitStructure);
}

void IN_Config(void){
  // IO Clocks Enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  IO_PinConfig(GPIOA, GPIO_Pin_9, GPIO_Speed_10MHz, GPIO_Mode_IPD); // USB Vin
}

uint8_t IN_ReadUsbVIn(void){
  return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9);
}


