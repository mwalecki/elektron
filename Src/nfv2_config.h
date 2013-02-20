/*
* External config file.
*/

#define NF_BroadcastAddress		0x00
#define NF_MainModuleAddress	0x01

#define NF_MotorDrv1Address		0x21
#define NF_MotorDrv2Address		0x22
#define NF_InOut1Address		0x31

/*
* Uncomment and update buffers' size [no of param set instances] for commands
* to be supported by module:
*/
	#define NF_BUFSZ_ReadDeviceStatus		1
	#define NF_BUFSZ_ReadDeviceVitals		8

	#define NF_BUFSZ_SetDrivesMode			2
	#define NF_BUFSZ_SetDrivesSpeed			2
//	#define NF_BUFSZ_SetDrivesCurrent		2
	#define NF_BUFSZ_SetDrivesPosition		2
	#define NF_BUFSZ_SetDrivesPWM			2
	#define NF_BUFSZ_SetDrivesMaxCurrent	2
	#define NF_BUFSZ_SetDrivesMaxSpeed		2
	#define NF_BUFSZ_SetDrivesMinPosition	2
	#define NF_BUFSZ_SetDrivesMaxPosition	2
	#define NF_BUFSZ_ReadDrivesPosition		2
	#define NF_BUFSZ_ReadDrivesCurrent		2
	#define NF_BUFSZ_SetDrivesMisc			2
	#define NF_BUFSZ_ReadDrivesStatus		2
	
	#define NF_BUFSZ_SetCurrentRegulator	2
	#define NF_BUFSZ_SetSpeedRegulator		2
	#define NF_BUFSZ_SetPositionRegulator	2

//	#define NF_BUFSZ_SetServosMode			28
	#define NF_BUFSZ_SetServosPosition		8
//	#define NF_BUFSZ_SetServosSpeed			28
//	#define NF_BUFSZ_SetServosMaxSpeed		28
//	#define NF_BUFSZ_SetServosMinPosition	28
//	#define NF_BUFSZ_SetServosMaxPosition	28
//	#define NF_BUFSZ_SetServosPositionAdj	28
//	#define NF_BUFSZ_ReadServosPosition		28
//	#define NF_BUFSZ_ReadServosCurrent		28
//	#define NF_BUFSZ_SetServosMisc			28
//	#define NF_BUFSZ_ReadServosStatus		28
											
	#define NF_BUFSZ_SetDigitalOutputs		4
	#define NF_BUFSZ_ReadDigitalInputs		2
	#define NF_BUFSZ_ReadAnalogInputs		8
	
//	#define NF_BUFSZ_ReadDistance			8  
	
//	#define NF_BUFSZ_SetDisplayMode			1
//	#define NF_BUFSZ_SetDisplayText			33
//	#define NF_BUFSZ_SetDisplayBacklight	1

/*
* Remember to declare:
* extern NF_STRUCT_ComBuf	NFComBuf;
*/
