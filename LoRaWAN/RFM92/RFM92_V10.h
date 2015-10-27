/******************************************************************************************
*
* File:        RFM92_V10.h
* Author:      Gerben den Hartog
* Compagny:    Ideetron B.V.
* Website:     http://www.ideetron.nl/LoRa
* E-mail:      info@ideetron.nl
******************************************************************************************/

/****************************************************************************************
*
* Created on:         26-10-2015
* Supported Hardware: ID150119-02 Nexus board with RFM92
*
* History:
*
* Firmware version: 1.0
* First version for LoRaWAN
****************************************************************************************/

#ifndef RFM92_V10_H
#define RFM92_V10_H

/*
*****************************************************************************************
* FUNCTION PROTOTYPES
*****************************************************************************************
*/

void RFM_Init();
void RFM_Send_Package(unsigned char *RFM_Tx_Package, unsigned char Package_Length);
unsigned char RFM_Read(unsigned char RFM_Address);
void RFM_Write(unsigned char RFM_Address, unsigned char RFM_Data);

#endif
