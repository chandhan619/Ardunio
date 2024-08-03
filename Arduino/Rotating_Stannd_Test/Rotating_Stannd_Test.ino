#include <Stepper.h>

const int stepsPerRevolution = 2048;  // Number of steps per revolution for 28BYJ-48

// Initialize the stepper library on pins 8 through 11
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  // Set the speed to 10 rpm
  myStepper.setSpeed(15);
  // Initialize the serial port
  Serial.begin(9600);
}

void loop() {
  // Step one revolution in one direction
  Serial.println("Clockwise");
  myStepper.step(stepsPerRevolution);
  delay(1000);

  // Step one revolution in the other direction
  Serial.println("Counterclockwise");
  myStepper.step(-stepsPerRevolution);
  delay(1000);
}
