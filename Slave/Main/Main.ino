#include <LEDDisplay.h> // My Custom Library for Controlling 4-Digit 7-Seg. Displays via 8-bit Shift Reg 74595HC
#include <Servo.h> // Servo Control Library

#define POWERLED 2 // Define Digital Pin Used for Power Indicator LED

int Pos = 0;
// Instantiate New LEDDisplay Class into Object, Configure as follows: (ClockPin, LatchPin, DataPin, Digit1Pwr, Digit2Pwr, Digit3Pwr, Digit4Pwr)
int Pins[4] = {4,5,6,7};
// Instantiate Classes as Objects
LEDDisplay Display(11, 12, 13, Pins); // Create Display Object to control 4D 7S Display
Servo myservo; // Create Servo Object

void PowerLed(bool State){
  if(State){
    digitalWrite(POWERLED, HIGH);
  }else{
    digitalWrite(POWERLED, LOW);
  }
}

void setup() {
  //Serial.begin(9600);
  // put your setup code here, to run once:
  char Message[4] = {'C', 'o', 'o', 'L'};
  Display.DisplayThis(Message);
  // Set Power Indicator LED PinMode
  pinMode(POWERLED, OUTPUT);
  // Turn Power Indicator Light On
  PowerLed(true);
  // Init Servo
  myservo.attach(9);
}

void loop() {
  // put your main code here, to run repeatedly:
  Display.UpdateDisplay();
  myservo.write(0);
  
}
