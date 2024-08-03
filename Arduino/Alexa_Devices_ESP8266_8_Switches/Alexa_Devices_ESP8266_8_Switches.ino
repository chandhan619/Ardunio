#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include "fauxmoESP.h"

#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800,60000)  ;
String arr_days[]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
String date_time;


#define WIFI_SSID "Kalyani"           
#define WIFI_PASS "7095321888"
 
//#define WIFI_SSID "ACTFIBERNET"               
//#define WIFI_PASS "act12345"

 
#define SERIAL_BAUDRATE 115200
                                

#define Device1 "Switch 1"
#define Device2 "Switch 2"
#define Device3 "Switch 3"
#define Device4 "Switch 4"
#define Device5 "Switch 5"
#define Device6 "Switch 6"
#define Device7 "Switch 7"
#define Device8 "Switch 8"
//#define Device9 "Switch 9"
#define DeviceA "Switch A"
#define DeviceB "Switch B"

int retry = 1000;
int currentRetry = 0;

fauxmoESP fauxmo;
// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------

 
void wifiSetup() {
 
// Set WIFI module to STA mode
WiFi.mode(WIFI_STA);
 
// Connect
Serial.printf("[WIFI] Connecting to %s```   +++ ", WIFI_SSID);
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
Serial.printf("ESP8266 : ON");

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
fauxmo.addDevice(Device1);
fauxmo.addDevice(Device2);
fauxmo.addDevice(Device3);
fauxmo.addDevice(Device4);
fauxmo.addDevice(Device5);
fauxmo.addDevice(Device6);
fauxmo.addDevice(Device7);
fauxmo.addDevice(Device8);
//fauxmo.addDevice(Device9);
fauxmo.addDevice(DeviceA);
fauxmo.addDevice(DeviceB);
 
// fauxmoESP 2.0.0 has changed the callback signature to add the device_id,
// this way it's easier to match devices to action without having to compare strings.
fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
//Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
 
//Serial.printf("Data(%s,%s)",device_name,state ? "ON" : "OFF");
Serial.printf("%s,%s",device_name,state ? "1" : "0");
Serial.println();
});

 
}


void updateTime()
{

  
  timeClient.update();
  
  int hh = timeClient.getHours();
  int mm = timeClient.getMinutes();
  int ss = timeClient.getSeconds();
  Serial.print("Time:");
  if(hh>12)
  {
    hh=hh-12;
    Serial.print(hh);
    Serial.print(":");
    Serial.print(mm);
    Serial.print(":");
    Serial.print(ss);
    Serial.print(" PM");
  }
  else
  {
    Serial.print(hh);
    Serial.print(":");
    Serial.print(mm);
    Serial.print(":");
    Serial.print(ss);
    Serial.print(" AM");   
  }
  Serial.print(",");
  int day = timeClient.getDay();
  Serial.print(arr_days[day]);
  Serial.print(",");
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
  Serial.print(" ");
  Serial.print(monthStr);
  Serial.print(" ");
  Serial.println(yearStr);
}


 
void loop() {

unsigned long last = millis();

updateTime();
delay(1000);

// Since fauxmoESP 2.0 the library uses the "compatibility" mode by
// default, this means that it uses WiFiUdp class instead of AsyncUDP.
// The later requires the Arduino Core for ESP8266 staging version
// whilst the former works fine with current stable 2.3.0 version.
// But, since it's not "async" anymore we have to manually poll for UDP
// packets
fauxmo.handle();

          if (millis() - last > 5000) {
                    last = millis();
                    //Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
                    
                    
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
