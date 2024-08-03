// Include the Arduino Stepper library
#include <Stepper.h>

// Define the number of steps per revolution
const int stepsPerRevolution = 2048; // Change this according to your motor

// Initialize the stepper motor with steps and pins
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  // Set the speed of the motor (in RPM)
  myStepper.setSpeed(10); // Adjust the speed as needed
}

void loop() {
  // Rotate the motor 360 degrees clockwise
  myStepper.step(stepsPerRevolution);
  delay(1000); // Adjust the delay between steps as needed
  
  // Rotate the motor 360 degrees counterclockwise
  myStepper.step(-stepsPerRevolution);
  delay(1000); // Adjust the delay between steps as needed
}
