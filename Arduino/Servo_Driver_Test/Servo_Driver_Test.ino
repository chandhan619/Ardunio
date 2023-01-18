/*************************************************** 
  This is an example for our Adafruit 16-channel PWM & Servo driver
  Servo test - this will drive 8 servos, one after the other on the
  first 8 pins of the PCA9685

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815
  
  These drivers use I2C to communicate, 2 pins are required to  
  interface.

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// our servo # counter
  int legs[8] = { 90-50,90-30,
                  90+50,90+30,
                  90-50,90-30,
                  90+50,90+30,
                };
  int servoNum = 0;
  int angle = 90;

void setup() { 
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");

  pwm.begin();
  // In theory the internal oscillator is 25MHz but it really isn't
  // that precise. You can 'calibrate' by tweaking this number till
  // you get the frequency you're expecting!
  pwm.setOscillatorFrequency(27000000);  // The int.osc. is closer to 27MHz  
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  delay(10);
}

// You can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. It's not precise!
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

void loop() {
  //updateAngles(30);
  walk(5,15);
  
}



void walk(int sec,int stepAngle)
{
  for(int i = 0;i < stepAngle ; i++){ legs[0]=legs[0]+1;legs[1]=legs[1]-0;         legs[2]=legs[2]-0;legs[3]=legs[3]-1;         legs[4]=legs[4]-0;legs[5]=legs[5]+1;          legs[6]=legs[6]-1;legs[7]=legs[7]-0;    updateAngles(sec);}
  for(int i = 0;i < stepAngle ; i++){ legs[0]=legs[0]+0;legs[1]=legs[1]-1;         legs[2]=legs[2]-1;legs[3]=legs[3]-0;         legs[4]=legs[4]+1;legs[5]=legs[5]+0;          legs[6]=legs[6]-0;legs[7]=legs[7]+1;    updateAngles(sec);}
  for(int i = 0;i < stepAngle ; i++){ legs[0]=legs[0]-1;legs[1]=legs[1]+0;         legs[2]=legs[2]+0;legs[3]=legs[3]+1;         legs[4]=legs[4]+0;legs[5]=legs[5]-1;          legs[6]=legs[6]+1;legs[7]=legs[7]-0;    updateAngles(sec);}
  for(int i = 0;i < stepAngle ; i++){ legs[0]=legs[0]+0;legs[1]=legs[1]+1;         legs[2]=legs[2]+1;legs[3]=legs[3]+0;         legs[4]=legs[4]-1;legs[5]=legs[5]-0;          legs[6]=legs[6]+0;legs[7]=legs[7]-1;    updateAngles(sec);}
}


void updateAngles(int sec)
{
  for(int i=0;i<8;i++)
  {
    setAngle(i,legs[i]);
  }
  delay(sec);
}

void setAngle(int servoNumber , int setAngle)
  {
    int pulseLenght = map(setAngle, 0, 180, SERVOMIN, SERVOMAX);
    pwm.setPWM(servoNumber, 0, pulseLenght);
  }
