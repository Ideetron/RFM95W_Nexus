/******************************************************************************************
*
* File:        RFM95_V21.h
* Author:      Gerben den Hartog
* Compagny:    Ideetron B.V.
* Website:     http://www.ideetron.nl/LoRa
* E-mail:      info@ideetron.nl
******************************************************************************************/

/****************************************************************************************
*
* Created on:         04-02-2016
* Supported Hardware: ID150119-02 Nexus board with RFM95
*
* History:
*
* Firmware version: 1.0
* First version for LoRaWAN
*
* Firmware version: 2.0
* Created function to read received package
* Switching to rigth receive channel
* Moved waiting on RxDone or Timeout to RFM file
* Moved CRC check to RFM file
*
* Firmware version: 2.1
* Switching to standby at start of send package function
****************************************************************************************/

#ifndef RFM95_V21_H
#define RFM95_V21_H

/*
*****************************************************************************************
* TYPE DEFENITIONS
*****************************************************************************************
*/

typedef enum {NO_MESSAGE,CRC_OK,MIC_OK,MESSAGE_DONE,TIMEOUT,WRONG_MESSAGE} message_t;

/*
*****************************************************************************************
* FUNCTION PROTOTYPES
*****************************************************************************************
*/

void RFM_Init();
void RFM_Send_Package(unsigned char *RFM_Tx_Package, unsigned char Package_Length);
unsigned char RFM_Get_Package(unsigned char *RFM_Rx_Package);
unsigned char RFM_Read(unsigned char RFM_Address);
void RFM_Write(unsigned char RFM_Address, unsigned char RFM_Data);
message_t RFM_Receive();

#endif
