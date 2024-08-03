#include <Stepper.h>


const int stepsPerRevolution = 2048;
const int switchPin = 2;
int switchState = 0;


Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

const int potPin = A0;

void setup() {
  Serial.begin(9600);
 
  myStepper.setSpeed(15);
  pinMode(switchPin, INPUT_PULLUP);
  
  
}

void loop() {
  
  int potValue = analogRead(potPin);
  int motorSpeed = map(potValue, 0, 670, 7, 16);
  myStepper.setSpeed(motorSpeed);
  
  Serial.print("Motor Speed: ");
  Serial.println(motorSpeed);

  
  switchState = digitalRead(switchPin);
  Serial.println(switchState);
     if (switchState == LOW) {
        myStepper.step(100);
      } else {
        myStepper.step(-100);
      }

 // delay(10);
}
