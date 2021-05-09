/*
  LEDDisplay.cpp - Library for controlling a 4 Digit 7 Segment Display via an 8-Bit Shift Register, using an Atmel based MCU
  Created by Zachary Williams. Dec 05, 2015
*/

#include "Arduino.h"
#include "LEDDisplay.h"

// Class Constructor
LEDDisplay::LEDDisplay(int ClockPin, int LatchPin, int DataPin, int DigitPins[4])
{
	this->_latchPin = LatchPin; // Set Latch Pin inside Object
	this->_clockPin = ClockPin; // Set Clock Pin inside Object
	this->_dataPin = DataPin; // Set Data Pin inside Object
	pinMode(LatchPin, OUTPUT); // Set LatchPin Mode
	pinMode(ClockPin, OUTPUT); // Set ClockPin Mode
	pinMode(DataPin, OUTPUT); // Set DataPin Mode
	for(int i=0;i<4;i++){ // Interate through Incoming DigitPins Array
	  this->_digitPins[i] = DigitPins[i]; // Place into Objects Private Properties
	  pinMode(this->_digitPins[i], OUTPUT); // Change DigitPins Mode to OUTPUT
	}
}

// Update LED Display, This method needs to be called in main MCU loop each time the loop executes.
void LEDDisplay::UpdateDisplay(){
	// Run a Loop to Update each Display Character
	for(byte j=0; j<4; j++){
		// Turn Digit Off
		digitalWrite(this->_digitPins[j], LOW);
	}
	// Bring Latch Pin (to Shift Reg) Low to Signal Incoming Data.
	digitalWrite(this->_latchPin, LOW);
	// Shift Out an All True Byte Pattern to Clear the Buffer.
	shiftOut(this->_dataPin, this->_clockPin, MSBFIRST, B11111111);
	// Bring Latch Pin(to Shift Reg) High to Signal Data Transmission Complete.
	digitalWrite(this->_latchPin, HIGH);

	// Delay for 100 Microseconds
	delayMicroseconds(100);
	// Turn Digit On
	digitalWrite(this->_digitPins[this->_digitScan], HIGH); 

	// Bring Latch Pin (to Shift Reg) Low to Signal Incoming Data
	digitalWrite(this->_latchPin, LOW);
	// Digit Scan ranges 0-3. Use this condition to place the Decimal Point
	if(this->_decimalLocation[this->_digitScan]==true){
		shiftOut(this->_dataPin, this->_clockPin, MSBFIRST, ~(this->_digit[this->_digitBuffer[this->_digitScan]] | B10000000)); //print the decimal point on the 3rd digit
	}else{
		shiftOut(this->_dataPin, this->_clockPin, MSBFIRST, ~this->_digit[this->_digitBuffer[this->_digitScan]]);
	}
	// Bring Latch Pin (to Shift Reg) High to Signal Data Transmission Complete.
	digitalWrite(this->_latchPin, HIGH);
	// Increment DigitScan for next loop.
	this->_digitScan++;
	// Check if we need to reset the DigitScan var to 0
	if(this->_digitScan>3){ this->_digitScan=0; }
}

// This method manipulates the array containing the assocation between Digits and their Decimal Places
bool LEDDisplay::SetDecimalPoint(int DecIndex, bool IsShown){
	if(DecIndex <= 3){
		this->_decimalLocation[DecIndex] = IsShown;
		return true;
	}else{
		return false;
	}
	return false;
}

bool LEDDisplay::DisplayThis(char Message[]){
	for(int CharIndex=0;CharIndex<4;CharIndex++){
		int DigitIndex = 0;
		switch(Message[CharIndex]){
			case '0':
				DigitIndex = 0;
				break;
			case '1':
				DigitIndex = 1;
				break;
			case '2':
				DigitIndex = 2;
				break;
			case '3':
				DigitIndex = 3;
				break;
			case '4':
				DigitIndex = 4;
				break;
			case '5':
				DigitIndex = 5;
				break;
			case '6':
				DigitIndex = 6;
				break;
			case '7':
				DigitIndex = 7;
				break;
			case '8':
				DigitIndex = 8;
				break;
			case '9':
				DigitIndex = 9;
				break;
			case 'A':
				DigitIndex = 10;
				break;
			case 'b':
				DigitIndex = 11;
				break;
			case 'C':
				DigitIndex = 12;
				break;
			case 'c':
				DigitIndex = 13;
				break;
			case 'd':
				DigitIndex = 14;
				break;
			case 'E':
				DigitIndex = 15;
				break;
			case 'F':
				DigitIndex = 16;
				break;
			case 'H':
				DigitIndex = 17;
				break;
			case 'J':
				DigitIndex = 18;
				break;
			case 'L':
				DigitIndex = 19;
				break;
			case 'n':
				DigitIndex = 20;
				break;
			case 'o':
				DigitIndex = 21;
				break;
			case 'P':
				DigitIndex = 22;
				break;
			case 'q':
				DigitIndex = 23;
				break;
			case 'r':
				DigitIndex = 24;
				break;
			case 'S':
				DigitIndex = 25;
				break;
			case 't':
				DigitIndex = 26;
				break;
			case 'u':
				DigitIndex = 27;
				break;
			case 'y':
				DigitIndex = 28;
				break;
			case 'k':
				DigitIndex = 29;
				break;
			default:
				DigitIndex = 0;
				break;
		}
		this->_digitBuffer[CharIndex] = DigitIndex;		//Serial.println(this->_digitBuffer[CharIndex]);
	}
	return true;
}