/*
	Party Pads Sound Game (2PSG) - Pad class *.cpp file.

	It is part of objective implementation of the project.
	The class is representing real object - the Pad that you can press, 
	its properties and possible actions. This particular file 
	consists of methods implementations which prototypes can be found in 
	the header (*.h) file of this class.

	Created 19-04-2018
	By Jarosław Bartoszuk (jarobar435)

	https://github.com/jarobar435/party-pads-sound-game
*/

#include <StandardCplusplus.h>
#include <serstream>
#include <string>
#include "Pad.h"

Pad::Pad(String padName, int pin, bool state = true) : padName(padName), pin(pin), state(state) {
};

Pad::Pad(const Pad &pad) : padName(pad.padName), pin(pad.pin), state(pad.state) {
};

void Pad::changeState() {
  state = !state;
};

bool Pad::getState() {
  return state;
};

int Pad::getPin() {
  return pin;
};

const String * Pad::getName() {
  return &padName;
};
