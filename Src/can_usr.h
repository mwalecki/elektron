/*
 * CAN_usr.h
 *
 *  Created on: Jul 1, 2015
 *      Author: wacek
 */

#ifndef CAN_USR_H_
#define CAN_USR_H_


void requestStatus(uint32_t *status, uint8_t *mode);
void requestPosition(int32_t *position);
void requestVelocity(int32_t *velocity);
void setMode(uint8_t mode);
void setPWM(int16_t pwm);
void setTrq(int16_t trq);
void setVel(int32_t vel);
void setPos(int32_t pos);

#endif /* CAN_USR_H_ */
