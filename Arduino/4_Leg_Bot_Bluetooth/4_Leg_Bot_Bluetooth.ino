/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;

Servo myservo5;
Servo myservo6;
Servo myservo7;
Servo myservo8; 

Servo myservo9;
Servo myservo10;
Servo myservo11;
Servo myservo12; 

  
int currentPositions[] = {90,90,90,90,90,90,90,90,90,90,90,90};
int motorCount = 12;
int movementSpeed = 30;

void setup() {
  myservo1.attach(0);
  myservo2.attach(1);
  myservo3.attach(2);
  myservo4.attach(3);
  
  myservo5.attach(4);
  myservo6.attach(5);
  myservo7.attach(6);
  myservo8.attach(7);

  myservo9.attach(8);
  myservo10.attach(9);
  myservo11.attach(10);
  myservo12.attach(11);

  updatePositions(movementSpeed,currentPositions);

   Serial.begin(9600);
}

void loop() {
  
    int pos1[] = {90,90,90,90,90+45,90-45,90-45,90+45,90-90,90+90,90+90,90-90};
    int pos2[] = {90,90,90,90,90-45,90+45,90+45,90-45,90+90,90-90,90-90,90+90};
    
                     
     
     updatePositions(movementSpeed,pos1);
     updatePositions(movementSpeed,pos2);
                   
}


void updatePositions(int movementSpeed, int newAngles[])
{

  


  delay(500);
  while(compareAngles(currentPositions,newAngles))
  {


   
    for(int i=0;i<motorCount;i++)
    {
      if(currentPositions[i]>newAngles[i])
      {
        currentPositions[i]--;
      }
      if(currentPositions[i]<newAngles[i])
      {
        currentPositions[i]++;
      }
    }  
    updateAngles(currentPositions);
    delay(movementSpeed);
}
  
}


void updateAngles(int angles[])
{
    myservo1.write(angles[0]);
    myservo2.write(angles[1]);
    myservo3.write(angles[2]);
    myservo4.write(angles[3]);
    
    myservo5.write(angles[4]);
    myservo6.write(angles[5]);
    myservo7.write(angles[6]);
    myservo8.write(angles[7]);

    myservo9.write(angles[8]);
    myservo10.write(angles[9]);
    myservo11.write(angles[10]);
    myservo12.write(angles[11]);
}

boolean compareAngles(int angles1[],int angles2[]){

  boolean flag = false;
  for(int i = 0;i<motorCount;i++)
  {

    

    if(angles1[i]!=angles2[i])
    {
      flag = true;

    }
  }
  return flag;
}
