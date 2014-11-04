#ifndef EEBACKUP_H
#define EEBACKUP_H

#include "common.h"
#include "port.h"

#define EE_REGIDX_CONFIG_REGS_START MB_HR_ADDR_SerialNumber
#define EE_REGIDX_CONFIG_REGS_END   (MB_HR_ADDR_MemoryOperation-1)
#define EE_EEADDR_CONFIG_REGS_START 0
#define EE_EEADDR_CONFIG_REGS_END 127 // Memory limit for config regs

void eebackup_Recover(void);
void eebackup_SaveAll(void);
void eebackup_Reset(void);
void eebackup_ResetCommunicationSettings(void);

#endif //EEBACKUP_H
