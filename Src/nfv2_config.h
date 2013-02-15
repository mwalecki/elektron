/*
* External config file.
*/

#define NF_BroadcastAddress		0x00
#define NF_MainModuleAddress	0x01

#define NF_MotorDrv1Address		0x21
#define NF_MotorDrv2Address		0x22
#define NF_UniMeter1Address		0x31

/*
* Uncomment and update buffers' size [no of param set instances] for commands
* to be supported by module:
*/
//	#define NF_BUFSZ_ReadDeviceStatus	1
	#define NF_BUFSZ_ReadDeviceVitals	8
	
	#define NF_BUFSZ_SetDrivesMode		2
	#define NF_BUFSZ_SetDrivesSpeed		2
//	#define NF_BUFSZ_SetDrivesCurrent	2
//	#define NF_BUFSZ_SetDrivesPosition	2

//	#define NF_BUFSZ_SetServosMode		4
	#define NF_BUFSZ_SetServosPosition	4
//	#define NF_BUFSZ_SetServosSpeed		4

	#define NF_BUFSZ_SetDigitalOutputs	1
	#define NF_BUFSZ_ReadAnalogInputs	8

/*
* Remember to declare:
* extern NF_STRUCT_ComBuf	NFComBuf;
*/
