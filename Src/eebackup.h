#ifndef EEBACKUP_H
#define EEBACKUP_H

#include "common.h"
#include "eeprom.h"

#define EEADDR_MC0_ADDR		1
#define EEADDR_MC1_ADDR		2

#define EEADDR_BATT_LOW		10
#define EEADDR_BATT_CRIT	11



void eebackup_Recover(void);	
void eebackup_SaveAll(void);
void eebackup_SaveInitialValues(void);

#endif //EEBACKUP_H
