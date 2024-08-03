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
#define RELAY1 D2
#define RELAY2 D1  //GPIO5  //D1

#define LAMP_1 "Light"
#define LAMP_2 "Fan"
#define LAMP_3 "Ac"

int LED_Brightness = 255;
int currentState = LOW;

int retry = 100;
int currentRetry = 0;

fauxmoESP fauxmo;
// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------


#include <IRremote.hpp>

#define RAW_DATA_LEN 198
uint16_t rawDataOff[RAW_DATA_LEN]={
  5990, 7474, 438, 1758, 506, 1646, 526, 1642, 
  418, 1758, 502, 1642, 494, 1682, 498, 1650, 
  550, 1618, 442, 702, 510, 606, 466, 622, 
  462, 626, 502, 610, 446, 642, 438, 650, 
  502, 614, 474, 1674, 526, 1646, 438, 1714, 
  522, 1646, 490, 1686, 502, 1642, 522, 1654, 
  502, 1650, 526, 642, 474, 590, 434, 678, 
  498, 594, 498, 614, 390, 674, 522, 590, 
  502, 614, 442, 1710, 522, 1646, 418, 1742, 
  522, 1642, 422, 1738, 518, 1650, 438, 1718, 
  522, 1646, 418, 726, 498, 590, 498, 618, 
  434, 630, 526, 586, 502, 614, 442, 622, 
  466, 630, 518, 1674, 466, 1694, 518, 1650, 
  438, 654, 522, 1646, 502, 1654, 522, 594, 
  498, 1650, 438, 682, 522, 590, 502, 586, 
  442, 1714, 518, 594, 498, 590, 438, 1718, 
  518, 590, 502, 1678, 494, 1678, 502, 1650, 
  498, 602, 514, 1654, 438, 650, 494, 1682, 
  502, 1654, 486, 634, 514, 594, 466, 622, 
  466, 1694, 514, 594, 494, 1658, 490, 606, 
  514, 594, 498, 618, 466, 1694, 510, 598, 
  442, 1710, 514, 582, 510, 1658, 438, 654, 
  490, 606, 510, 1686, 462, 630, 518, 1658, 
  490, 598, 462, 1694, 514, 598, 494, 1658, 
  486, 1690, 490, 7418, 466, 1000};
  
uint16_t rawDataOn[RAW_DATA_LEN]={
  6014, 7450, 466, 1718, 482, 1706, 466, 1706, 
  438, 1710, 490, 1658, 490, 1682, 462, 1710, 
  466, 1682, 462, 678, 466, 646, 438, 650, 
  466, 626, 462, 646, 442, 646, 466, 626, 
  462, 650, 438, 1718, 482, 1686, 486, 1686, 
  466, 1682, 490, 1686, 462, 1686, 490, 1682, 
  466, 1686, 486, 674, 442, 646, 470, 622, 
  466, 626, 462, 646, 442, 650, 466, 622, 
  466, 646, 442, 1738, 466, 1682, 490, 1686, 
  466, 1682, 490, 1686, 466, 1682, 494, 1682, 
  466, 1682, 494, 650, 462, 650, 438, 650, 
  466, 626, 466, 646, 442, 646, 442, 650, 
  466, 626, 462, 1714, 490, 626, 466, 1682, 
  466, 646, 470, 1686, 462, 1686, 494, 622, 
  466, 1686, 462, 674, 470, 1686, 462, 650, 
  438, 1694, 486, 626, 462, 626, 462, 1694, 
  486, 626, 462, 1714, 490, 1686, 466, 1686, 
  490, 602, 490, 1686, 462, 626, 490, 1666, 
  486, 1686, 466, 654, 486, 626, 466, 622, 
  466, 1690, 490, 622, 466, 1686, 466, 626, 
  486, 610, 482, 650, 462, 1694, 490, 622, 
  466, 1686, 462, 630, 490, 1682, 470, 622, 
  462, 630, 490, 1714, 462, 626, 466, 1694, 
  482, 626, 466, 1686, 462, 630, 490, 1686, 
  466, 1686, 490, 7430, 478, 1000};




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


IrSender.begin(D6, LED_BUILTIN);
 
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
fauxmo.addDevice(LAMP_3);

 
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
              else if ( (strcmp(device_name, LAMP_3) == 0) )
              {
                    if (!state)
                    {
                     IrSender.sendRaw(rawDataOff, sizeof(rawDataOff) / sizeof(rawDataOff[0]), NEC_KHZ);
                    
                     Serial.print("AC OFF ");
                    }
                    else
                    {
                     IrSender.sendRaw(rawDataOn, sizeof(rawDataOn) / sizeof(rawDataOn[0]), NEC_KHZ);
                    
                     Serial.print("AC ON ");
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
