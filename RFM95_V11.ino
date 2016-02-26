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
* File:     RFM95_V11.ino
* Author:   Gerben den Hartog
* Compagny: Ideetron B.V.
* Website:  http://www.ideetron.nl/LoRa
* E-mail:   info@ideetron.nl
****************************************************************************************/
/****************************************************************************************
* Created on: 2-10-2015
* Supported Hardware: ID150119-02 Nexus board with RFM95
* 
* Description
* Code to setup a working connection between two Nexus boards.
* The Nexus will send a predefined message when it receives the character S trough the serial connection
*
* Firmware version: 1.1
* First version
* 
* 30-11-2015 [Bart Hiddink]
* Added RSSI to output
****************************************************************************************/

#include <SPI.h>

//Define pin description
#define DS2401  2
#define MFP     3
#define DIO0    4
#define DIO1    5 
#define DIO5    6
#define DIO2    7
#define CS      8
#define LED     9

//Define other constants
#define PACKAGE_LENGTH 10

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
  unsigned char UART_Data;
  unsigned char RFM_Package[PACKAGE_LENGTH];
  unsigned char RFM_RSSI;

  RFM_Package[0] = 0x12; RFM_Package[1] = 0x34; RFM_Package[2] = 0x56; RFM_Package[3] = 0x78; RFM_Package[4] = 0x9A;
  RFM_Package[5] = 0xBC; RFM_Package[6] = 0xDE; RFM_Package[7] = 0xF0; RFM_Package[8] = 0xAA; RFM_Package[9] = 0x00;

  //Initialieze the RFM module
  RFM_Init();

  //Set led on to inidicate the Nexus has started up
  digitalWrite(LED,HIGH);

  //Main loop
  while(1)
  {
    //Check if there is serial data
    if(Serial.available()  != 0)
    {
      UART_Data = Serial.read();
    }

    //Check if the serial data
    if(UART_Data == 'S')
    {
      RFM_SendPackage(RFM_Package);
      RFM_Package[9]++;
      UART_Data = 0x00;
    }

    //Check if the RFM has received a package
    if(digitalRead(DIO0) == HIGH)
    {
      //Get package from RFM
      RFM_ReadPackage(RFM_Package);

      //Send package over Serial
      for(i = 0; i < PACKAGE_LENGTH; i++)
      {
        Serial.write(RFM_Package[i]);
      }
      //Get RSSI register
      RFM_RSSI = RFM_Read(0x1A);
      Serial.write(RFM_RSSI);      
    }
      
  }
}

/*
*****************************************************************************************
* Description  :   Initialisation of the RFM module. 
*                  Check the datasheet if you want to use other settings
*****************************************************************************************
*/
void RFM_Init()
{
  //Setting RFM in sleep
  RFM_Write(0x01,0x00);

  //Switch to LoRa mode
  RFM_Write(0x01,0x80);

  //Setting RFM to standby
  RFM_Write(0x01,0x81);
  //Wait for mode ready
  while(digitalRead(DIO5) == LOW)
  {
    
  }

  //Set carrier frequency
  //868.000 MHz / 61.035 Hz = 14221348 = 0xD90024
  RFM_Write(0x06,0xD9);
  RFM_Write(0x07,0x00);
  RFM_Write(0x08,0x24);

  //Set Pa pin to maximal power
  RFM_Write(0x09,0xFF);

  //Bandwith 250 kHz, Coding rate = 4/8, Implicit header mode
  RFM_Write(0x1D,0x89);

  //Spreading factor 6, PayloadCRC on
  RFM_Write(0x1E,0x64);
  //Setting additional register for SF6 use other settings for the other SF
  RFM_Write(0x31,0xC5);
  RFM_Write(0x37,0x0C);

  //Preamble length 0x0018 + 4 = 28
  RFM_Write(0x20,0x00);
  RFM_Write(0x21,0x18);

  //Payload length
  RFM_Write(0x22,PACKAGE_LENGTH);

  //Set RFM in continues receive
  RFM_Write(0x01,0x85);
  //Wait for mode ready
  while(digitalRead(DIO5) == LOW)
  {
    
  }  
}

