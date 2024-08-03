/*
  ESP8266 Blink by Simon Peter
  Blink the blue LED on the ESP-01 module
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/

#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif


#include "fauxmoESP.h"


//#define WIFI_SSID "Kalyani"               
//#define WIFI_PASS "7095321888"

#define WIFI_SSID "Network"               
#define WIFI_PASS "Aegon@123"

fauxmoESP fauxmo;

int retry = 100;
int currentRetry = 0;

#define Relay1 D1

#define Switch1 D5

#define Enable D8

int State_1 = 0;

int relayState = LOW;
#define Tiny_1 "Geyser"

#define onTime 1000*60*15
static unsigned long last = 0;
int lastOnTime = 0;
void wifiSetup() {
 
      // Set WIFI module to STA mode
      WiFi.mode(WIFI_STA);
       
      // Connect
      Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
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
      
      }
 
}


void setup() {

delay(1000*60*5);

  Serial.begin(115200);

  
  pinMode(Relay1, OUTPUT);     

  pinMode(Switch1, INPUT_PULLUP);      


  //Signal Enable Pin
  pinMode(Enable, OUTPUT);
  digitalWrite(Enable,HIGH);


  //setup and wifi connection
  wifiSetup();

                              




  //setup devices to be discovered by Alexa
  fauxmo.createServer(true);
  fauxmo.setPort(80);

  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.printf("Device Discovery on");
    fauxmo.enable(true);
  }
  else
  {
    Serial.printf("Device Discovery Off");
    fauxmo.enable(false);
  }

  fauxmo.addDevice(Tiny_1);

  // fauxmoESP 2.0.0 has changed the callback signature to add the device_id,
// this way it's easier to match devices to action without having to compare strings.
fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
 
//Switching action on detection of device name
 
              if ((strcmp(device_name, Tiny_1) == 0) )
              {
                    if (state)
                    {
                      digitalWrite(Relay1, HIGH);
                      relayState = HIGH;
                      lastOnTime = last;
                    }
                    else
                    {
                      digitalWrite(Relay1, LOW);
                      relayState = LOW;
                    }
              }
             
              

});

}


void loop() {

last = millis();
  
if(relayState==HIGH)
{
  if( last - lastOnTime > onTime)
  {
      digitalWrite(Relay1, LOW);
      Serial.println("Relay 1 OFF");
      relayState = LOW;
  }
}
  //Serial.println(digitalRead(Switch1));

Serial.println( last - lastOnTime );


Serial.println(digitalRead(Switch1));

if (digitalRead(Switch1) == 1)
{
  delay(200);
  if (digitalRead(Switch1) == 1)
  {
      if (relayState == HIGH)
        {
          digitalWrite(Relay1, LOW);
          Serial.println("Relay 1 OFF");
          relayState = LOW;
        }
        else if(State_1 == LOW)
        {
          digitalWrite(Relay1, HIGH);
          Serial.println("Relay 1 ON");
          relayState = HIGH;
          lastOnTime = last;
        }
        delay(1000);
  }
}


  

// Since fauxmoESP 2.0 the library uses the "compatibility" mode by
// default, this means that it uses WiFiUdp class instead of AsyncUDP.
// The later requires the Arduino Core for ESP8266 staging version
// whilst the former works fine with current stable 2.3.0 version.
// But, since it's not "async" anymore we have to manually poll for UDP
// packets
fauxmo.handle();

if (millis() - last > 5000) {
last = millis();
          Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
          
          //Serial.println(State_1);
          if(WiFi.status() != WL_CONNECTED)
          {
            wifiSetup();
            if(WiFi.status() == WL_CONNECTED)
            {
                    fauxmo.enable(true);
                    Serial.println("Online");
            }
            else
            {
                    fauxmo.enable(false);
                    Serial.println("Offline");
            }
          }
}
            
}
