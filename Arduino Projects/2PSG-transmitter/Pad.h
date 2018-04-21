/*
	Party Pads Sound Game (2PSG) - Pad class header file.

	It is part of objective implementation of the project.
	The class is representing real object - the Pad that you can press, 
	its properties and possible actions.

	Created 19-04-2018
	By Jarosław Bartoszuk (jarobar435)

	https://github.com/jarobar435/party-pads-sound-game
*/

#pragma once

class Pad {
    const String padName;
    const int pin;
    bool state;

  public:
    Pad(String padName, int pin, bool state = true);
    Pad(const Pad &pad);
    void changeState();
    bool getState();
    int getPin();
    const String * getName();
};
