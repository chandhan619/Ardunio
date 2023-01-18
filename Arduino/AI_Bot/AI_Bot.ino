#include <Servo.h>


int motor1pin1 = 2;
int motor1pin2 = 3;

int motor2pin1 = 4;
int motor2pin2 = 5;

int incomingByte = 0;
Servo myservo1;  // create servo object to control a servo
Servo myservo2;  // create servo object to control a servo


void setup() {
  Serial.begin(9600);
  
  // put your setup code here, to run once:
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);

  myservo1.attach(6);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  
     if (Serial.available() > 0) {  
    
        incomingByte = Serial.read();
    switch (incomingByte) {
        case '0':
          digitalWrite(motor1pin1, LOW);
          digitalWrite(motor1pin2, LOW);
          digitalWrite(motor2pin1, LOW);
          digitalWrite(motor2pin2, LOW);
          break;
      
        case '1':
          digitalWrite(motor1pin1, LOW);
          digitalWrite(motor1pin2, HIGH);
          digitalWrite(motor2pin1, HIGH);
          digitalWrite(motor2pin2, LOW);
          break;
        case '2':
          digitalWrite(motor1pin1, HIGH);
          digitalWrite(motor1pin2, LOW);
          digitalWrite(motor2pin1, HIGH);
          digitalWrite(motor2pin2, LOW);
          break;

        case '3':
          digitalWrite(motor1pin1, LOW);
          digitalWrite(motor1pin2, HIGH);
          digitalWrite(motor2pin1, HIGH);
          digitalWrite(motor2pin2, LOW);
          break;
        case '4':
          digitalWrite(motor1pin1, HIGH);
          digitalWrite(motor1pin2, LOW);
          digitalWrite(motor2pin1, LOW);
          digitalWrite(motor2pin2, HIGH);
          break;

        default:
          digitalWrite(motor1pin1, LOW);
          digitalWrite(motor1pin2, LOW);
          digitalWrite(motor2pin1, LOW);
          digitalWrite(motor2pin2, LOW);
          break;
      }

  }
  else{

          digitalWrite(motor1pin1, LOW);
          digitalWrite(motor1pin2, LOW);
          digitalWrite(motor2pin1, LOW);
          digitalWrite(motor2pin2, LOW);
  }

         
}