/*
*****************************************************************************************
* Description:  Function is used to read a value from a specific register of the RFM module
*
* Arguments:    RFM_Adress  Adress of the register to read
*
* Return:       The value of the register is returned
*****************************************************************************************
*/
unsigned char RFM_Read(unsigned char RFM_Address)
{
  unsigned char RFM_Data;

  //Set NSS pin low to start SPI communication
  digitalWrite(10,LOW);

  //Send Address
  SPI.transfer(RFM_Address);
  //Send 0x00 to be able to receive the answer from the RFM
  RFM_Data = SPI.transfer(0x00);

  //Set NSS high to end communication
  digitalWrite(10,HIGH);

  //Return received data
  return RFM_Data;
}

/*
*****************************************************************************************
* Description:    Function is used to write a value to a specific register of the RFM module
*
* Arguments:      RFM_Adress  Adress of the register to be written
                  RFM_Data    Data that will be written
*****************************************************************************************
*/
void RFM_Write(unsigned char RFM_Address, unsigned char RFM_Data)
{
  //Set NSS pin Low to start communication
  digitalWrite(10,LOW);

  //Send Addres with MSB 1 to make it a writ command
  SPI.transfer(RFM_Address | 0x80);
  //Send Data
  SPI.transfer(RFM_Data);

  //Set NSS pin High to end communication
  digitalWrite(10,HIGH);
}

/*
*****************************************************************************************
* Description  :   Function is used to send a message of 10 bytes
*
* Arguments   :   *RFM_Package Pointer to the array with the data to send
*****************************************************************************************
*/
void RFM_SendPackage(unsigned char *RFM_Package)
{
  unsigned char i;

  //Switch RFM to standby
  RFM_Write(0x01,0x81);
  //Wait for mode ready
  while(digitalRead(DIO5) == LOW)
  {
  }

  //Set SPI pointer to Tx base address
  RFM_Write(0x0D,0x80);

  //Switch DIO to TxDone
  RFM_Write(0x40,0x40);

  //Write payload in to the fifo
  for(i = 0; i < PACKAGE_LENGTH; i++)
  {
    RFM_Write(0x00,*RFM_Package);
    RFM_Package++;
  }

  //Switch RFM to TX
  RFM_Write(0x01,0x83);

  //Wait on TxDone
  while(digitalRead(DIO0) == LOW)
  {
    
  }

  //Clear interrupt
  RFM_Write(0x12,0x08);

  //Set DIO0 to RxDone
  RFM_Write(0x40,0x00);

   //Set RFM in continues receive
  RFM_Write(0x01,0x85);
  //Wait for mode ready
  while(digitalRead(DIO5) == LOW)
  {
    
  }    
}

/*
*****************************************************************************************
* Description:  Function is to get a message of 10 bytes received by the RFM
*
* Arguments:    *RFM_Package  Pointer to the array with the data to send
*****************************************************************************************
*/
void RFM_ReadPackage(unsigned char *RFM_Package)
{
  unsigned char i;
  unsigned char RFM_Interrupt;
  unsigned char RFM_PackageLocation;

  //Get interrupt register
  RFM_Interrupt = RFM_Read(0x12);

  //Switch RFM to Standby
  RFM_Write(0x01,0x81);
  //Wait for mode ready
  while(digitalRead(DIO5) == LOW)
  {
  }

  //Clear interrupt register
  RFM_Write(0x12,0x60);

  //Get package location
  RFM_PackageLocation = RFM_Read(0x10);

  //Set SPI pointer to Packagelocation
  RFM_Write(0x0D,RFM_PackageLocation);

  //Get message
  for(i = 0; i < PACKAGE_LENGTH; i++)
  {
    *RFM_Package = RFM_Read(0x00);
    RFM_Package++;
  }

  //Switch RFM to receive
  RFM_Write(0x01,0x85);
  //Wait for mode ready
  while(digitalRead(DIO5) == LOW)
  {
    
  }    
}
