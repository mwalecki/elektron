#include "eebackup.h"
#include "nf/nfv2.h"
	 
extern uint16_t				serialNumber;
extern NF_STRUCT_ComBuf 	NFComBuf;

uint8_t MotorControllerAddr0, MotorControllerAddr1, InputOutputAddr0;

void eebackup_Recover(void)
{
	MotorControllerAddr0 = EEPROM_Read(EEADDR_MC0_ADDR);
	MotorControllerAddr1 = EEPROM_Read(EEADDR_MC1_ADDR);

//	NFComBuf.SetDrivesSpeed.addr[0] =
//		(((uint32_t)EEPROM_Read(EEADDR_DRV1_MIN_H)) << 16 ) | EEPROM_Read(EEADDR_DRV1_MIN_L);
}

void eebackup_SaveAll(void)
{
	EEPROM_Write(EEADDR_MC0_ADDR, (u16) MotorControllerAddr0);
	EEPROM_Write(EEADDR_MC1_ADDR, (u16) MotorControllerAddr1);

//	EEPROM_Write(EEADDR_DRV1_MIN_L, (u16) (NFComBuf.SetDrivesMinPosition.data[0] & 0x0000ffff));
}

void eebackup_SaveInitialValues(void)
{
}

