#ifndef __MYCAN_H__
#define __MYCAN_H__
void MyCAN_Init();
void MyCAN_Transmit(CanTxMsg *TxMessage);
uint8_t MyCAN_ReceiveFlag();

void MyCAN_RECEIVE(CanRxMsg *CanRxessage);

#endif
