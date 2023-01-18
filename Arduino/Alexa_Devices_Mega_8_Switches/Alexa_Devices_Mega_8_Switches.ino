

#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1327_WS_128X128_F_4W_HW_SPI u8g2(/* CLK = 52 , DIN =  51 */ U8G2_R0, /* cs=*/ 10, /* dc=*/ 7, /* reset=*/ 8);

String readString;

#define Relay1 23
#define Relay2 25
#define Relay3 27
#define Relay4 29
#define Relay5 31
#define Relay6 33
#define Relay7 35
#define Relay8 37
#define Relay9 39
#define RelayA 41 


#define Touch1 22
#define Touch2 24
#define Touch3 26
#define Touch4 28
#define Touch5 30
#define Touch6 32


#define Touch1_LED 2
#define Touch2_LED 3
#define Touch3_LED 4
#define Touch4_LED 5
#define Touch5_LED 6
#define Touch6_LED 9



int incomingByte = 0; // for incoming serial data

int Switch1 = HIGH;
int Switch2 = HIGH;
int Switch3 = HIGH;
int Switch4 = HIGH;
int Switch5 = HIGH;
int Switch6 = HIGH;
int Switch7 = HIGH;
int Switch8 = HIGH;
int Switch9 = HIGH;
int SwitchA = HIGH;
int SwitchB = HIGH;

int LED_Brightness = 255;
int switch_Delay = 500;

String currentTime = "Dummy";
String currentDate = "Dummy";
String currentDay = "Dummy";
String amOrPm = "AM";

int period = 1000;
unsigned long time_now = 0;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  // put your setup code here, to run once:
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  pinMode(Relay5, OUTPUT);
  pinMode(Relay6, OUTPUT);
  pinMode(Relay7, OUTPUT);
  pinMode(Relay8, OUTPUT);
  pinMode(Relay9, OUTPUT);
  pinMode(RelayA, OUTPUT);

  pinMode(Touch1_LED, OUTPUT);
  pinMode(Touch2_LED, OUTPUT);
  pinMode(Touch3_LED, OUTPUT);
  pinMode(Touch4_LED, OUTPUT);
  pinMode(Touch5_LED, OUTPUT);
  pinMode(Touch6_LED, OUTPUT);

  //  analogWrite(Touch1_LED,LED_Brightness);
  //  analogWrite(Touch2_LED,LED_Brightness);
  //  analogWrite(Touch3_LED,LED_Brightness);
  // analogWrite(Touch4_LED,LED_Brightness);
  //  analogWrite(Touch5_LED,LED_Brightness);
  //analogWrite(Touch6_LED,LED_Brightness);


  pinMode(Touch1, INPUT);
  pinMode(Touch2, INPUT);
  pinMode(Touch3, INPUT);
  pinMode(Touch4, INPUT);
  pinMode(Touch5, INPUT);
  pinMode(Touch6, INPUT);

  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, LOW);
  digitalWrite(Relay3, LOW);
  digitalWrite(Relay4, LOW);
  digitalWrite(Relay5, LOW);
  digitalWrite(Relay6, LOW);
  digitalWrite(Relay7, LOW);
  digitalWrite(Relay8, LOW);
  digitalWrite(Relay9, LOW);
  digitalWrite(RelayA, LOW);


  u8g2.begin();

}

