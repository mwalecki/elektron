	/*
 * FreeModbus Callback Functions and other stuff
 * Copyright (C) 2013 Michal Walecki <m.walecki@versabox.pl>
 */

#include "common.h"

#include "mb.h"
#include "mbport.h"
#include "eebackup.h"
#include "central.h"
#include "io.h"
#include "usart1.h"
#include "eebackup.h"
#include "systick.h"

extern MODBUS_St ModBus;
extern uint16_t				serialNumber;
extern uint32_t       mileage;
extern USART_St				Usart1;
extern MCENTRAL_St     MCentral;
extern DEVICE_DIAGNOSTICS_St   DevDiagnostics;
extern DEVICE_STATE_St         DevState;
extern DEVICE_CONTROL_St       DevControl;
extern DEVICE_CONFIGURATION_St DevConfiguration;
extern STDOWNCNT_St STDownCnt[ST_Downcounters];

void MODBUS_Config(uint8_t addr, uint32_t bitrate){
    eMBErrorCode    eStatus;
	const UCHAR     ucSlaveID[] = { 0xAA, 0xBB, 0xCC };

	ModBus.usRegInputStart = REG_INPUT_START;
	ModBus.usRegHoldingStart = REG_HOLDING_START;
    ModBus.usRegHoldingFlags.dataReceivedGlobal = 0;

	//if( MB_ENOERR != ( eStatus = eMBInit( MB_RTU, addr, 2, 38400, MB_PAR_NONE) ) )
	if( MB_ENOERR != ( eStatus = eMBInit( MB_RTU, addr, 2, bitrate, MB_PAR_NONE) ) )
	{
		/* Can not initialize. Add error handling code here. */
		while(1){
		}
	}
	else
	{
		if( MB_ENOERR != ( eStatus = eMBSetSlaveID( 0x34, TRUE, ucSlaveID, 3 ) ) )
		{
			/* Can not set slave id. Check arguments */
			while(1){
			}
		}
		else if( MB_ENOERR != ( eStatus = eMBEnable(  ) ) )
		{
			/* Enable failed. */
			while(1){
			}
		}
		else
		{
			ModBus.usRegHoldingBuf[0] = 1;
			return;
		}
	}
}

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - ModBus.usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ =
                ( unsigned char )( ModBus.usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( ModBus.usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_HOLDING_START ) && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - ModBus.usRegHoldingStart );
        switch ( eMode )
        {
        case MB_REG_READ:
            while( usNRegs > 0 )
            {
                *pucRegBuffer++ = ( unsigned char )( ModBus.usRegHoldingBuf[iRegIndex] >> 8 );
                *pucRegBuffer++ = ( unsigned char )( ModBus.usRegHoldingBuf[iRegIndex] & 0xFF );
                iRegIndex++;
                usNRegs--;
            }
            ModBus.usRegHoldingFlags.dataReadGlobal = 1;
            break;

        case MB_REG_WRITE:
            while( usNRegs > 0 )
            {
            	ModBus.usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
            	ModBus.usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
            	ModBus.usRegHoldingFlags.dataReceived[iRegIndex] = 1;
                iRegIndex++;
                usNRegs--;
            }
            ModBus.usRegHoldingFlags.dataReceivedGlobal = 1;
            break;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    return MB_ENOREG;
}

