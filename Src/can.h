/*
 * can.h
 *
 *  Created on: Jul 1, 2015
 *      Author: wacek
 */

#ifndef CAN_H_
#define CAN_H_

#include "common.h"

#define CAN_RX_PIN GPIO_Pin_0
#define CAN_RX_GPIO GPIOD
#define CAN_RX_APB2 RCC_APB2Periph_GPIOD
#define CAN_TX_PIN GPIO_Pin_1
#define CAN_TX_GPIO GPIOD
#define CAN_TX_APB2 RCC_APB2Periph_GPIOD

void canConfig(void);
void canIstr(void);
void canProcess(CanRxMsg *rx);

#endif /* CAN_H_ */
