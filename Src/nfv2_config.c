#include "stdio.h"
#include "nf/nfv2.h"

void NFv2_Config(NF_STRUCT_ComBuf *NFComBuf, uint8_t myAddress){

	NFComBuf->myAddress = myAddress;
	NFComBuf->dataReceived = 0;

	// Address map is essential only when device acts as Master on NFv2 bus.


	NFComBuf->SetDrivesMode.addr[0] = NF_MotorDrv1Address;
	NFComBuf->SetDrivesMode.addr[1] = NF_MotorDrv2Address;

	NFComBuf->SetDrivesSpeed.addr[0] = NF_MotorDrv1Address;
	NFComBuf->SetDrivesSpeed.addr[1] = NF_MotorDrv2Address;

	NFComBuf->ReadDrivesPosition.addr[0] = NF_MotorDrv1Address;
	NFComBuf->ReadDrivesPosition.addr[1] = NF_MotorDrv2Address;

	NFComBuf->ReadAnalogInputs.addr[0] = NF_InOut1Address;
	NFComBuf->ReadAnalogInputs.addr[1] = NF_InOut1Address;
	NFComBuf->ReadAnalogInputs.addr[2] = NF_InOut1Address;
	NFComBuf->ReadAnalogInputs.addr[3] = NF_InOut1Address;
	NFComBuf->ReadAnalogInputs.addr[4] = NF_InOut1Address;
	NFComBuf->ReadAnalogInputs.addr[5] = NF_InOut1Address;
	NFComBuf->ReadAnalogInputs.addr[6] = NF_InOut1Address;
	NFComBuf->ReadAnalogInputs.addr[7] = NF_InOut1Address;

	NFComBuf->ReadDigitalInputs.addr[0] = NF_InOut1Address;
	NFComBuf->ReadDigitalInputs.addr[1] = NF_MotorDrv1Address;

	NFComBuf->SetDigitalOutputs.addr[0] = NF_MainModuleAddress;
	NFComBuf->SetDigitalOutputs.addr[1] = NF_InOut1Address;

	NFComBuf->SetServosPosition.addr[0] = NF_InOut1Address;
	NFComBuf->SetServosPosition.addr[1] = NF_InOut1Address;
	NFComBuf->SetServosPosition.addr[2] = NF_InOut1Address;
	NFComBuf->SetServosPosition.addr[3] = NF_InOut1Address;
	NFComBuf->SetServosPosition.addr[4] = NF_InOut1Address;
	NFComBuf->SetServosPosition.addr[5] = NF_InOut1Address;
	NFComBuf->SetServosPosition.addr[6] = NF_InOut1Address;
	NFComBuf->SetServosPosition.addr[7] = NF_InOut1Address;


	// Data initialization

	NFComBuf->ReadAnalogInputs.data[0] = 5;
	NFComBuf->ReadAnalogInputs.data[1] = 6;
	NFComBuf->ReadAnalogInputs.data[2] = 7;
	NFComBuf->ReadAnalogInputs.data[3] = 8;
	NFComBuf->ReadAnalogInputs.data[4] = 9;
	NFComBuf->ReadAnalogInputs.data[5] = 10;
	NFComBuf->ReadAnalogInputs.data[6] = 11;
	NFComBuf->ReadAnalogInputs.data[7] = 10;


	NFComBuf->ReadDeviceVitals.data[0] = 24500;
	NFComBuf->ReadDeviceVitals.data[1] = 24000;
	NFComBuf->ReadDeviceVitals.data[2] = 12000;
	NFComBuf->ReadDeviceVitals.data[3] = 5000;
	NFComBuf->ReadDeviceVitals.data[4] = 24500;
	NFComBuf->ReadDeviceVitals.data[5] = 24500;
	NFComBuf->ReadDeviceVitals.data[6] = 0;
	NFComBuf->ReadDeviceVitals.data[7] = 0;

}
