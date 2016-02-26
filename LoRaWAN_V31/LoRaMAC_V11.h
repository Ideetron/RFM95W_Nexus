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
* File:        LoRaMAC_V11.h
* Author:      Gerben den Hartog
* Compagny:    Ideetron B.V.
* Website:     http://www.ideetron.nl/LoRa
* E-mail:      info@ideetron.nl
******************************************************************************************/
/****************************************************************************************
*
* Created on: 			  04-02-2016
* Supported Hardware: ID150119-02 Nexus board with RFM95
*
* Firmware version: 1.0
* First version
*
* Firmware version: 2.1
* Removed a bug from the waitloop file
****************************************************************************************/

#ifndef LORAMAC_V11_H
#define LORAMAC_V11_H

/*
*****************************************************************************************
* FUNCTION PROTOTYPES
*****************************************************************************************
*/

unsigned char LORA_Cycle(unsigned char *Data_Tx, unsigned char *Data_Rx, unsigned char Data_Length_Tx);
void LORA_Send_Data(unsigned char *Data, unsigned char Data_Length, unsigned int Frame_Counter_Up);
unsigned char LORA_Receive_Data(unsigned char *Data);

#endif
