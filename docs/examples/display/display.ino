#include "Display.h"
void setup(){
  Serial.begin(115200);
}
void loop(){
    digitalWrite(O0,HIGH);
    delay(500);
    digitalWrite(O0,LOW);
    delay(500);
}
