/*
 * Temperature Sensor Displayed on 4 Digit 7 segment common anode
 * Created by Rui Santos, http://randomnerdtutorials.com
*/

const int digitPins[4] = {
  4,5,6,7};                 //4 common anode pins of the display
const int clockPin = 11;    //74HC595 Pin 11 
const int latchPin = 12;    //74HC595 Pin 12
const int dataPin = 13;     //74HC595 Pin 14
const byte digit[10] =      //seven segment digits in bits
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
  B01101111  //9
};
int digitBuffer[4] = {
  0};
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
void updateDisp(){
  for(byte j=0; j<4; j++)  
    digitalWrite(digitPins[j], LOW);
 
  digitalWrite(latchPin, LOW);  
  shiftOut(dataPin, clockPin, MSBFIRST, B11111111);
  digitalWrite(latchPin, HIGH);
 
  delayMicroseconds(100);
  digitalWrite(digitPins[digitScan], HIGH); 
 
  digitalWrite(latchPin, LOW);  
  if(digitScan==2)
    shiftOut(dataPin, clockPin, MSBFIRST, ~(digit[digitBuffer[digitScan]] | B10000000)); //print the decimal point on the 3rd digit
  else
    shiftOut(dataPin, clockPin, MSBFIRST, ~digit[digitBuffer[digitScan]]);
 
  digitalWrite(latchPin, HIGH);
  digitScan++;
  if(digitScan>3) digitScan=0; 
}
 
void loop(){ 
  digitBuffer[3] = 1;
  digitBuffer[2] = 1;
  digitBuffer[1] = 1;
  digitBuffer[0] = 1;
  updateDisp();
  delay(2);
}
