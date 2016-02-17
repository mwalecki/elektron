#include "stm32f10x.h"
#include "nf/nfv2.h"
#include "can.h"
#include "can_usr.h"
#include "io.h"

extern volatile NF_STRUCT_ComBuf 	NFComBuf;
extern volatile struct CommunicationSettings comm_set;

uint16_t devid;

CanRxMsg can_rx_message;

void canConfig(void) {
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

  /* AFIO clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	/* GPIO clock enable */
	RCC_APB2PeriphClockCmd(CAN_RX_APB2 | CAN_TX_APB2, ENABLE);
	/* Configure CAN pin: RX */
	IoPinConfig(CAN_RX_GPIO,  CAN_RX_PIN,  GPIO_Speed_50MHz, GPIO_Mode_IPU);
	/* Configure CAN pin: TX */
  IoPinConfig(CAN_TX_GPIO,  CAN_TX_PIN,  GPIO_Speed_50MHz, GPIO_Mode_AF_PP);

  /* CANx Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	/* CANx Pin Remap */
	GPIO_PinRemapConfig(GPIO_Remap2_CAN1 , ENABLE);

	/* CAN register init */
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

	/* CAN Baudrate = 1MBps*/
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
  //CAN_InitStructure.CAN_Prescaler = 3;
  CAN_InitStructure.CAN_Prescaler = 3; // 250kbps
	CAN_Init(CAN1, &CAN_InitStructure);


	//####################
	/*
	void CAN_Configuration(void)
	{
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = ENABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_10tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;
	CAN_InitStructure.CAN_Prescaler = 8;
	CAN_Init(CAN1, &CAN_InitStructure);
	CAN_FilterInitStructure.CAN_FilterNumber=0;
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	}
	*/
	//#############


	devid = NFComBuf.myAddress;
	/* CAN filter init */

	// filter single receiver messages
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0011;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x00ff;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

//	// filter dual receiver messages
//	CAN_FilterInitStructure.CAN_FilterNumber = 1;
//	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
//	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
//	CAN_FilterInitStructure.CAN_FilterIdHigh = (16 | (devid/2)) << 5;
//	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (16 | 15) << 5;
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
//	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
//	CAN_FilterInit(&CAN_FilterInitStructure);
//
//	// filter quad receiver messages
//	CAN_FilterInitStructure.CAN_FilterNumber = 2;
//	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
//	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
//	CAN_FilterInitStructure.CAN_FilterIdHigh = (24 | (devid/4)) << 5;
//	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (16 | 15) << 5;
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
//	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
//	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

CanRxMsg rx_message;


void CAN1_RX0_IRQHandler(void)
{
  CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
  canProcess(&rx_message);
}

#define MSG_MASK (63 << 5)

#define MSG_POS_MSR 48
#define MSG_VEL_MSR 49
#define MSG_POS_AND_VEL_MSR 50

#define MSG_SET_PARAM 53
#define MSG_GET_PARAM 54
#define MSG_GET_PARAM_REP 55

#define MSG_MODE_CMD 56
#define MSG_STATUS 57
#define MSG_SW_VER 58

#define MSG_PWM_CMD 16
#define MSG_TRQ_CMD 17

#define MSG_VEL_CMD 32
#define MSG_POS_CMD 33

void canProcess(CanRxMsg *rx) {
  CanTxMsg tx_message;

  can_rx_message = *rx;

  if(rx->RTR == CAN_RTR_REMOTE) {
    if((rx->StdId & MSG_MASK) == (MSG_STATUS << 5)) {
      tx_message.StdId = (MSG_STATUS << 5) | devid;
      tx_message.ExtId = 0x01;
      tx_message.RTR = CAN_RTR_DATA;
      tx_message.IDE = CAN_ID_STD;
      tx_message.DLC = 5;

      requestStatus((uint32_t*)&tx_message.Data[0], (uint8_t*)&tx_message.Data[4]);

      CAN_Transmit(CAN1, &tx_message);
    } else if((rx->StdId & MSG_MASK) == (MSG_POS_MSR << 5)) {
      tx_message.StdId = (MSG_POS_MSR << 5) | devid;
      tx_message.ExtId = 0x01;
      tx_message.RTR = CAN_RTR_DATA;
      tx_message.IDE = CAN_ID_STD;
      tx_message.DLC = 4;

      requestPosition((int32_t*)&tx_message.Data[0]);

      CAN_Transmit(CAN1, &tx_message);
    } else if((rx->StdId & MSG_MASK) == (MSG_VEL_MSR << 5)) {
      tx_message.StdId = (MSG_VEL_MSR << 5) | devid;
      tx_message.ExtId = 0x01;
      tx_message.RTR = CAN_RTR_DATA;
      tx_message.IDE = CAN_ID_STD;
      tx_message.DLC = 4;

      requestVelocity((int32_t*)&tx_message.Data[0]);

      CAN_Transmit(CAN1, &tx_message);
    } else if((rx->StdId & MSG_MASK) == (MSG_POS_AND_VEL_MSR << 5)) {
      tx_message.StdId = (MSG_POS_AND_VEL_MSR << 5) | devid;
      tx_message.ExtId = 0x01;
      tx_message.RTR = CAN_RTR_DATA;
      tx_message.IDE = CAN_ID_STD;
      tx_message.DLC = 8;

      requestPosition((int32_t*)&tx_message.Data[0]);
      requestVelocity((int32_t*)&tx_message.Data[4]);

      CAN_Transmit(CAN1, &tx_message);
    }
  } else {
    if((rx->StdId & MSG_MASK) == (MSG_MODE_CMD << 5)) {
      setMode(rx->Data[0]);
    } else if((rx->StdId & MSG_MASK) == (MSG_PWM_CMD << 5)) {
      setPWM(*(int16_t*)&rx->Data[(devid%2)*2]);
    } else if((rx->StdId & MSG_MASK) == (MSG_TRQ_CMD << 5)) {
      setTrq(*(int16_t*)&rx->Data[(devid%2)*2]);
    } else if((rx->StdId & MSG_MASK) == (MSG_VEL_CMD << 5)) {
      setVel(*(int32_t*)&rx->Data[(devid%2)*4]);
    } else if((rx->StdId & MSG_MASK) == (MSG_POS_CMD << 5)) {
      setPos(*(int32_t*)&rx->Data[(devid%2)*4]);
    }

  }

}

void canTestMessage(CanRxMsg *rx) {
  CanTxMsg tx_message;


  tx_message.StdId=0x00;
  tx_message.RTR=CAN_RTR_DATA;
  tx_message.IDE=CAN_ID_STD;
  tx_message.DLC=2;
  tx_message.Data[0]=0xCA;
  tx_message.Data[1]=0xFE;

  CAN_Transmit(CAN1, &tx_message);
}



