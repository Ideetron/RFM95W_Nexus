/******************************************************************************************
* Copyright 2015, 2016 Ideetron B.V.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************************/
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
