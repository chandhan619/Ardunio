#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NTPClient.h>
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include "fauxmoESP.h"

#include <WiFiUdp.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels



Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800,60000)  ;
String arr_days[]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
String date_time;


 
#define WIFI_SSID "Kalyani"               
#define WIFI_PASS "7095321888"

//#define WIFI_SSID "ACTFIBERNET"               
//#define WIFI_PASS "act12345"
 
#define SERIAL_BAUDRATE 115200
#define LED 2

#define LEDStatus D0
 
/* Set Relay Pins */
#define RELAY_1 D7
#define RELAY_2 D8
//#define RELAY_3 D9   //RX 
#define RELAY_4 D10  //TX


#define SWITCH1 D6  //GPIO5  //D1
#define SWITCH2 D5  //GPIO4  //D2
//#define SWITCH3 D4  //GPIO9 //SDD3
#define SWITCH4 D3 //GPIO10  //SDD2


#define LAMP_1 "Light One"
#define LAMP_2 "Light Two"
//#define LAMP_3 "Light Three"
#define LAMP_4 "Light Three"

int currentState1 = HIGH;
int currentState2 = HIGH;
//int currentState3 = HIGH;
int currentState4 = HIGH;

int currentState = LOW;

int retry = 100;
int currentRetry = 0;

fauxmoESP fauxmo;
// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------


 
void wifiSetup() {
 
// Set WIFI module to STA mode
WiFi.mode(WIFI_STA);
 
// Connect
Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
setMessageOnDisplay("Connecting ",1,true);
WiFi.begin(WIFI_SSID, WIFI_PASS);
 
// Wait
while (WiFi.status() != WL_CONNECTED && currentRetry < retry ) {
Serial.print(".");
//setMessageOnDisplay(".",1,false);

delay(100);
currentRetry++;
}

currentRetry = 0;
Serial.println();
if( WiFi.status() == WL_CONNECTED )
{
// Connected!
Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
setMessageOnDisplay("Connected!",1,true);

}
 
}
void setMessageOnDisplay(String message,int fontSize,boolean refresh)
{
  if(refresh)
  {
  display.clearDisplay();
  display.setTextSize(fontSize);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  }
  display.println(message);
  display.display(); 
}
  
  
  
void updateTime()
{

  
  timeClient.update();
  
  display.clearDisplay();
  
  Serial.println(timeClient.getFormattedTime());
  
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  int hh = timeClient.getHours();
  int mm = timeClient.getMinutes();
  int ss = timeClient.getSeconds();
  
  if(hh>12)
  {
    hh=hh-12;
    display.print(hh);
    display.print(":");
    display.print(mm);
   // display.print(":");
   // display.print(ss);
   display.setTextSize(1);
    display.println(" PM");
  }
  else
  {
    display.print(hh);
    display.print(":");
    display.print(mm);
    //display.print(":");
    //display.print(ss);
    display.setTextSize(1);
    display.println(" AM");   
  }
display.println("");
display.println("");
  int day = timeClient.getDay();
  display.println(arr_days[day]);
display.setTextSize(2);
  display.println("");
  
  time_t rawtime = timeClient.getEpochTime();

   struct tm * ti;
   ti = localtime (&rawtime);

   uint16_t year = ti->tm_year + 1900;
   String yearStr = String(year);

   uint8_t month = ti->tm_mon + 1;
   String monthStr = month < 10 ? "0" + String(month) : String(month);

   uint8_t day1 = ti->tm_mday;
   String dayStr = day1 < 10 ? "0" + String(day1) : String(day1);


  Serial.print(dayStr);
  display.print(dayStr);

  Serial.print(" ");
  display.print(" ");

  Serial.print(monthStr);
  display.print(monthStr);

  Serial.print(" ");
  display.print(" ");

  Serial.print(yearStr);
  display.print(yearStr);

  
  display.display();      // Show initial text

}

void updateRelayState()
{
 
  setMessageOnDisplay(String(digitalRead(SWITCH1)),1,true);
  setMessageOnDisplay(String(digitalRead(SWITCH2)),1,false);
 // setMessageOnDisplay(String(digitalRead(SWITCH3)),1,false);
  setMessageOnDisplay(String(digitalRead(SWITCH4)),1,false);
}
 
void setup() {
 
Serial.begin(SERIAL_BAUDRATE);


//Setting up display
if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }



 
//Set relay pins to outputs
pinMode(RELAY_1, OUTPUT);
pinMode(RELAY_2, OUTPUT);
//pinMode(RELAY_3, OUTPUT);
pinMode(RELAY_4, OUTPUT);

pinMode(SWITCH1, 3);
pinMode(SWITCH2, 3);
//pinMode(SWITCH3, 3);
pinMode(SWITCH4, 3);

pinMode(LEDStatus, OUTPUT);

