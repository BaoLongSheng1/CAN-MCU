#include "stm32f10x.h"                  // Device header
#ifndef __MYCAN_H__
#define __MYCAN_H__
void MyCan_Init(void);
void MyCAN_Transmit(CanTxMsg *TxMessage);
uint8_t MyCAN_ReceiveFlag(void);
void MyCAN_RECEIVE(CanRxMsg *CanRxessage);
#endif
