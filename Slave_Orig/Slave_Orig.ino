/*
 * Temperature Sensor Displayed on 4 Digit 7 segment common anode
 * Created by Rui Santos, http://randomnerdtutorials.com
*/

const int digitPins[4] = {4,5,6,7};                 //4 common anode pins of the display
//const int clockPin = 11;    //74HC595 Pin 11
#define clockPin 11
//const int latchPin = 12;    //74HC595 Pin 12
#define latchPin 12
//const int dataPin = 13;     //74HC595 Pin 14
#define dataPin 13
const byte digit[38] =      //seven segment digits in bits
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
  B01000110, //t - 26
  B00011100, //u - 27
  B01100110  //y - 28
};
int digitBuffer[4] = {0};
int digitScan = 0;
 
void setup(){                
  for(int i=0;i<4;i++)
  {
    pinMode(digitPins[i],OUTPUT);
  }
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
}
 
//writes the temperature on display
// This function must be called cont. from loop() in order to keep display, well displaying... :-)
void updateDisp(){
  // Run a Loop to Update each Display Character
  for(byte j=0; j<4; j++){
    // Turn Digit Off
    digitalWrite(digitPins[j], LOW);
  }
  // Bring Latch Pin (to Shift Reg) Low to Signal Incoming Data.
  digitalWrite(latchPin, LOW);  
  // Shift Out an All True Byte Pattern to Clear the Buffer.
  shiftOut(dataPin, clockPin, MSBFIRST, B11111111);
  // Bring Latch Pin(to Shift Reg) High to Signal Data Transmission Complete.
  digitalWrite(latchPin, HIGH);
  
  // Delay for 100 Microseconds
  delayMicroseconds(100);
  // Turn Digit On
  digitalWrite(digitPins[digitScan], HIGH); 
  
  // Bring Latch Pin (to Shift Reg) Low to Signal Incoming Data
  digitalWrite(latchPin, LOW);
  // Digit Scan ranges 0-3. Use this condition to place the Decimal Point
  if(digitScan==2){
    shiftOut(dataPin, clockPin, MSBFIRST, ~(digit[digitBuffer[digitScan]] | B10000000)); //print the decimal point on the 3rd digit
  }else{
    shiftOut(dataPin, clockPin, MSBFIRST, ~digit[digitBuffer[digitScan]]);
  }
  // Bring Latch Pin (to Shift Reg) High to Signal Data Transmission Complete.
  digitalWrite(latchPin, HIGH);
  // Increment DigitScan for next loop.
  digitScan++;
  // Check if we need to reset the DigitScan var to 0
  if(digitScan>3){ digitScan=0; }
}
 
void loop(){
  for(int z=0;z<=28;z++){
    
    digitBuffer[3] = digit[z];
    digitBuffer[2] = digit[z];
    digitBuffer[1] = digit[z];
    digitBuffer[0] = digit[z];
    updateDisp();
    delay(200);
  }
}
