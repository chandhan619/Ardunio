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

#define WIFI_SSID "ACTFIBERNET"               
#define WIFI_PASS "act12345"

fauxmoESP fauxmo;

int retry = 100;
int currentRetry = 0;

#define Relay1 D0
#define Relay2 D1
#define Relay3 D2
#define Relay4 D3


#define Switch1 D4
#define Switch2 D5
#define Switch3 D6
#define Switch4 D7

#define Enable D8

int State_1 = 0;
int State_2 = 0;
int State_3 = 0;
int State_4 = 0;


#define Tiny_1 "Tiny PCB 1"
#define Tiny_2 "Tiny PCB 2"
#define Tiny_3 "Tiny PCB 3"
#define Tiny_4 "Tiny PCB 4"

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

  Serial.begin(115200);

  
  pinMode(Relay1, OUTPUT);     
  pinMode(Relay2, OUTPUT);     
  pinMode(Relay3, OUTPUT);     
  pinMode(Relay4, OUTPUT); 

  pinMode(Switch1, INPUT_PULLUP);     
  pinMode(Switch2, INPUT_PULLUP);     
  pinMode(Switch3, INPUT_PULLUP);     
  pinMode(Switch4, INPUT_PULLUP); 


  //Signal Enable Pin
  pinMode(Enable, OUTPUT);
  digitalWrite(Enable,HIGH);


  //setup and wifi connection
  wifiSetup();

  // Update current pin status
     State_1 = digitalRead(Switch1);
                              if (State_1 == HIGH)
                              {
                                digitalWrite(Relay1, LOW);
                                Serial.println("Relay 1 OFF");
                              }
                              else if(State_1 == LOW)
                              {
                                digitalWrite(Relay1, HIGH);
                                Serial.println("Relay 1 ON");
                              }

                              
     State_2 = digitalRead(Switch2);
                              if (State_2 == HIGH)
                              {
                                digitalWrite(Relay2, LOW);
                                Serial.println("Relay 2 OFF");
                              }
                              else if(State_2 == LOW)
                              {
                                digitalWrite(Relay2, HIGH);
                                Serial.println("Relay 2 ON");
                              }

                              
     State_3 = digitalRead(Switch3);
                              if (State_3 == HIGH)
                              {
                                digitalWrite(Relay3, LOW);
                                Serial.println("Relay 3 OFF");
                              }
                              else if(State_3 == LOW)
                              {
                                digitalWrite(Relay3, HIGH);
                                Serial.println("Relay 3 ON");
                              }

                              
    State_4 = digitalRead(Switch4);
                              if (State_4 == HIGH)
                              {
                                digitalWrite(Relay4, LOW);
                                Serial.println("Relay 4 OFF");
                              }
                              else if(State_4 == LOW)
                              {
                                digitalWrite(Relay4, HIGH);
                                Serial.println("Relay 4 ON");
                              }
  




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
  fauxmo.addDevice(Tiny_2);
  fauxmo.addDevice(Tiny_3);
  fauxmo.addDevice(Tiny_4);

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
                      
                    }
                    else
                    {
                     digitalWrite(Relay1, LOW);
                     
                    }
              }
              else if ((strcmp(device_name, Tiny_2) == 0) )
              {
                    if (state)
                    {
                     digitalWrite(Relay2, HIGH);
                      
                    }
                    else
                    {
                     digitalWrite(Relay2, LOW);
                     
                    }
              }
              else if ((strcmp(device_name, Tiny_3) == 0) )
              {
                    if (state)
                    {
                     digitalWrite(Relay3, HIGH);
                      
                    }
                    else
                    {
                     digitalWrite(Relay3, LOW);
                     
                    }
              }
              else if ((strcmp(device_name, Tiny_4) == 0) )
              {
                    if (state)
                    {
                     digitalWrite(Relay4, HIGH);
                      
                    }
                    else
                    {
                     digitalWrite(Relay4, LOW);
                     
                    }
              }

});

}


void loop() {

  Serial.print(digitalRead(Switch1));
  Serial.print(" ");
  Serial.print(digitalRead(Switch2));
  Serial.print(" ");
  Serial.print(digitalRead(Switch3));
  Serial.print(" ");
  Serial.print(digitalRead(Switch4));
  Serial.println(" ");

static unsigned long last1 = millis();

if (millis() - last1 > 500)
{
  last1 = millis();

  
  if (digitalRead(Switch1) != State_1)
  {
    
    State_1 = digitalRead(Switch1);
    
    if (State_1 == HIGH)
    {
      digitalWrite(Relay1, LOW);
      Serial.println("Relay 1 OFF");
    }
    else if(State_1 == LOW)
    {
      digitalWrite(Relay1, HIGH);
      Serial.println("Relay 1 ON");
    }
  }



  
  else if (digitalRead(Switch2) != State_2)
  {
    
    State_2 = digitalRead(Switch2);
    
    if (State_2 == HIGH)
    {
      digitalWrite(Relay2, LOW);
      Serial.println("Relay 2 OFF");
    }
    else if(State_2 == LOW)
    {
      digitalWrite(Relay2, HIGH);
      Serial.println("Relay 2 ON");
    }
  }



  
  else if (digitalRead(Switch3) != State_3)
  {
    
    State_3 = digitalRead(Switch3);
    
    if (State_3 == HIGH)
    {
      digitalWrite(Relay3, LOW);
      Serial.println("Relay 3 OFF");
    }
    else if(State_3 == LOW)
    {
      digitalWrite(Relay3, HIGH);
      Serial.println("Relay 3 ON");
    }
  }




  
  else if (digitalRead(Switch4) != State_4)
  {
    
    State_4 = digitalRead(Switch4);
    
    if (State_4 == HIGH)
    {
      digitalWrite(Relay4, LOW);
      Serial.println("Relay 4 OFF");
    }
    else if(State_4 == LOW)
    {
      digitalWrite(Relay4, HIGH);
      Serial.println("Relay 4 ON");
    }
  }

  
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
          
          //Serial.println(State_1);
          //Serial.println(State_2);
          //Serial.println(State_3);
          //Serial.println(State_4);
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