void loop() {

  time_now = millis();
  // Serial.print(digitalRead(Touch1));
  // Serial.print(digitalRead(Touch2));
  // Serial.print(digitalRead(Touch3));
  // Serial.print(digitalRead(Touch4));
  // Serial.print(digitalRead(Touch5));
  // Serial.println(digitalRead(Touch6));



  if (digitalRead(Touch1) == 1)
  {
    if (Switch1 == LOW)
    {
      Switch1 = HIGH;
      analogWrite(Touch1_LED, 0);
      digitalWrite(Relay1, LOW);
    }
    else
    {
      Switch1 = LOW;
      analogWrite(Touch1_LED, LED_Brightness);
      digitalWrite(Relay1, HIGH);
    }
    
    delay(switch_Delay);
  }
  else if (digitalRead(Touch2) == 1)
  {
    if (Switch2 == LOW)
    {
      Switch2 = HIGH;
      analogWrite(Touch2_LED, 0);
      digitalWrite(Relay2, LOW);
    }
    else
    {
      Switch2 = LOW;
      analogWrite(Touch2_LED, LED_Brightness);
      digitalWrite(Relay2, HIGH);
    }
    delay(switch_Delay);
  }
  else if (digitalRead(Touch3) == 1)
  {
    if (Switch3 == LOW)
    {
      Switch3 = HIGH;
      analogWrite(Touch3_LED, 0);
      digitalWrite(Relay3, LOW);
    }
    else
    {
      Switch3 = LOW;
      analogWrite(Touch3_LED, LED_Brightness);
      digitalWrite(Relay3, HIGH);
    }

    
    delay(switch_Delay);
  }
  else if (digitalRead(Touch4) == 1)
  {
    if (Switch4 == LOW)
    {
      Switch4 = HIGH;
      analogWrite(Touch4_LED, 0);
      digitalWrite(Relay4, LOW);
    }
    else
    {
      Switch4 = LOW;
      analogWrite(Touch4_LED, LED_Brightness);
      digitalWrite(Relay4, HIGH);
    }

    
    delay(switch_Delay);
  }
  else if (digitalRead(Touch5) == 1)
  {
    if (Switch5 == LOW)
    {
      Switch5 = HIGH;
      analogWrite(Touch5_LED, 0);
      digitalWrite(Relay5, LOW);
    }
    else
    {
      Switch5 = LOW;
      analogWrite(Touch5_LED, LED_Brightness);
      digitalWrite(Relay5, HIGH);
    }

    
    delay(switch_Delay);
  }
  else if (digitalRead(Touch6) == 1)
  {
    if (Switch6 == LOW)
    {
      Switch6 = HIGH;
      analogWrite(Touch6_LED, 0);
      digitalWrite(Relay6, LOW);
    }
    else
    {
      Switch6 = LOW;
      analogWrite(Touch6_LED, LED_Brightness);
      digitalWrite(Relay6, HIGH);
    }
    delay(switch_Delay);
  }




  if (Serial1.available()) {
    Serial.println("Incomming Bytes");
    readString = Serial1.readStringUntil('\n');
    Serial.println(readString);
    if(readString.startsWith("Switch"))
    {
      updateRelayStates(readString);
    }
    else
    {
      updateTimeInDisplay(readString);
    }
  }


  //Serial.print(Switch1);
  // Serial.print(Switch2);
  // Serial.print(Switch3);
  // Serial.print(Switch4);
  // Serial.print(Switch5);
  //Serial.print(Switch6);
  // Serial.print(Switch7);
  // Serial.println(Switch8);

     

}

void updateTimeInDisplay(String dataString)
{
 // Serial.println("Updating Time : "+dataString);
  
           u8g2.clearBuffer();
           //Time
           u8g2.setFont(u8g2_font_logisoso32_tn);
           u8g2.drawStr(0,40,currentTime.c_str());
           
           //AM or PM
           u8g2.setFont(u8g2_font_ncenB14_tr);
           u8g2.drawStr(95,30,amOrPm.c_str());
           
           //Day
           u8g2.setFont(u8g2_font_ncenB18_tr);
           u8g2.drawStr(0,80,currentDay.c_str());
           
           //Date
           u8g2.setFont(u8g2_font_logisoso16_tn);
           u8g2.drawStr(0,120,currentDate.c_str());
           u8g2.sendBuffer();

  //3:0:20 AM,Friday,13 05 2022

  amOrPm = dataString.substring(dataString.indexOf(",")-2,dataString.indexOf(","));
  Serial.println("AM or PM :"+amOrPm);
  
  currentTime = dataString.substring(5,dataString.indexOf(","));
  currentTime = currentTime.substring(0,currentTime.lastIndexOf(":"));
  
  Serial.println("Current Time :"+currentTime);
  dataString = dataString.substring(dataString.indexOf(",")+1,dataString.length());
  Serial.println("---->:"+dataString);

  currentDay = dataString.substring(0,dataString.indexOf(","));
  Serial.println("Current Day :"+currentDay);
  dataString.replace(currentDay+",","");

  currentDate = dataString;
  Serial.println("current Date :"+currentDate);


}

