#include <Servo.h>

// Create servo objects to control two servos
Servo servo1;
Servo servo2;

// Define the pins for the servos
//int servo1Pin = 6; // Pin connected to servo 1
//int servo2Pin = 7; // Pin connected to servo 2

// Define the pins for the servos
int servo1Pin = 9; // Pin connected to servo 1
int servo2Pin = 10; // Pin connected to servo 2

void setup() {
  // Attach the servo objects to the pins
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  // Initialize servos to the middle position
  servo1.write(90); 
  servo2.write(180);
}

void loop() {

  // Move servo1 from 0 to 180 degrees
  for (int pos = 0; pos <= 180; pos++) {
    servo1.write(pos);
    delay(15); // Wait for 15 milliseconds to allow the servo to reach the position
  }
 
  // Move servo1 from 180 to 0 degrees
  for (int pos = 180; pos >= 0; pos--) {
    servo1.write(pos);
    delay(15);
  }



}
