/*
  Party Pads Sound Game (2PSG) - program for transmitter device.

  Program that will handle the inputs from Floor Pads and send them via radio waves
  to another device (another Arduino - NRF pair working as a receiver.
  That way, by pressing on Floor Pad you will be able to trigger any sound file 
  (most likely premade samples) from your computer.
  
  The circuit:
    Modules:
    • Arduino Mini Pro 3.3V 8MHz,
    • NRF24L01 radio frequency transreceiver (2.4GHz),
    • DIY Floor Pads - pushbuttons made of cardboard, sponge and aluminium foil.

    Pinout wirings:
    • NRF - Arduino connection:
      * SS - 10
      * MOSI - 11
      * MISO - 12
      * SCK - 13
      * CE - 14 (A0)
      * CS - 15 (A1)
     • Pads connected to arduino on pins:
      * Digital Inputs from 0 to 9
      * Analog Inputs from A2 to A5 (treated as digital from 14 to 17)
    
  Created 19-04-2018
  By Jarosław Bartoszuk (jarobar435)

  https://github.com/jarobar435/party-pads-sound-game
*/

#include <StandardCplusplus.h>
#include <serstream>
#include <iterator>
#include <string>
#include <vector>
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>
#include "Pad.h"

using namespace std;

/* 
 * Warning - program uses every single available 
 * Digital Port of Arduino Mini Pro
 * changing values below will create the necessity
 * to modify range of pins in generatePinNumber().
 */
const int 
  CE_PIN = 14,
  CS_PIN = 15;
  
vector<Pad*> pads;
RF24 radioTransmitter(CE_PIN, CS_PIN);    
const String START_TEXT = "start";
const String STOP_TEXT = "stop";

void setup() {
  initializeRadioConnection();
  addPads();
  initializePadInputs();
}

void initializeRadioConnection() {
  radioTransmitter.begin();
  radioTransmitter.setRetries(15, 15);
  radioTransmitter.openWritingPipe(byte(17));
  radioTransmitter.stopListening();
}

void addPads() {
  for (int i = 0; i < 14; i++)
    pads.push_back(new Pad(generateName(i), generatePinNumber(i)));
}

int generatePinNumber(int iterator) {
  if (iterator < 10)
    return iterator;
  else
    return iterator + 4;
}

String generateName(int iterator) {
  return "Pad #" + String(iterator + 1) + " ";
}

void initializePadInputs() {
  for (int i = 0; i < pads.size(); i++) {
    pinMode(pads[i]->getPin(), INPUT_PULLUP);
  }
}

void loop() {
  updatePadsStates();
}

void updatePadsStates() {
  for (int i = 0; i < pads.size(); i++) {
    if (pinChangedState(i)) {
      saveNewState(i);
      sendInformationToReceiver(pads[i]);
      delay(10);
    }
  }
}

bool pinChangedState(int padNumber) {
  if(currentState(padNumber) != savedState(padNumber))
    return true;
  else
    return false;
}

bool currentState(int padNumber) {
  return digitalRead(pads[padNumber]->getPin());
}

bool savedState(int padNumber) {
  return pads[padNumber]->getState();
}

void saveNewState(int padNumber) {
  pads[padNumber]->changeState();
}

void sendInformationToReceiver(Pad* pad) {
  sendPadName(pad->getName());
  sendActionIndicator(pad->getState());
}

void sendPadName(const String * padName) {
  radioTransmitter.write(padName, sizeof(padName));
}

void sendActionIndicator(bool state) {
  if(state)
    radioTransmitter.write(&START_TEXT, sizeof(START_TEXT));
  else
    radioTransmitter.write(&STOP_TEXT, sizeof(STOP_TEXT));
}

