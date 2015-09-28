/*
 * FreeModbus Libary: Atmel AT91SAM3S Demo Application
 * Copyright (C) 2010 Christian Walter <cwalter@embedded-solutions.at>
 *
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * IF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: port.h,v 1.2 2010/06/06 13:46:42 wolti Exp $
 */

#ifndef _PORT_H
#define _PORT_H

/* ----------------------- Platform includes --------------------------------*/
#include "common.h"

/* ----------------------- Defines for Communication Buffer -----------------*/

#define MB_HR_TO_NFCOMBUF_UPDATE_NOTIFY

#define REG_INPUT_START                 ( 1000 )
#define REG_INPUT_NREGS                 ( 1 )

#define REG_HOLDING_START               ( 0 )
#define REG_HOLDING_NREGS               ( MB_HR_ADDR_MemoryOperation+1 )

#define MB_HR_ADDR_Config_Start	MB_HR_ADDR_SerialNumber
#define MB_HR_ADDR_Config_End	MB_HR_ADDR_RecallConfigurationFromMemory

#define MB_HR_RANGE_Diagnostics			( 1<<0 )
#define MB_HR_RANGE_State				( 1<<1 )
#define MB_HR_RANGE_Control				( 1<<2 )
#define MB_HR_RANGE_Config				( 1<<3 )

// Diagnostics
#define MB_HR_ADDR_DeviceInfo_L 0x01
#define MB_HR_ADDR_DeviceInfo_H 0x02
#define MB_HR_ADDR_DeviceStatus 0x03
#define MB_HR_ADDR_Voltage_5V 0x04
#define MB_HR_ADDR_Voltage_12V  0x05
#define MB_HR_ADDR_Voltage_24V  0x06
#define MB_HR_ADDR_Voltage_PowerStage1  0x07
#define MB_HR_ADDR_Voltage_PowerStage2  0x08
#define MB_HR_ADDR_Reserved09 0x09
#define MB_HR_ADDR_Reserved0a 0x0a
#define MB_HR_ADDR_Reserved0b 0x0b
#define MB_HR_ADDR_Reserved0c 0x0c
#define MB_HR_ADDR_Reserved0d 0x0d
// State
#define MB_HR_ADDR_Status_L 0x0e
#define MB_HR_ADDR_Status_H 0x0f
#define MB_HR_ADDR_DigitalInputs  0x10
#define MB_HR_ADDR_Reserved11 0x11
#define MB_HR_ADDR_Reserved12 0x12
#define MB_HR_ADDR_Reserved13 0x13
// Control
#define MB_HR_ADDR_ResetErrorFlags  0x14
#define MB_HR_ADDR_DrivesMode 0x15
#define MB_HR_ADDR_DigitalOutputs 0x16
#define MB_HR_ADDR_PowerOptions 0x17
#define MB_HR_ADDR_Reserved18 0x18
#define MB_HR_ADDR_Reserved19 0x19
#define MB_HR_ADDR_Reserved1a 0x1a
#define MB_HR_ADDR_Reserved1b 0x1b
// Configuration
#define MB_HR_ADDR_SerialNumber 0x1c
#define MB_HR_ADDR_DataLinkTimeout  0x1d
#define MB_HR_ADDR_MotorInstantPoweroffSource 0x1e
#define MB_HR_ADDR_MotorDelayedPoweroffSource 0x1f
#define MB_HR_ADDR_MotorPoweroffDelay 0x20
#define MB_HR_ADDR_BatteryVoltageLow  0x21
#define MB_HR_ADDR_BatteryVoltageCritical 0x22
#define MB_HR_ADDR_ShutdownDelay  0x23
#define MB_HR_ADDR_Reserved23 0x24
#define MB_HR_ADDR_Reserved25 0x25
#define MB_HR_ADDR_Reserved26 0x26
#define MB_HR_ADDR_Reserved27 0x27
#define MB_HR_ADDR_SerialInterfaceBitrate_L 0x28
#define MB_HR_ADDR_SerialInterfaceBitrate_H 0x29
#define MB_HR_ADDR_Mileage_L  0x2a
#define MB_HR_ADDR_Mileage_H  0x2b
#define MB_HR_ADDR_MemoryOperation  0x2c

// Memory Operation
#define MB_HR_Memory_StoreConfiguration (1<<0)
#define MB_HR_Memory_RecallConfiguration  (1<<1)
#define MB_HR_Memory_ResetConfiguration (1<<2)

#define MB_HR_PowerOptions_ShutDownNow  (1<<0)
#define MB_HR_PowerOptions_ShutDownAfterDelay (1<<1)
#define MB_HR_PowerOptions_ShutDownOnUsbDisconnect  (1<<2)



// The end of memory map
// #####################







/* ----------------------- Defines ------------------------------------------*/
#define	INLINE                      inline
#define PR_BEGIN_EXTERN_C           extern "C" {
#define	PR_END_EXTERN_C             }

#define ENTER_CRITICAL_SECTION( )   vMBPortEnterCritical()
#define EXIT_CRITICAL_SECTION( )    vMBPortExitCritical()

#define assert( x )

typedef char    BOOL;

typedef unsigned char UCHAR;
typedef char    CHAR;

typedef unsigned short USHORT;
typedef short   SHORT;

typedef unsigned long ULONG;
typedef long    LONG;

#ifndef TRUE
#define TRUE                                    1
#endif

#ifndef FALSE
#define FALSE                                   0
#endif

#define MB_PORT_HAS_CLOSE	                    1
#define MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS    2

/* ----------------------- Typedefs -----------------------------------------*/
typedef struct{
	uint8_t dataReadGlobal;
	uint8_t dataReceivedGlobal;
	uint8_t dataReceived[REG_HOLDING_NREGS];
} MODBUS_HOLDINGBUFFLAGS_St;

typedef struct{
	USHORT						usRegInputStart;
	USHORT						usRegInputBuf[REG_INPUT_NREGS];
	USHORT						usRegHoldingStart;
	USHORT						usRegHoldingBuf[REG_HOLDING_NREGS];
	MODBUS_HOLDINGBUFFLAGS_St	usRegHoldingFlags;
} MODBUS_St;

/* ----------------------- Prototypes ---------------------------------------*/
void MODBUS_Config(uint8_t addr, uint32_t bitrate);
void            vMBPortEnterCritical( void );
void            vMBPortExitCritical( void );
void  eMBSynchronizeFromHoldingRegs( void );
void  eMBSynchronizeToHoldingRegs( uint8_t range );

#endif
