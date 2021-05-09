/*
  LEDDisplay.h - Library for controlling a 4 Digit 7 Segment Display via an 8-Bit Shift Register, using an Atmel based MCU
  Created by Zachary Williams. Dec 05, 2015
*/
#ifndef LEDDisplay_h
#define LEDDisplay_h

#include "Arduino.h"

class LEDDisplay
{
  public:
	// Constructor
    LEDDisplay(int ClockPin, int LatchPin, int DataPin, int DigitPins[4]);
	//Public Properties
	
    // Public Methods
	void UpdateDisplay();
	bool DisplayThis(char Message[]);
	bool SetDecimalPoint(int DecIndex, bool IsShown); // Manipulates _decimalLocation Array turning on/off each digits Decimal Place
	char* _currentMessage[4]; // Hold Character Array that Represents the currently Displayed Message
  private:
	// Private Properties
	int _clockPin; // Clock Pin to Shift Register
	int _latchPin; // Latch Pin to Shift Register
	int _dataPin; // Data Pin to Shift Register
	int _digitPins[4]; // Pins Used on MCU to either power LEDs directly or as with the original design, These pins trigger NPN Transistors configured as switches.
	int _digitScan = 0; // Hold Digit Index currently being Updated by UpdateDisplay Method
	int _digitBuffer[4] = {0}; // Holds what byte to display in which place.
	bool _decimalLocation[4] = {false, false, false, false}; // Hold Array of TRUE/FALSE values representing whether or not to turn on the decimal place for the digit relative to array index.
	
	const byte _digit[30] =      //seven segment digits in bits
	{
	  B00111111, //0
	  B00000110, //1
	  B01011011, //2
	  B01001111, //3
	  B01100110, //4
	  B01101101, //5
	  B01111101, //6
	  B00000111, //7
	  B01111111, //8
	  B01101111, //9
	  B01110111, //A - 10
	  B01111100, //b - 11
	  B00111001, //C - 12
	  B01011000, //c - 13
	  B01011110, //d - 14
	  B01111001, //E - 15
	  B01110001, //F - 16
	  B01110110, //H - 17
	  B00011110, //J - 18
	  B00111000, //L - 19
	  B01010100, //n - 20
	  B01011100, //o - 21
	  B01110011, //P - 22
	  B01100111, //q - 23
	  B01010000, //r - 24
	  B01101101, //S - 25
	  B01111000, //t - 26	  
//	  B01000110, //t - 26
	  B00011100, //u - 27
	  B01100110, //y - 28
	  B00000000  //Digit OFF
	};
	
	// Private Methods
	
};

#endif