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
/****************************************************************************************
* File:     LoRaWAN_V20.ino
* Author:   Gerben den Hartog
* Compagny: Ideetron B.V.
* Website:  http://www.ideetron.nl/LoRa
* E-mail:   info@ideetron.nl
****************************************************************************************/
/****************************************************************************************
* Created on: 23-10-2015
* Supported Hardware: ID150119-02 Nexus board with RFM95
* 
* Description
* 
* Minimal Uplink for LoRaWAN
* 
* This code demonstrates a LoRaWAN connection on a Nexus board. This code sends a messege every minute
* on chanell 0 (868.1 MHz) Spreading factor 7.
* On every message the frame counter is raised
* 
* This code does not include
* Receiving packets and handeling
* Channel switching
* MAC control messages
* Over the Air joining* 
*
* Firmware version: 1.0
* First version
* 
* Firmware version 2.0
* Working with own AES routine
****************************************************************************************/

/*
*****************************************************************************************
* INCLUDE FILES
*****************************************************************************************
*/
#include <SPI.h>
#include "AES-128_V10.h"
#include "Encrypt_V20.h"
#include "LoRaWAN_V20.h"
#include "RFM95_V10.h"

/*
*****************************************************************************************
* GLOBAL VARIABLES
*****************************************************************************************
*/
unsigned char NwkSkey[16] = {
  0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
  0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};

unsigned char AppSkey[16] = {
  0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
  0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};

unsigned char DevAddr[4] = {
  0x02, 0x01, 0x05, 0x04
};

void setup() 
{
   //Initialize the UART
  Serial.begin(9600);

   //Initialise the SPI port
  SPI.begin();
  SPI.beginTransaction(SPISettings(4000000,MSBFIRST,SPI_MODE0));
  
  //Initialize I/O pins
  pinMode(DS2401,OUTPUT);
  pinMode(MFP,INPUT);
  pinMode(DIO0,INPUT);
  pinMode(DIO1,INPUT); 
  pinMode(DIO5,INPUT);
  pinMode(DIO2,INPUT);
  pinMode(CS,OUTPUT);
  pinMode(LED,OUTPUT);

  digitalWrite(DS2401,HIGH);
  digitalWrite(CS,HIGH);

  //Wait until RFM module is started
  delay(20);   
}

void loop() 
{
  unsigned char i;

  unsigned char RFM_Package_Length = 1;
  unsigned char RFM_Data[64];

  unsigned char Mac_Header = 0x40; //Unconfirmed data up
  unsigned char Mac_Length = 0x00;
  unsigned char MIC[4];

  unsigned char Frame_Control = 0x00;
  unsigned int  Frame_Counter = 0x0000;
  unsigned char Frame_Port = 0x01;

  unsigned char Data[24] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17
  };

  //Set fixed data length
  unsigned char Data_Length = 0x18;

  //initialize the RFM module
  RFM_Init();

  while(1)
  {
    //Encrypt the payload
    Encrypt_Payload(Data, Data_Length, Frame_Counter);

    //Build the Radiopackage
    RFM_Data[0] = Mac_Header;
    RFM_Data[1] = DevAddr[3];
    RFM_Data[2] = DevAddr[2];
    RFM_Data[3] = DevAddr[1];
    RFM_Data[4] = DevAddr[0];
    RFM_Data[5] = Frame_Control;

    RFM_Data[6] = (Frame_Counter & 0x00FF);
    RFM_Data[7] = ((Frame_Counter >> 8) & 0x00FF);

    RFM_Data[8] = Frame_Port;

    //Load Data into package
    for(i = 0; i < Data_Length; i++)
    {
      RFM_Data[i+9] = Data[i];
    }

    Mac_Length = Data_Length + 9;

    //Calculate the MIC for the package
    Calculate_MIC(RFM_Data, MIC, Mac_Length, Frame_Counter);

    //Load MIC in RFM Package
    for(i = 0; i < 4; i++)
    {
      RFM_Data[Mac_Length + i] = MIC[i];
    }

    RFM_Package_Length = Mac_Length + 4;

    //Send Package with RFM
    RFM_Send_Package(RFM_Data, RFM_Package_Length);

    //Rais frame counter
    Frame_Counter++;

    //Reset Data
    for(i = 0; i < 24; i++)
    {
      Data[i] = i;
    }

    //Delay for sending next message set to 1 min
    delay(60000);

  }//While(1)
}
