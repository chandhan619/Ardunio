
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  110
#define SERVOMAX  550 
#define SERVO_FREQ 50 


int currentPositions[] = {352,330,352,340,320,330,345,347,330,330,330,330,330,330,330,330};
int movementSpeed = 5;

void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  delay(5);
}

// You can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. It's not precise!


void loop() {
 int angles1[] = {352,330,352,340,320,330,345,347,330,330,330,330,330,330,330,330};
 int angles2[] = {355,469,202,341,328,330,345,404,330,330,330,330,330,330,330,330};
 Serial.println("Updating Angles");
  updatePositions(movementSpeed,angles1);
  Serial.println("Positions1");
  updatePositions(movementSpeed,angles2);
  Serial.println("Positions2");

}


void updatePositions(int movementSpeed, int newAngles[])
{
  
  while(compareAngles(currentPositions,newAngles))
  {
   
    for(int i=0;i<8;i++)
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

boolean compareAngles(int angles1[],int angles2[]){
  boolean flag = false;
  for(int i = 0;i<sizeof(angles1);i++)
  {
    if(angles1[i]!=angles2[i])
    {
      flag = true;
    }
  }

  return flag;
}

void updateAngles(int angles[])
{
  for(int i = 0 ; i<8 ; i++)
  {
     pwm.setPWM(i, 0, angles[i]);
  }
}

void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= SERVO_FREQ;   // Analog servos run at ~60 Hz updates
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000000;  // convert input seconds to us
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}
