#include <EEPROM.h>
#include <LEDDisplay.h> // My Custom Library for Controlling 4-Digit 7-Seg. Displays via 8-bit Shift Reg 74595HC

// Define Indicator LEDs Pins
#define LED5 1  // PIN Number that is attached to LED in location 5
#define LED4 2  // PIN Number that is attached to LED in location 4
#define LED6 3  // PIN Number that is attached to LED in location 6
#define LED3 8  // PIN Number that is attached to LED in location 3
#define LED2 9  // PIN Number that is attached to LED in location 2
#define LED1 10  // PIN Number that is attached to LED in location 1

// Define Remote Read Pins
#define A A0
#define B A1
#define C A2
#define D A3

int Pins[4] = {4,5,6,7}; // PINS {Digit 1 PWR, Digit 2 PWR, Digit 3 PWR, Digit 4 PWR}
int RmtBtnState[4] = {0,0,0,0}; // Latches for Remote Buttons
long Timer = 21600; // Timer that counts down. 28800 seconds = 8hrs
//long Timer = 27365;
int LoopCounter = 0; // Used to delay startup so as to display a message before starting the countdown timer.
char Message[4] = {'b', 'o', 'o', 't'}; // Initial Message to Display
int LEDsOn[6] = {0,0,0,0,0,0}; // Array of Logic Bits Representing which of the 6 LEDs are on
unsigned long LastRun = 0; // Holds Millis() at end of loop
unsigned long Now = 0; // Holds Millis() at the start of each loop
unsigned long Difference; // The Difference Between the two aforementioned values. When this value >=1000 then 1 second has elapsed
const unsigned long period = 1000; // The Time in MilliSeconds equal to 1 Second
int CurrentPercentageRemaining; // Holds Current Percetage Being Displayed
long TempMessageDelay = 3200;
long TempMessageCounter = 0;
int ResetToggle = 0;

int ButtonAPressCount = 0;
int ButtonBPressCount = 0;
int ButtonCPressCount = 0;
int ButtonDPressCount = 0;

// Instantiate New LEDDisplay Class into Object, Configure as follows: (ClockPin, LatchPin, DataPin, Digit1Pwr, Digit2Pwr, Digit3Pwr, Digit4Pwr)
LEDDisplay Display(11, 12, 13, Pins); // Create Display Object to control 4 Digit 7 Segment Display

void setup() {
//  Serial.begin(9600);
  Display.DisplayThis(Message); // Set The Message Currently in the Message Variable into Class Object as next screen to display.
  // Set LED Pins Output Mode
  pinMode(LED5, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED1, OUTPUT);
  // Flash Booting Light Pattern
  BootLights();
  // Set All LEDs as OFF
  SetLights(0,0,0,0,0,0);
  Now = millis(); // Set Now to CurrentTime in Millis
  LastRun = Now; // Set LastRun to Now
  long RecoveredTimer = readIntFromEEPROM(0);
  if(RecoveredTimer>0){
    Timer = RecoveredTimer; 
  }
}

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void loop() {
  Display.UpdateDisplay(); // Update 7 Segment Display with Message Currently in it's buffer.
  // On Initial boot we wait for 3200 cycles so that the Boot Message can be displayed.
  if(LoopCounter<=3200){
    LoopCounter = ++LoopCounter;
  }else{ // After Looping Delay is complete, Start Running Time Decrementing Loop
    Now = millis(); // Set Now to Current Time
    Difference = Now - LastRun; // Get Difference between LastRun Time and Now
    if(Difference >= period){ // If Difference exceeds period interval (usually 1000ms)
      unsigned long TimerN = Timer - 1L; // Store Timer - 1L in a new Variable
      Timer = TimerN; // Update Main Timer with New Decremented Timer Value
      LastRun = Now; // Set LastRun to Now
    }
    CheckForRemoteInteraction();
    UpdatePercentageDisplayed(); // Check if 7 Segment Display Message Needs Changing
  }
}

// Boot-Up Lights Pattern
// Called during Setup to test LEDs
void BootLights(){
  int Delay = 200;
  int i = 0;
  for(i=0;i<10;i++){
    digitalWrite(LED1, HIGH);
    delay(Delay);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    delay(Delay);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    delay(Delay);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, HIGH);
    delay(Delay);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, HIGH);
    delay(Delay);
    digitalWrite(LED5, LOW);
    digitalWrite(LED6, HIGH);
    delay(Delay);
    digitalWrite(LED6, LOW);
    Delay = (Delay * 0.75);
  }
}

