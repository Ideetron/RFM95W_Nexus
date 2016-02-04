/******************************************************************************************
*
* File:        Waitloop_V11.cpp
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

/*
*****************************************************************************************
* INCLUDE FILES
*****************************************************************************************
*/

#include "Arduino.h"
#include "Waitloop_V11.h"


/*
*****************************************************************************************
* Description  : Initialize Timer2
*****************************************************************************************
*/
void WaitLoop_Init()
{
  ASSR = 0x00;
  //Switch of interrupts
  TIMSK2 = 0x00;
  //Initialize timer 2 Normal operation
  TCCR2A = 0x00;
  //Set prescaler to 1:64
  TCCR2B = 0x04;
  //Set Comperator register to 1 ms
  OCR2A = 0xFA;
}


/*
*****************************************************************************************
* Description	: Waitloop for waiting a given number of miliseconds
*
* Arguments		: ms number of milisecond to wait
*****************************************************************************************
*/
void WaitLoop(unsigned char ms)
{
  unsigned char Counter = 0x00;

  //Clear Timer 2
  TCNT2 = 0x00;
  //Clear compare flag
  TIFR2 = 0x02;

  while(Counter < ms)
  {
    if((TIFR2 & 0x02) == 0x02)
    {
      Counter++;
      //Clear timer register
      TCNT2 = 0x00;
      //Clear compare flag
      TIFR2 = 0x02;
    }
  }
}
