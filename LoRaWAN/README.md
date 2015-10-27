# Nexus-LoRaWAN-Mote
With this code the Nexus will send messages that will be accepted by the LoRaWAN network

This code is minimal code it only sends a message that will be accepted by the LoRaWAN network.
The settings are:

- Channel 0 = 868,1 MHz
- Spreading factor = 7
- Full power

This code will not:
- Receive packages from a gateway
- Change channels
- Change datarates
- Send MAC commands

# Hardware
The Nexus board is fitted with an ATMEGA328P and a RFM95 module

To adapt this code to other hardware change the LoRaWAN_V20.h

From the DIO's only 0,1 and 5 are used

#RFM92
When the Nexus board is fitted with a RFM92 module replace the following files with the files from the RFM92 folder.
- LoRaWAN.ino
- RFM95_V10.cpp
- RFM95_V10.h
