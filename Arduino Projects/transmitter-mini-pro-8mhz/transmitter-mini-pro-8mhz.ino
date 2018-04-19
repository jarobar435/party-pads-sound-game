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
#include <string>
#include <vector>
#include <iterator>
//libraries for NRF:
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

using namespace std;

class Pad {
    const String padName;
    const int pin;
    bool state;

  public:
    Pad(String padName, int pin, bool state = true) : padName(padName), pin(pin), state(state) {
    }

    Pad(const Pad &pad) : padName(pad.padName), pin(pad.pin), state(pad.state) {
    }

    void changeState() {
      state = !state;
    }

    bool getState() {
      return state;
    }

    int getPin() {
      return pin;
    }

    const String * getName() {
      return &padName;
    }
};

vector<Pad*> pads;
RF24 radioTransmitter(14, 15);
const String startText = " start";
const String stopText = " stop";

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

/*
  addPads is prepared purely for MiniPro
  20 pins that can work as digital inputs
  (minus 6 needed for NRF module)
*/
void addPads() {
  for (int i = 0; i < 14; i++) {
    if (i < 10)
      pads.push_back(new Pad(generateName(i), i));
    else
      pads.push_back(new Pad(generateName(i), i + 4));
  }
}

String generateName(int number) {
  return "Pad #" + String(number + 1) + " ";
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
    if (digitalRead(pads[i]->getPin()) != pads[i]->getState()) {
      pads[i]->changeState();
      sendRequest(pads[i]);
      delay(10);
    }
  }
}

void sendRequest(Pad* pad) {
  radioTransmitter.write(pad->getName(), sizeof(pad->getName()));
  if (!pad->getState())
    radioTransmitter.write(&startText, sizeof(startText));
  else
    radioTransmitter.write(&stopText, sizeof(stopText));
}


