#include "SevSeg.h"
SevSeg sevseg; 

int lstst_button1 = 1;
int lstst_button2 = 1;

int temperature = 5;

void setup(){
  Serial.begin(9600);
  pinMode(2,INPUT_PULLUP);
  pinMode(15,INPUT_PULLUP);
  byte numDigits = 2;
  byte digitPins[] = {18,19};
  byte segmentPins[] = {26,25,27,14,12,32,33};
  bool resistorsOnSegments = true;

  byte hardwareConfig = COMMON_ANODE; 
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);
}

void loop(){
    /*for(int i = 0; i < 10; i++){
        sevseg.setNumber(i, i%2);
        delay(1000);
        sevseg.refreshDisplay(); 
        }*/
  if (lstst_button1 != digitalRead(2)){
    delay(5);
    if (digitalRead(2)==1){
      temperature--;}
    lstst_button1 = digitalRead(2);}
  if (lstst_button2 != digitalRead(15)){
    delay(5);
    if (digitalRead(15)==1){
      temperature++;}
    lstst_button2 = digitalRead(15);}
  sevseg.setNumber(temperature);
  sevseg.refreshDisplay(); 
}