// Update Percentage Displayed
// Check how many seconds are left in Timer and change the message stored in the Display Class as the next screen to display.
void UpdatePercentageDisplayed(){
    if(Timer<=0){ // Battery Below 10%
      char NewMessage[4] = {'b', 'A', 't', 't'};
      SetLights(0,0,0,0,1,1);
      Display.DisplayThis(NewMessage);
    }else if(Timer>=20520){ // Battery Between 100%
      if(CurrentPercentageRemaining!=100){
        CurrentPercentageRemaining = 100;
        char NewMessage[4] = {'k', '1', '0', '0'};
        SetLights(1,1,1,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=19440){ // Battery Between 95%
      if(CurrentPercentageRemaining!=95){
        CurrentPercentageRemaining = 95;
        char NewMessage[4] = {'k', 'k', '9', '5'};
        SetLights(1,1,1,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=18360){ // Battery Between 90%
      if(CurrentPercentageRemaining!=90){
        CurrentPercentageRemaining = 90;
        char NewMessage[4] = {'k', 'k', '9', '0'};
        SetLights(1,1,1,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=17280){ // Battery Between 85%
      if(CurrentPercentageRemaining!=85){
        CurrentPercentageRemaining = 85;
        char NewMessage[4] = {'k', 'k', '8', '5'};
        SetLights(1,1,1,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=16200){ // Battery Between 80%
      if(CurrentPercentageRemaining!=80){
        CurrentPercentageRemaining = 80;
        char NewMessage[4] = {'k', 'k', '8', '0'};
        SetLights(1,1,1,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=15120){ // Battery Between 75%
      if(CurrentPercentageRemaining!=75){
        CurrentPercentageRemaining = 75;
        char NewMessage[4] = {'k', 'k', '7', '5'};
        SetLights(0,1,1,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=14040){ // Battery Between 70%
      if(CurrentPercentageRemaining!=70){
        CurrentPercentageRemaining = 70;
        char NewMessage[4] = {'k', 'k', '7', '0'};
        SetLights(0,1,1,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=12960){ // Battery Between 65%
      if(CurrentPercentageRemaining!=65){
        CurrentPercentageRemaining = 65;
        char NewMessage[4] = {'k', 'k', '6', '5'};
        SetLights(0,1,1,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=11880){ // Battery Between 60%
      if(CurrentPercentageRemaining!=60){
        CurrentPercentageRemaining = 60;
        char NewMessage[4] = {'k', 'k', '6', '0'};
        SetLights(0,1,1,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=10800){ // Battery Between 55%
      if(CurrentPercentageRemaining!=55){
        CurrentPercentageRemaining = 55;
        char NewMessage[4] = {'k', 'k', '5', '5'};
        SetLights(0,1,1,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=9720){ // Battery Between 50%
      if(CurrentPercentageRemaining!=50){
        CurrentPercentageRemaining = 50;
        char NewMessage[4] = {'k', 'k', '5', '0'};
        SetLights(0,0,1,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=8640){ // Battery Between 45%
      if(CurrentPercentageRemaining!=45){
        CurrentPercentageRemaining = 45;
        char NewMessage[4] = {'k', 'k', '4', '5'};
        SetLights(0,0,1,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=7560){ // Battery Between 40%
      if(CurrentPercentageRemaining!=40){
        CurrentPercentageRemaining = 40;
        char NewMessage[4] = {'k', 'k', '4', '0'};
        SetLights(0,0,1,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=6480){ // Battery Between 35%
      if(CurrentPercentageRemaining!=35){
        CurrentPercentageRemaining = 35;
        char NewMessage[4] = {'k', 'k', '3', '5'};
        SetLights(0,0,1,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=5400){ // Battery Between 30%
      if(CurrentPercentageRemaining!=30){
        CurrentPercentageRemaining = 30;
        char NewMessage[4] = {'k', 'k', '3', '0'};
        SetLights(0,0,1,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=4320){ // Battery Between 25%
      if(CurrentPercentageRemaining!=25){
        CurrentPercentageRemaining = 25;
        char NewMessage[4] = {'k', 'k', '2', '5'};
        SetLights(0,0,0,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=3240){ // Battery Between 20%
      if(CurrentPercentageRemaining!=20){
        CurrentPercentageRemaining = 20;
        char NewMessage[4] = {'k', 'k', '2', '0'};
        SetLights(0,0,0,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=3024){ // Battery Between 19%
      if(CurrentPercentageRemaining!=19){
        CurrentPercentageRemaining = 19;
        char NewMessage[4] = {'k', 'k', '1', '9'};
        SetLights(0,0,0,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=2808){ // Battery Between 18%
      if(CurrentPercentageRemaining!=18){
        CurrentPercentageRemaining = 18;
        char NewMessage[4] = {'k', 'k', '1', '8'};
        SetLights(0,0,0,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=2592){ // Battery Between 17%
      if(CurrentPercentageRemaining!=17){
        CurrentPercentageRemaining = 17;
        char NewMessage[4] = {'k', 'k', '1', '7'};
        SetLights(0,0,0,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=2376){ // Battery Between 16%
      if(CurrentPercentageRemaining!=16){
        CurrentPercentageRemaining = 16;
        char NewMessage[4] = {'k', 'k', '1', '6'};
        SetLights(0,0,0,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=2160){ // Battery Between 15%
      if(CurrentPercentageRemaining!=15){
        CurrentPercentageRemaining = 15;
        char NewMessage[4] = {'k', 'k', '1', '5'};
        SetLights(0,0,0,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=1944){ // Battery Between 14%
      if(CurrentPercentageRemaining!=14){
        CurrentPercentageRemaining = 14;
        char NewMessage[4] = {'k', 'k', '1', '4'};
        SetLights(0,0,0,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=1728){ // Battery Between 13%
      if(CurrentPercentageRemaining!=13){
        CurrentPercentageRemaining = 13;
        char NewMessage[4] = {'k', 'k', '1', '3'};
        SetLights(0,0,0,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=1512){ // Battery Between 12%
      if(CurrentPercentageRemaining!=12){
        CurrentPercentageRemaining = 12;
        char NewMessage[4] = {'k', 'k', '1', '2'};
        SetLights(0,0,0,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=1296){ // Battery Between 11%
      if(CurrentPercentageRemaining!=11){
        CurrentPercentageRemaining = 11;
        char NewMessage[4] = {'k', 'k', '1', '1'};
        SetLights(0,0,0,1,0,0);
        Display.DisplayThis(NewMessage);
      }
    }else if(Timer>=1080){ // Battery Between 10%
      if(CurrentPercentageRemaining!=10){
        CurrentPercentageRemaining = 10;
        char NewMessage[4] = {'k', 'k', '1', '0'};
        SetLights(0,0,0,0,1,1);
        Display.DisplayThis(NewMessage);
      }
    }
}

// Set State of LED Indicator Lights
void SetLights(int a, int b, int c, int d, int e, int f){
  LEDsOn[0] = a;
  LEDsOn[1] = b;
  LEDsOn[2] = c;
  LEDsOn[3] = d;
  LEDsOn[4] = e;
  LEDsOn[5] = f;
  if(LEDsOn[0]==1){
    digitalWrite(LED1, HIGH);
  }else if(LEDsOn[0]==0){
    digitalWrite(LED1, LOW);
  }

  if(LEDsOn[1]==1){
    digitalWrite(LED2, HIGH);
  }else if(LEDsOn[1]==0){
    digitalWrite(LED2, LOW);
  }

  if(LEDsOn[2]==1){
    digitalWrite(LED3, HIGH);
  }else if(LEDsOn[2]==0){
    digitalWrite(LED3, LOW);
  }

  if(LEDsOn[3]==1){
    digitalWrite(LED4, HIGH);
  }else if(LEDsOn[3]==0){
    digitalWrite(LED4, LOW);
  }

  if(LEDsOn[4]==1){
    digitalWrite(LED5, HIGH);
  }else if(LEDsOn[4]==0){
    digitalWrite(LED5, LOW);
  }

  if(LEDsOn[5]==1){
    digitalWrite(LED6, HIGH);
  }else if(LEDsOn[5]==0){
    digitalWrite(LED6, LOW);
  }
}

void CheckForRemoteInteraction(){
//  delayMicroseconds(200);
//  int ButtonA = analogRead(14);  
//  delayMicroseconds(200);
//  int ButtonB = analogRead(15);
//  delayMicroseconds(200);
//  int ButtonC = analogRead(16);
//  delayMicroseconds(200);
//  int ButtonD = analogRead(17);
//  delayMicroseconds(200);

  int ButtonA = analogRead(14);
  delayMicroseconds(200);
  if(ButtonA>=1020 && RmtBtnState[0]==0){ // Set EEPROM Battery Value
    if(ButtonAPressCount>200){
      RmtBtnState[0] = 1;
      long SavedTimer = Timer;
      writeIntIntoEEPROM(0, SavedTimer);
      char NewMessage[4] = {'k', 'S', 'E', 't'};
      Display.DisplayThis(NewMessage);
      ButtonAPressCount = 0;
    }else{
      ButtonAPressCount = ++ButtonAPressCount;
    }
  }else if(ButtonA<1020){
    RmtBtnState[0] = 0;
    ButtonAPressCount = 0;
  }

  int ButtonB = analogRead(15);
  delayMicroseconds(200);
  if(ButtonB>=1020 && RmtBtnState[1]==0){ // Clear EEPROM Battery Value
    if(ButtonBPressCount>200){
      RmtBtnState[1] = 1;
      unsigned long BlankTimer = 00000;
      writeIntIntoEEPROM(0, BlankTimer);
      char NewMessage[4] = {'k', 'C', 'L', 'r'};
      Display.DisplayThis(NewMessage);
      ButtonBPressCount = 0;
    }else{
      ButtonBPressCount = ++ButtonBPressCount;
    }
  }else if(ButtonB<1020){
    RmtBtnState[1] = 0;
    ButtonBPressCount = 0;
  }

  int ButtonC = analogRead(16);
  delayMicroseconds(200);
  if(ButtonC>=1020 && RmtBtnState[2]==0){ // Reset Arduino
    if(ButtonCPressCount>200){
      RmtBtnState[2] = 1;
      char NewMessage[4] = {'k', 'r', 'S', 't'};
      Display.DisplayThis(NewMessage);
      ResetToggle = 1;
      ButtonCPressCount = 0;
    }else{
      ButtonCPressCount = ++ButtonCPressCount;
    }
  }else if(ButtonC<1020){
    RmtBtnState[2] = 0;
    ButtonCPressCount = 0;
  }

  int ButtonD = analogRead(17);
  delayMicroseconds(200);
  if(ButtonD>=1020 && RmtBtnState[3]==0){ // Read EEPROM to Serial Out
    if(ButtonDPressCount>200){
      RmtBtnState[3] = 1;
      long RecoveredTimer = readIntFromEEPROM(0);
      if(RecoveredTimer>0){
        char NewMessage[4] = {'k', 'y', 'E', 'S'};
        Display.DisplayThis(NewMessage);
      }else{
        char NewMessage[4] = {'k', 'k', 'n', 'o'};
        Display.DisplayThis(NewMessage);
      }
      ButtonDPressCount = 0;
    }else{
      ButtonDPressCount = ++ButtonDPressCount;
    }
  }else if(ButtonD<1020){
    RmtBtnState[3] = 0;
    ButtonDPressCount = 0;
  }

  if(TempMessageCounter<TempMessageDelay){
    TempMessageCounter = ++TempMessageCounter;
  }else{
    TempMessageCounter = 0;
    CurrentPercentageRemaining = 0;
    if(ResetToggle){
//      Serial.println("Reset Toggle was True, Resetting...");
      char NewMessage[4] = {'k', 'k', 'k', 'k'};
      Display.DisplayThis(NewMessage);
      Display.UpdateDisplay();
      delay(1000);
      resetFunc();  //call reset
    }
  }
}

void writeIntIntoEEPROM(int address, int number){ 
  byte byte1 = number >> 8;
  byte byte2 = number & 0xFF;
  EEPROM.put(address, byte1);
  EEPROM.put(address + 1, byte2);
}

int readIntFromEEPROM(int address)
{
  byte byte1 = EEPROM.read(address);
  byte byte2 = EEPROM.read(address + 1);
  return (byte1 << 8) + byte2;
}
