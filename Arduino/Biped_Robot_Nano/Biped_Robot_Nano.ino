/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo1;  // create servo object to control a servo
Servo myservo2;  // create servo object to control a servo
Servo myservo3;  // create servo object to control a servo
Servo myservo4;  // create servo object to control a servo
Servo myservo5;  // create servo object to control a servo
Servo myservo6;  // create servo object to control a servo
Servo myservo7;  // create servo object to control a servo
Servo myservo8;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int currentAngles[] = {90,90,80,90,90,90,90,90};
int newAngles[]     = {90,90,90,90,90,90,90,90};
String incomingByte  ="";

void setup() {
  Serial.begin(9600);
  myservo1.attach(2);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(3);  // attaches the servo on pin 9 to the servo object
  myservo3.attach(4);  // attaches the servo on pin 9 to the servo object
  myservo4.attach(5);  // attaches the servo on pin 9 to the servo object
  myservo5.attach(6);  // attaches the servo on pin 9 to the servo object
  myservo6.attach(7);  // attaches the servo on pin 9 to the servo object
  myservo7.attach(8);  // attaches the servo on pin 9 to the servo object
  myservo8.attach(9);  // attaches the servo on pin 9 to the servo object

  myservo1.write(100);  // attaches the servo on pin 9 to the servo object
  myservo2.write(90);  // attaches the servo on pin 9 to the servo object
  myservo3.write(100);  // attaches the servo on pin 9 to the servo object
  myservo4.write(90);  // attaches the servo on pin 9 to the servo object
  myservo5.write(90);  // attaches the servo on pin 9 to the servo object
  myservo6.write(90);  // attaches the servo on pin 9 to the servo object
  myservo7.write(80);  // attaches the servo on pin 9 to the servo object
  myservo8.write(90);  // attaches the servo on pin 9 to the servo object

}

void loop() {
      
  /*  if (Serial.available() > 0) {
      incomingByte = Serial.readString();
      if(incomingByte.length()==5)
      {
      currentAngles[incomingByte.substring(0,1).toInt()]=incomingByte.substring(1,4).toInt();
      }
      Serial.println(incomingByte.length());
    }
    updatePosition();
    delay(1000); */
}

void updatePosition(){
  for(int i = 1 ; i<=8;i++)
  {
          if(currentAngles[i]>newAngles[i])
          {
            newAngles[i] = newAngles[i] + 1;
          }
          else if(currentAngles[i]<newAngles[i])
          {
            newAngles[i] = newAngles[i] - 1;
          }
          writeAngle(i,newAngles[i]);
  }
}

void writeAngle(int servoNum,int angle)
{

   switch (servoNum) {
          case 1:
            myservo1.write(angle);
            Serial.println("Updating sero 1");
            break;
          case 2:
            myservo2.write(angle);
            Serial.println("Updating sero 2");
            break;
          case 3:
            myservo3.write(angle);
            Serial.println("Updating sero 3");
            break;
          case 4:
            myservo4.write(angle);
            Serial.println("Updating sero 4");
            break;
          case 5:
            myservo5.write(angle);
            Serial.println("Updating sero 5");
            break;
          case 6:
            myservo6.write(angle);
            Serial.println("Updating sero 6");
            break;
           case 7:
            myservo7.write(angle);
            Serial.println("Updating sero 7");
            break;
           case 8:
            myservo8.write(angle);
            Serial.println("Updating sero 8");
            break;
    
   }
   Serial.println(angle);
}
