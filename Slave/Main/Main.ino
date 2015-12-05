#include <LEDDisplay.h>

// Instantiate New LEDDisplay Class into Object, Configure as follows: (ClockPin, LatchPin, DataPin, Digit1Pwr, Digit2Pwr, Digit3Pwr, Digit4Pwr)
int Pins[4] = {4,5,6,7};
LEDDisplay Display(11, 12, 13, Pins);

void setup() {
  // put your setup code here, to run once:
  char Message[4] = {'1', '1', '1', '1'};
  Display.DisplayThis(Message);
}

void loop() {
  // put your main code here, to run repeatedly:
  Display.UpdateDisplay();
}
