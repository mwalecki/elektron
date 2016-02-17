#include "stm32f10x.h"
#include "nf/nfv2.h"
#include "systick.h"
#include "can_usr.h"

extern volatile NF_STRUCT_ComBuf 	NFComBuf;
extern volatile STDOWNCNT_St		STDownCnt[ST_Downcounters];

void requestStatus(uint32_t *status, uint8_t *mode) {
	((uint16_t*)status)[0] = NFComBuf.ReadDrivesStatus.data[0];
	((uint16_t*)status)[1] = NFComBuf.ReadDigitalInputs.data[0];
//	mode[0] = Motor.stateMode;
}

void requestPosition(int32_t *position) {
//	*position = Motor.measureCurrentPosition;
}

void requestVelocity(int32_t *velocity) {
//	*velocity = Motor.measureCurrentIncrement;
}

void setMode(uint8_t mode) {
	NFComBuf.SetDrivesMode.data[0] = mode;
}

void setPWM(int16_t pwm) {
	NFComBuf.SetDrivesPWM.data[0] = pwm;
}

void setTrq(int16_t trq) {

}

void setVel(int32_t vel) {
	NFComBuf.SetDrivesSpeed.data[0] = vel;
//	ST_Reset(ST_CommandWD);
}

void setPos(int32_t pos) {
	NFComBuf.SetDrivesPosition.data[0] = pos;
}
