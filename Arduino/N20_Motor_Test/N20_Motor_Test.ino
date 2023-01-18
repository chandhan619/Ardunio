
const byte interruptPin1 = 2;
const byte interruptPin2 = 3;


void setup() {

  attachInterrupt(digitalPinToInterrupt(interruptPin1), motor1Interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), motor2Interrupt, CHANGE);
}

void loop() {

}

void motor1Interrupt() {
  
}

void motor2Interrupt() {
  
}
