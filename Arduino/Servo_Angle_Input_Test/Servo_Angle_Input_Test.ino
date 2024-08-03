// Create servo objects to control two servos
#include <Servo.h>

Servo servo1;
Servo servo2;

// Define the pins for the servos
int servo1Pin = 6; // Pin connected to servo 1
int servo2Pin = 7; // Pin connected to servo 2

int sero1Sensitivity = 200;
int sero2Sensitivity = 200;


int servo1TargetPos = 90;
int servo2TargetPos = 90;

int servo1CurrentPos = 90;
int servo2CurrentPos = 90;

String servoData[2];

String incomingData = "";

void setup() {
  

Serial.begin(9600);
  
  

}

void loop() {
  if (Serial.available() > 0) {
    //Read Data if available
    incomingData = Serial.readStringUntil('\n');
    Serial.println(incomingData);

    if(incomingData.startsWith("Servo1")){
          Serial.println(incomingData);
          splitString(incomingData, ' ', servoData, 5);
          Serial.println("After Split");
          Serial.println(servoData[1]);
          servo1TargetPos = (servoData[1]).toInt();
           if( servo1TargetPos >= 0 && servo1TargetPos <= 90){
              if( servo1CurrentPos != servo1TargetPos){
                moveServo(servo1,servo1CurrentPos,servo1TargetPos,sero1Sensitivity);
              }
           }
       }
  }
}

void moveServo(Servo servoName,int currentPos,int TargetPos,int sensitivity){
    while(currentPos!=TargetPos){
      if(currentPos<TargetPos){
        currentPos++;
      }
      else{
        currentPos--;
      }
    }
    //servoName.write(currentPos);
    Serial.println(currentPos);
    delay(sensitivity);
}

int splitString(String data, char delimiter, String *outputArray, int maxParts) {
  int currentIndex = 0;
  int startIndex = 0;
  int endIndex = 0;

  while (currentIndex < maxParts - 1 && endIndex >= 0) {
    endIndex = data.indexOf(delimiter, startIndex);
    if (endIndex == -1) {
      outputArray[currentIndex] = data.substring(startIndex);
    } else {
      outputArray[currentIndex] = data.substring(startIndex, endIndex);
      startIndex = endIndex + 1;
    }
    currentIndex++;
  }
  
  return currentIndex;
}