digitalWrite(LEDStatus, HIGH);

 
//Set each relay pin to HIGH ====== NOTE THAT THE RELAYS USE INVERSE LOGIC =====
digitalWrite(RELAY_1, currentState1);
delay(500);
digitalWrite(RELAY_2, currentState2);
delay(500);
//digitalWrite(RELAY_3, currentState3);
//delay(500);
digitalWrite(RELAY_4, currentState4);
delay(500);

//setup and wifi connection
wifiSetup();
 
// You have to call enable(true) once you have a WiFi connection
// You can enable or disable the library at any moment
// Disabling it will prevent the devices from being discovered and switched
fauxmo.createServer(true);
fauxmo.setPort(80);

if(WiFi.status() == WL_CONNECTED)
{
  fauxmo.enable(true);
  digitalWrite(LEDStatus, LOW);
}
else
{
  fauxmo.enable(false);
  digitalWrite(LEDStatus, HIGH);
}
// Device Names for Simulated Wemo switches
fauxmo.addDevice(LAMP_1);
fauxmo.addDevice(LAMP_2);
//fauxmo.addDevice(LAMP_3);
fauxmo.addDevice(LAMP_4);

 
// fauxmoESP 2.0.0 has changed the callback signature to add the device_id,
// this way it's easier to match devices to action without having to compare strings.
fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
digitalWrite(LED, !state);
 
//Switching action on detection of device name
 
              if ( (strcmp(device_name, LAMP_1) == 0) )
              {
                    if (!state)
                    {
                     digitalWrite(RELAY_1, HIGH);
                      
                    }
                    else
                    {
                     digitalWrite(RELAY_1, LOW);
                     
                    }
              }

              if ( (strcmp(device_name,LAMP_2) == 0) )
              {
                    if (!state)
                    {
                      digitalWrite(RELAY_2, HIGH);
                   
                    }
                    else
                    {
                      digitalWrite(RELAY_2, LOW);
                   
                    }
              }

         /*     if ( (strcmp(device_name,LAMP_3) == 0) )
              {
                    if (!state)
                    {
                      digitalWrite(RELAY_3, HIGH);
                    
                    }
                    else
                    {
                     digitalWrite(RELAY_3, LOW);
                   
                    }
              }
      */
              if ( (strcmp(device_name, LAMP_4) == 0) )
              {
                    if (!state)
                    {
                         digitalWrite(RELAY_4, HIGH);

                    }
                    else
                    {
                        digitalWrite(RELAY_4, LOW);

                    }
              }



});
 
}



 
void loop() {

     updateTime();
     //updateRelayState();
    
    
    if(digitalRead(SWITCH1)==HIGH && currentState1 != LOW)
    {
       digitalWrite(RELAY_1, LOW);
       currentState1 = LOW;
    }
    if(digitalRead(SWITCH1)==LOW && currentState1 != HIGH)
    {
      digitalWrite(RELAY_1, HIGH);
      currentState1 = HIGH;
    }


    if(digitalRead(SWITCH2)==HIGH && currentState2 != LOW)
    {
       digitalWrite(RELAY_2, LOW);
       currentState2 = LOW;
    }
    if(digitalRead(SWITCH2)==LOW && currentState2 != HIGH)
    {
      digitalWrite(RELAY_2, HIGH);
      currentState2 = HIGH;
    }
/*
    if(digitalRead(SWITCH3)==HIGH && currentState3 != LOW)
    {
       digitalWrite(RELAY_3, LOW);
       currentState3 = LOW;
    }
   if(digitalRead(SWITCH3)==LOW && currentState3 != HIGH)
    {
      digitalWrite(RELAY_3, HIGH);
      currentState3 = HIGH;
    }
*/

    if(digitalRead(SWITCH4)==HIGH && currentState4 != LOW)
    {
       digitalWrite(RELAY_4, LOW);
       currentState4 = LOW;
    }
    if(digitalRead(SWITCH4)==LOW && currentState4 != HIGH)
    {
      digitalWrite(RELAY_4, HIGH);
      currentState4 = HIGH;
    }

    


// Since fauxmoESP 2.0 the library uses the "compatibility" mode by
// default, this means that it uses WiFiUdp class instead of AsyncUDP.
// The later requires the Arduino Core for ESP8266 staging version
// whilst the former works fine with current stable 2.3.0 version.
// But, since it's not "async" anymore we have to manually poll for UDP
// packets
fauxmo.handle();
 
static unsigned long last = millis();

if (millis() - last > 5000) {
last = millis();
          Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
          
          Serial.println(currentState1);
          Serial.println(currentState2);
         // Serial.println(currentState3);
          Serial.println(currentState4);
          if(WiFi.status() != WL_CONNECTED)
          {
            wifiSetup();
            if(WiFi.status() == WL_CONNECTED)
            {
                    fauxmo.enable(true);
                    digitalWrite(LEDStatus, LOW);
                    Serial.println("Online");
                    
                    setMessageOnDisplay("Online!",1,true);
            }
            else
            {
                  fauxmo.enable(false);
                  digitalWrite(LEDStatus, HIGH);
                  Serial.println("Offline");
                  setMessageOnDisplay("Offline!",1,true);
            }
          }
}
 
}
