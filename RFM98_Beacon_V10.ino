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
* File:     RFM98_Beacon_V10.ino
* Author:   Gerben den Hartog
* Compagny: Ideetron B.V.
* Website:  http://www.ideetron.nl/LoRa
* E-mail:   info@ideetron.nl
****************************************************************************************/
/****************************************************************************************
* Created on: 05-11-2015
* Supported Hardware: ID150119-02 Nexus board with RFM98
* 
* Description
* Send a message with the unique ID and a message counter every second. Can be received by anohter Nexus board programmed with
* RFM98_V10.ino
*
* Firmware version: 1.0
* First version
****************************************************************************************/

#include <SPI.h>
#include <Wire.h>

//Define pin description
#define DS2401  2
#define MFP     3
#define DIO0    4
#define DIO1    5 
#define DIO5    6
#define DIO2    7
#define CS      8
#define LED     9
#define NSS     10
#define BAUD    9600

//Define other constants
#define PACKAGE_LENGTH 10

void setup() 
{
  //Initialize the UART
  Serial.begin(BAUD);

  //Initialise the SPI port
  SPI.begin();
  SPI.beginTransaction(SPISettings(4000000,MSBFIRST,SPI_MODE0));

  //Initialize the I2C bus
  Wire.begin();

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
  unsigned char Package_Counter = 0x00;
  unsigned char Pulse_Status = 0x00;
  unsigned char UART_Data;
  unsigned char DS_Bytes[8];
  unsigned char RFM_Tx_Package[PACKAGE_LENGTH];
  unsigned char RFM_Rx_Package[PACKAGE_LENGTH];

  //Get DS bytes
  DS_Read(DS_Bytes);

  //Set DS bytes in package
  RFM_Tx_Package[0] = DS_Bytes[0];
  RFM_Tx_Package[1] = DS_Bytes[1];
  RFM_Tx_Package[2] = DS_Bytes[2];
  RFM_Tx_Package[3] = DS_Bytes[3];
  RFM_Tx_Package[4] = DS_Bytes[4];
  RFM_Tx_Package[5] = DS_Bytes[5];
  RFM_Tx_Package[6] = DS_Bytes[6];
  RFM_Tx_Package[7] = DS_Bytes[7];
  RFM_Tx_Package[8] = 0xAA;
  RFM_Tx_Package[9] = Package_Counter;
  
  //Initialise the RFM module
  RFM_Init();

  //Initialise the RTCC clock
  RTCC_Init();

  //Set led on to inidicate the Nexus has started up
  digitalWrite(LED,HIGH);

  //Main loop
  while(1)
  {
    //New pulse
    if(digitalRead(MFP) == 1 && Pulse_Status == 0x00)
    {
      Pulse_Status = 0x01;
      RFM_SendPackage(RFM_Tx_Package);
      Package_Counter++;
      RFM_Tx_Package[9] = Package_Counter;
      digitalWrite(LED,HIGH);
    }

    if(digitalRead(MFP) == 0 && Pulse_Status == 0x01)
    {
      Pulse_Status = 0x00;
      digitalWrite(LED,LOW);
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
  //433.175 MHz / 61.035 Hz = 7097157 = 0x6C4B45
  RFM_Write(0x06,0x6C);
  RFM_Write(0x07,0x4B);
  RFM_Write(0x08,0x45);

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
  digitalWrite(NSS,LOW);

  //Send Address
  SPI.transfer(RFM_Address);
  //Send 0x00 to be able to receive the answer from the RFM
  RFM_Data = SPI.transfer(0x00);

  //Set NSS high to end communication
  digitalWrite(NSS,HIGH);

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
  digitalWrite(NSS,LOW);

  //Send Addres with MSB 1 to make it a writ command
  SPI.transfer(RFM_Address | 0x80);
  //Send Data
  SPI.transfer(RFM_Data);

  //Set NSS pin High to end communication
  digitalWrite(NSS,HIGH);
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

/*
*****************************************************************************************
* Description:  Function is used to read the all the bytes provide by the DS2401
*
* Arguments:    *DS_Bytes pointer to an array of 8 unsigned chars
*****************************************************************************************
*/
void DS_Read(unsigned char *DS_Bytes)
{
  unsigned char i;

  //Preform reset pulse
  pinMode(DS2401,OUTPUT);
  digitalWrite(DS2401,LOW);
  delayMicroseconds(700);
  pinMode(DS2401,INPUT);
  delayMicroseconds(700);

  //Send command 0x33 LSB
  DS_WR1();
  DS_WR1();
  DS_WR0();
  DS_WR0();
  DS_WR1();
  DS_WR1();
  DS_WR0();
  DS_WR0();

  //Read DS bytes
  for (i = 0; i < 8; i++)
  {
    *DS_Bytes = DS_ReadByte();
    DS_Bytes++;
  }

  //Shutdonw DS2401
  digitalWrite(DS2401,HIGH);  
}

/*
*****************************************************************************************
* Description:  Function is used to write a logical 1 to the DS2401
*****************************************************************************************
*/
void DS_WR1()
{
  pinMode(DS2401,OUTPUT);
  digitalWrite(DS2401,LOW);
  delayMicroseconds(2);
  pinMode(DS2401,INPUT);
  delayMicroseconds(80);
}

/*
*****************************************************************************************
* Description:  Function is used to write a logical 0 to the DS2401
*****************************************************************************************
*/
void DS_WR0()
{
  pinMode(DS2401,OUTPUT);
  digitalWrite(DS2401,LOW);
  delayMicroseconds(80);
  pinMode(DS2401,INPUT);
  delayMicroseconds(2);
}

/*
*****************************************************************************************
* Description:  Function is used to read the one byte provided by the DS2401
*
* Return:       Returns the byte received from the DS2401
*****************************************************************************************
*/
unsigned char DS_ReadByte()
{
  unsigned char DS_Byte = 0x00;
  unsigned char i, t = 0x01;

  for (i = 0; i < 8; i++)
  {
    pinMode(DS2401,OUTPUT);
    digitalWrite(DS2401,LOW);
    delayMicroseconds(2);
    pinMode(DS2401,INPUT);
    delayMicroseconds(10);
    if(digitalRead(DS2401) == 1)
    {
      DS_Byte = DS_Byte | t;
    }

    t = t * 2;
    delayMicroseconds(80);
  }
  return DS_Byte;
}

void RTCC_Init()
{
  I2C_Write(0x6F,0x00,0x80);
  I2C_Write(0x6F,0x07,0x40);  
}

/*
*****************************************************************************************
* Description:  Function is used read a value of a specific register from a specific device on the I2C bus
* 
* Arguments:    Address:  Address of the I2C device
*               Register: The register to read
*
* Return:       Returns the value of the register
*****************************************************************************************
*/
unsigned char I2C_Read(unsigned char Address, unsigned char Register)
{
  unsigned char Data;
  unsigned char Number_Bytes = 0x01;
  
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();
  Wire.requestFrom(Address,Number_Bytes);
  Data = Wire.read();

  return Data;
}

/*
*****************************************************************************************
* Description:  Function is used write data to a specific register from a specific device on the I2C bus
* 
* Arguments:    Address:  Address of the I2C device
*               Register: The register to write
*               Data:     The data to be written
*
* Return:       Returns the value of the register
*****************************************************************************************
*/
void I2C_Write(unsigned char Address, unsigned char Register, unsigned char Data)
{
    Wire.beginTransmission(Address);
    Wire.write(Register);  // GYRO_CONFIG register
    Wire.write(Data);  // Setting register to 11100011 in hex
    Wire.endTransmission();
}
