/******************************************************************************************
*
* File:        Waitloop_V11.h
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
* History:
*
* Firmware version: 1.0
* First version
*
* Firmware version: 1.1
* Removed a bug from the waitloop file
****************************************************************************************/

#ifndef WAITLOOP_V11_H
#define WAITLOOP_V11_H

/*
*****************************************************************************************
* FUNCTION PROTOTYPES
*****************************************************************************************
*/

void WaitLoop_Init();
void WaitLoop(unsigned char ms);

#endif
