void setup(){

}
void loop(){
  if (digitalRead(I0)){
    digitalWrite(O0, HIGH);
  }else{
    digitalWrite(O0, LOW);
  }
}