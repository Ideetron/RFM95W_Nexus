/******************************************************************************************
* File:        LoRaWAN_V31.h
* Author:      Gerben den Hartog
* Compagny:    Ideetron B.V.
* Website:     http://www.ideetron.nl/LoRa
* E-mail:      info@ideetron.nl
******************************************************************************************/

/****************************************************************************************
*
* Created on: 			  04-02-2016
* Supported Hardware:	ID150119-02 Nexus board with RFM95
*
* History:
*
* Firmware version: 1.0
* First version for LoRaWAN
*
* Firmware version: 2.0
* Ported to Arduino using own AES encryption
*
* Firmware version 3.0
* Added receiving in receive slot 2
*
* Firmware version 3.1
* Removed a bug from the Waitloop file
****************************************************************************************/

#ifndef LORAWAN_V31_H
#define LORAWAN_V31_H

/*
*****************************************************************************************
* DEFINES
*****************************************************************************************
*/

#define DS2401  2
#define MFP     3
#define DIO0    4
#define DIO1    5
#define DIO5    6
#define DIO2    7
#define CS      8
#define LED     9

#endif
