#include <Servo.h>

// Create servo objects to control two servos
Servo servo1;
Servo servo2;

// Define the pins for the servos
int servo1Pin = 9; // Pin connected to servo 1
int servo2Pin = 10; // Pin connected to servo 2

int sero1Sensitivity = 0;
int sero2Sensitivity = 0;

int servo1CurrentPos = 90;
int servo2CurrentPos = 90;

boolean Servo1_Left = false;
boolean Servo1_Right = false;
boolean Servo1_Stop = false;
boolean Servo2_Up = false;
boolean Servo2_Down = false;
boolean Servo2_Stop = false;

String servoData[2];


// Define motor pins
int motor1Pin1 = 2; // IN1
int motor1Pin2 = 3; // IN2
int motor2Pin1 = 4; // IN3
int motor2Pin2 = 5; // IN4

String incomingData = "";

unsigned long previousMillis = 0; // Stores the last time the LED was updated
const long interval = 15;

void setup() {

Serial.begin(9600);
  
  // Set motor pins as output
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
Serial.println("Hello from Arduino!");
Serial.println("Waiting for data to be recived ..");
  
  // Attach the servo objects to the pins
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  // Initialize servos to the middle position
  servo1.write(servo1CurrentPos); 
  servo2.write(servo1CurrentPos);
  
}

void loop() {

  if (Serial.available() > 0) {
    //Read Data if available
    incomingData = Serial.readStringUntil('\n');
    Serial.println(incomingData);
    if(incomingData.equalsIgnoreCase("Forword")){
            //  Move motor1 forward
            digitalWrite(motor1Pin1, LOW);
            digitalWrite(motor1Pin2, HIGH);
            // Move motor2 forward
           digitalWrite(motor2Pin1, HIGH);
           digitalWrite(motor2Pin2, LOW);
    }
    if(incomingData.equalsIgnoreCase("Backword")){
            // Move motor1 backward
            digitalWrite(motor1Pin1, HIGH);
            digitalWrite(motor1Pin2, LOW);
            // Move motor2 backward
            digitalWrite(motor2Pin1, LOW);
            digitalWrite(motor2Pin2, HIGH);
       }
   if(incomingData.equalsIgnoreCase("Left")){
            // Move motor1 backward
            digitalWrite(motor1Pin1, HIGH);
            digitalWrite(motor1Pin2, LOW);
            // Move motor2 backward
            digitalWrite(motor2Pin1, HIGH);
            digitalWrite(motor2Pin2, LOW);
       }
     if(incomingData.equalsIgnoreCase("Right")){
            // Move motor1 backward
            digitalWrite(motor1Pin1, LOW);
            digitalWrite(motor1Pin2, HIGH);
            // Move motor2 backward
            digitalWrite(motor2Pin1, LOW);
            digitalWrite(motor2Pin2, HIGH);
       }
    if(incomingData.equalsIgnoreCase("Stop")){
            // Move motor1 backward
            digitalWrite(motor1Pin1, LOW);
            digitalWrite(motor1Pin2, LOW);
            // Move motor2 backward
            digitalWrite(motor2Pin1, LOW);
            digitalWrite(motor2Pin2, LOW);
       }
    if(incomingData.startsWith("Reset")){
            servo1.write(90); 
            servo2.write(90);
            servo1CurrentPos = 90;
            servo2CurrentPos = 90;
             Servo1_Left = false;
             Servo1_Right = false;
             Servo1_Stop = false;
             Servo2_Up = false;
             Servo2_Down = false;
             Servo2_Stop = false;
       }
    if(incomingData.startsWith("Servo1_Left")){
      Serial.println("Servo1_Left");
        Servo1_Left = true;
        Servo1_Right = false;
      }
    if(incomingData.startsWith("Servo1_Right")){
      Serial.println("Servo1_Right");
        Servo1_Left = false;
        Servo1_Right = true;
      }
    if(incomingData.startsWith("Servo1_Stop")){
      Serial.println("Servo1_Stop");
        Servo1_Left = false;
        Servo1_Right = false;
      }
      
    if(incomingData.startsWith("Servo2_Up")){
      Serial.println("Servo2_Up");
        Servo2_Up = true;
        Servo2_Down = false;
      }
    if(incomingData.startsWith("Servo2_Down")){
      Serial.println("Servo2_Down");
        Servo2_Up = false;
        Servo2_Down = true;
      }
    if(incomingData.startsWith("Servo2_Stop")){
      Serial.println("Servo2_Stop");
        Servo2_Up = false;
        Servo2_Down = false;
      }
  }

unsigned long currentMillis = millis();


  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
          if(Servo1_Left==true){
             Serial.println("Updating Servo Left");
            servo1CurrentPos++;
            servo1.write(servo1CurrentPos); 
          }
        
          if(Servo1_Right==true){
            servo1CurrentPos--;
            servo1.write(servo1CurrentPos); 
          }
        
          
          if(Servo2_Up==true){
            servo2CurrentPos++;
            servo2.write(servo2CurrentPos); 
          }
        
          if(Servo2_Down==true){
            servo2CurrentPos--;
            servo2.write(servo2CurrentPos); 
          }
   
  }


}

 