void updateRelayStates(String dataString)
{
  String deviceName = dataString.substring(0, 8);
  String state = dataString.substring(9, 10);
  Serial.println(deviceName);
  Serial.println(state);

  if (deviceName == "Switch 1")
  {
    if (state == "1")
    {
      Switch1 = LOW;
      digitalWrite(Relay1, HIGH);
      analogWrite(Touch1_LED, LED_Brightness);
    }
    else
    {
      Switch1 = HIGH;
      digitalWrite(Relay1, LOW);
      analogWrite(Touch1_LED, 0);
    }
  }
  else if (deviceName == "Switch 2")
  {
    if (state == "1")
    {
      Switch2 = LOW;
      digitalWrite(Relay2, HIGH);
      analogWrite(Touch2_LED, LED_Brightness);
    }
    else
    {
      Switch2 = HIGH;
      digitalWrite(Relay2, LOW);
      analogWrite(Touch2_LED, 0);
    }
  }
  else if (deviceName == "Switch 3")
  {
    if (state == "1")
    {
      Switch3 = LOW;
      digitalWrite(Relay3, HIGH);
      analogWrite(Touch3_LED, LED_Brightness);
    }
    else
    {
      Switch3 = HIGH;
      digitalWrite(Relay3, LOW);
      analogWrite(Touch3_LED, 0);
    }
  }
  else if (deviceName == "Switch 4")
  {
    if (state == "1")
    {
      Switch4 = LOW;
      digitalWrite(Relay4, HIGH);
      analogWrite(Touch4_LED, LED_Brightness);
    }
    else
    {
      Switch4 = HIGH;
      digitalWrite(Relay4, LOW);
      analogWrite(Touch4_LED, 0);
    }
  }
  else if (deviceName == "Switch 5")
  {
    if (state == "1")
    {
      Switch5 = LOW;
      digitalWrite(Relay5, HIGH);
      analogWrite(Touch5_LED, LED_Brightness);
    }
    else
    {
      Switch5 = HIGH;
      digitalWrite(Relay5, LOW);
      analogWrite(Touch5_LED, 0);
    }
  }
  else if (deviceName == "Switch 6")
  {
    if (state == "1")
    {
      Switch6 = LOW;
      digitalWrite(Relay6, HIGH);
      analogWrite(Touch6_LED, LED_Brightness);
    }
    else
    {
      Switch6 = HIGH;
      digitalWrite(Relay6, LOW);
      analogWrite(Touch6_LED, 0);
    }
  }
  else if (deviceName == "Switch 7")
  {
    if (state == "1")
    {
      Switch7 = LOW;
      digitalWrite(Relay7, HIGH);
      //analogWrite(Touch2_LED,LED_Brightness);
    }
    else
    {
      Switch7 = HIGH;
      digitalWrite(Relay7, LOW);
      //analogWrite(Touch2_LED,0);
    }
  }
  else if (deviceName == "Switch 8")
  {
    if (state == "1")
    {
      Switch8 = LOW;
      digitalWrite(Relay8, HIGH);
      //analogWrite(Touch8_LED,LED_Brightness);
    }
    else
    {
      Switch8 = HIGH;
      digitalWrite(Relay8, LOW);
      //analogWrite(Touch8_LED,0);
    }
  }
  else if (deviceName == "Switch 9")
  {
    if (state == "1")
    {
      Switch9 = LOW;
      digitalWrite(Relay9, HIGH);
      //analogWrite(Touch9_LED,LED_Brightness);
    }
    else
    {
      Switch9 = HIGH;
      digitalWrite(Relay9, LOW);
      //analogWrite(Touch9_LED,0);
    }
  }
  else if (deviceName == "Switch A")
  {
    if (state == "1")
    {
      SwitchA = LOW;
      digitalWrite(RelayA, HIGH);
      //analogWrite(Touch9_LED,LED_Brightness);
      Serial.println("Switch A On");
    }
    else
    {
      SwitchA = HIGH;
      digitalWrite(RelayA, LOW);
      //analogWrite(Touch9_LED,0);
      Serial.println("Switch A Off");
    }
  }
  else if (deviceName == "Switch B")
  {
    if (state == "1")
    {
      SwitchB = LOW;
      u8g2.setPowerSave(0);
      //digitalWrite(RelayB, HIGH);
      //analogWrite(Touch9_LED,LED_Brightness);
      Serial.println("Switch B On");
    }
    else
    {
      SwitchB = HIGH;
      u8g2.setPowerSave(1);
      //digitalWrite(RelayB, LOW);
      //analogWrite(Touch9_LED,0);
      Serial.println("Switch B Off");
    }
  }

}
