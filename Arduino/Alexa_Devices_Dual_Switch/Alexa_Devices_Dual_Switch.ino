#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include "fauxmoESP.h"

#define WIFI_SSID "ACTFIBERNET"               
#define WIFI_PASS "act12345"

//#define WIFI_SSID "Kalyani"               
//#define WIFI_PASS "7095321888"
 
#define SERIAL_BAUDRATE 115200
                                

/* Set Relay Pins */
#define RELAY1 D1
#define RELAY2 D2  //GPIO5  //D1

#define LAMP_1 "FAN"
#define LAMP_2 "LIGHT"

int LED_Brightness = 255;
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
WiFi.begin(WIFI_SSID, WIFI_PASS);
 
// Wait
while (WiFi.status() != WL_CONNECTED && currentRetry < retry ) {
Serial.print(".");


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
 
Serial.begin(SERIAL_BAUDRATE);

 
//Set relay pins to outputs
pinMode(RELAY1, OUTPUT);
pinMode(RELAY2, OUTPUT);

//digitalWrite(RELAY, LOW);
 
//Set each relay pin to HIGH ====== NOTE THAT THE RELAYS USE INVERSE LOGIC =====
digitalWrite(RELAY1, HIGH);
digitalWrite(RELAY2, HIGH);
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
  
}
else
{
  fauxmo.enable(false);
  
}
// Device Names for Simulated Wemo switches
fauxmo.addDevice(LAMP_1);
fauxmo.addDevice(LAMP_2);

 
// fauxmoESP 2.0.0 has changed the callback signature to add the device_id,
// this way it's easier to match devices to action without having to compare strings.
fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
 
//Switching action on detection of device name
 
              if ( (strcmp(device_name, LAMP_1) == 0) )
              {
                    if (!state)
                    {
                     digitalWrite(RELAY1, HIGH);
                    
                     Serial.print("Relay OFF ");
                    }
                    else
                    {
                     digitalWrite(RELAY1, LOW);
                   
                     Serial.print("Relay ON ");
                    }
              }
              else if ( (strcmp(device_name, LAMP_2) == 0) )
              {
                    if (!state)
                    {
                     digitalWrite(RELAY2, HIGH);
                    
                     Serial.print("Relay OFF ");
                    }
                    else
                    {
                     digitalWrite(RELAY2, LOW);
                   
                     Serial.print("Relay ON ");
                    }
              }



});
 
}
 
void loop() {


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


if(WiFi.status() != WL_CONNECTED)
{
  wifiSetup();   
  if(WiFi.status() == WL_CONNECTED)
  {
    fauxmo.enable(true);
    // Serial.println("Online");
  }
  else
  {
    fauxmo.enable(false);
   // Serial.println("Offline");
}
}
}
 
}
