#include "Display.h"
void setup(){
  Serial.begin(115200);
  pinMode(A0, INPUT);
}
void loop(){
  Serial.println(analogRead(A0)/4096.0*10.0*1000.0);
}