void
eMBSynchronizeFromHoldingRegs( void )
{
	uint16_t iRegIndex;
	for(iRegIndex=0; iRegIndex<REG_HOLDING_NREGS; iRegIndex++){
		if(ModBus.usRegHoldingFlags.dataReceived[iRegIndex] == 1){
			switch(iRegIndex){
			case MB_HR_ADDR_ResetErrorFlags:
				if(ModBus.usRegHoldingBuf[MB_HR_ADDR_ResetErrorFlags] != 0){
          MCentral.statusL = 0;
          MCentral.statusH = 0;
				}
				break;
      case MB_HR_ADDR_DrivesMode:
        DevControl.mode = ModBus.usRegHoldingBuf[MB_HR_ADDR_DrivesMode];
        break;
      case MB_HR_ADDR_DigitalOutputs:
        DevControl.digitalOutputs = ModBus.usRegHoldingBuf[MB_HR_ADDR_DigitalOutputs];
        break;
      case MB_HR_ADDR_PowerOptions:
        DevControl.power_options = ModBus.usRegHoldingBuf[MB_HR_ADDR_PowerOptions];
        break;
      case MB_HR_ADDR_SerialNumber:
        DevConfiguration.serialNumber = ModBus.usRegHoldingBuf[MB_HR_ADDR_SerialNumber];
        break;
      case MB_HR_ADDR_DataLinkTimeout:
        STDownCnt[ST_CommandWD].period = ModBus.usRegHoldingBuf[MB_HR_ADDR_DataLinkTimeout] *1000; // 100ms->100us
        break;
      case MB_HR_ADDR_MotorPoweroffDelay:
        DevConfiguration.MotorPoweroffDelay = ModBus.usRegHoldingBuf[MB_HR_ADDR_MotorPoweroffDelay]; // [100ms]
        break;
      case MB_HR_ADDR_BatteryVoltageLow:
        MCentral.batteryVoltageLow = ModBus.usRegHoldingBuf[MB_HR_ADDR_BatteryVoltageLow]; // [mV]
        break;
      case MB_HR_ADDR_BatteryVoltageCritical:
        MCentral.batteryVoltageCritical = ModBus.usRegHoldingBuf[MB_HR_ADDR_BatteryVoltageCritical]; // [mV]
        break;
      case MB_HR_ADDR_ShutdownDelay:
        MCentral.shutDownDelay = ModBus.usRegHoldingBuf[MB_HR_ADDR_ShutdownDelay]; // [s]
        break;
      case MB_HR_ADDR_SerialInterfaceBitrate_L:
        DevConfiguration.serialInterfaceBitrate =
            ModBus.usRegHoldingBuf[MB_HR_ADDR_SerialInterfaceBitrate_L] |
            ((int32_t)ModBus.usRegHoldingBuf[MB_HR_ADDR_SerialInterfaceBitrate_H] << 16);
        break;
      case MB_HR_ADDR_SerialInterfaceBitrate_H:
        break;

      case MB_HR_ADDR_MemoryOperation:
        switch(ModBus.usRegHoldingBuf[MB_HR_ADDR_MemoryOperation] & (MB_HR_Memory_RecallConfiguration | MB_HR_Memory_StoreConfiguration | MB_HR_Memory_ResetConfiguration)){
        case MB_HR_Memory_StoreConfiguration:
          eebackup_SaveAll();
          break;
        case MB_HR_Memory_RecallConfiguration:
          eebackup_Recover();
          break;
        case MB_HR_Memory_ResetConfiguration:
          eebackup_Reset();
          break;
        }
        break;
			}
			ModBus.usRegHoldingFlags.dataReceived[iRegIndex] = 0;
		}
	}
}

void
eMBSynchronizeToHoldingRegs( uint8_t range ){
	if(range & MB_HR_RANGE_Diagnostics){
    ModBus.usRegHoldingBuf[MB_HR_ADDR_DeviceStatus] = 0;
    ModBus.usRegHoldingBuf[MB_HR_ADDR_Voltage_5V] = DevDiagnostics.voltage5V;
    ModBus.usRegHoldingBuf[MB_HR_ADDR_Voltage_12V] = DevDiagnostics.voltage12V;
    ModBus.usRegHoldingBuf[MB_HR_ADDR_Voltage_24V] = DevDiagnostics.voltage24V;
    ModBus.usRegHoldingBuf[MB_HR_ADDR_Voltage_PowerStage1] = DevDiagnostics.voltagePowerStage1;
    ModBus.usRegHoldingBuf[MB_HR_ADDR_Voltage_PowerStage2] = DevDiagnostics.voltagePowerStage2;
	}
	if(range & MB_HR_RANGE_State){
    ModBus.usRegHoldingBuf[MB_HR_ADDR_Status_L] = DevState.status_L;
    ModBus.usRegHoldingBuf[MB_HR_ADDR_Status_H] = DevState.status_H;
		ModBus.usRegHoldingBuf[MB_HR_ADDR_DigitalInputs] = 0;
	}
	if(range & MB_HR_RANGE_Control){
    ModBus.usRegHoldingBuf[MB_HR_ADDR_ResetErrorFlags] = 0;
	  ModBus.usRegHoldingBuf[MB_HR_ADDR_DigitalOutputs] = DevControl.mode;
    ModBus.usRegHoldingBuf[MB_HR_ADDR_DigitalOutputs] = DevControl.digitalOutputs;
	}
	  if(range & MB_HR_RANGE_Config){
		ModBus.usRegHoldingBuf[MB_HR_ADDR_SerialNumber] = DevConfiguration.serialNumber;
		ModBus.usRegHoldingBuf[MB_HR_ADDR_DataLinkTimeout] = STDownCnt[ST_CommandWD].period /1000; // 100us->100ms
		ModBus.usRegHoldingBuf[MB_HR_ADDR_MotorPoweroffDelay] = DevConfiguration.MotorPoweroffDelay; // [100ms]
    ModBus.usRegHoldingBuf[MB_HR_ADDR_SerialInterfaceBitrate_L] = (DevConfiguration.serialInterfaceBitrate & 0xffff);
    ModBus.usRegHoldingBuf[MB_HR_ADDR_SerialInterfaceBitrate_H] = ((DevConfiguration.serialInterfaceBitrate >> 16) & 0xffff);
    ModBus.usRegHoldingBuf[MB_HR_ADDR_Mileage_L] = (DevConfiguration.mileage & 0xffff);
    ModBus.usRegHoldingBuf[MB_HR_ADDR_Mileage_H] = ((DevConfiguration.mileage >> 16) & 0xffff);
		ModBus.usRegHoldingBuf[MB_HR_ADDR_MemoryOperation] = 0;
	}
}



























