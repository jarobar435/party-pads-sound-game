/*
  Party Pads Sound Game (2PSG) - program for receiver device.

  Program that will receive data from the transmitter with requests to play certain sounds,
  then pass them via USB port (Serial Connection) to computer. Properly configured and working
  at that time AHK script will receive the request and trigger the start or stop  action on demand.
  
  
  The circuit:
    Modules:
    • Arduino Uno R3,
    • NRF24L01 radio frequency transreceiver (2.4GHz).

    Pinout wirings:
    • NRF - Arduino connection:
      * CE - 8
      * CS - 9
      * SS - 10
      * MOSI - 11
      * MISO - 12
      * SCK - 13
    
  Created 19-04-2018
  By Jarosław Bartoszuk (jarobar435)

  https://github.com/jarobar435/party-pads-sound-game
*/

//libraries for NRF:
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radioReceiver(8, 9);
String receivedText;

void setup() {  
  initializeSerialConnection();
  initializeRadioConnection();
}

void initializeSerialConnection() {
  Serial.begin(115200);
  while (!Serial) { };
}

void initializeRadioConnection() {
  radioReceiver.begin();
  radioReceiver.openReadingPipe(0, byte(17));
  radioReceiver.startListening();
}

void loop() {
  checkForUpdates();
}

void checkForUpdates() {
  if (radioReceiver.available()) {
    radioReceiver.read(&receivedText, sizeof(receivedText));    
    Serial.println(receivedText);
  }
}
