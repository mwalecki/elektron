#include "eebackup.h"
#include "eeprom.h"
#include "port.h"

extern MODBUS_St ModBus;
extern uint32_t        mileage;
extern DEVICE_CONFIGURATION_St DevConfiguration;
uint16_t iRegIndex, iEeAddr;

void eebackup_Recover(void)
{
//  uint16_t iRegIndex, iEeAddr;
  iEeAddr = EE_EEADDR_CONFIG_REGS_START;
  iRegIndex = EE_REGIDX_CONFIG_REGS_START;

  while(1){
    if((iEeAddr > EE_EEADDR_CONFIG_REGS_END) || (iRegIndex > EE_REGIDX_CONFIG_REGS_END))
      break;
    ModBus.usRegHoldingBuf[iRegIndex] = EEPROM_Read(iEeAddr);
    ModBus.usRegHoldingFlags.dataReceived[iRegIndex] = 1;
    iEeAddr++;
    iRegIndex++;
  }
  DevConfiguration.mileage =
      ModBus.usRegHoldingBuf[MB_HR_ADDR_Mileage_L] |
      ((int32_t)ModBus.usRegHoldingBuf[MB_HR_ADDR_Mileage_H] << 16);

  if(DevConfiguration.mileage == 0xffffffff){
    eebackup_ResetCommunicationSettings();
  }
  ModBus.usRegHoldingFlags.dataReceivedGlobal = 1;
}

void eebackup_SaveAll(void)
{
//  uint16_t iRegIndex, iEeAddr;
  iEeAddr = EE_EEADDR_CONFIG_REGS_START;
  iRegIndex = EE_REGIDX_CONFIG_REGS_START;

  DevConfiguration.mileage++;
  ModBus.usRegHoldingBuf[MB_HR_ADDR_Mileage_L] = (DevConfiguration.mileage & 0xffff);
  ModBus.usRegHoldingBuf[MB_HR_ADDR_Mileage_H] = ((DevConfiguration.mileage >> 16) & 0xffff);

  while(1){
    if((iEeAddr > EE_EEADDR_CONFIG_REGS_END) || (iRegIndex > EE_REGIDX_CONFIG_REGS_END))
      break;
    EEPROM_Write(iEeAddr, (u16) ModBus.usRegHoldingBuf[iRegIndex]);
    iEeAddr++;
    iRegIndex++;
  }
}

void eebackup_Reset(void)
{
  uint16_t iRegIndex;
  iRegIndex = EE_REGIDX_CONFIG_REGS_START;

  while(1){
    if(iRegIndex > EE_REGIDX_CONFIG_REGS_END)
      break;
    ModBus.usRegHoldingBuf[iRegIndex] = 0;
    iRegIndex++;
  }
}

void eebackup_ResetCommunicationSettings(void){
  DevConfiguration.serialInterfaceBitrate = SERIAL_DEF_BITRATE;
  ModBus.usRegHoldingBuf[MB_HR_ADDR_SerialInterfaceBitrate_L] = (DevConfiguration.serialInterfaceBitrate & 0xffff);
  ModBus.usRegHoldingBuf[MB_HR_ADDR_SerialInterfaceBitrate_H] = ((DevConfiguration.serialInterfaceBitrate >> 16) & 0xffff);
  eebackup_SaveAll();
}
