//This example demonstrates the ESP RainMaker with a standard Switch device.
#include "RMaker.h"
#include "WiFi.h"
#include "WiFiProv.h"

#define DEFAULT_POWER_MODE LOW
const char *service_name = "Bedroom Board";
const char *pop = "ajb1132e3";

//GPIO for push button
#if CONFIG_IDF_TARGET_ESP32C3
static int gpio_0 = 9;
static int gpio_switch = 7;
#else
//GPIO for virtual device
static int gpio_0 = 0;
#endif

#include <IRremote.hpp>

#define IR_Rev 15
#define IR_Send 2

int khz = 38;

#include "DHT.h"

// Uncomment one of the lines below for whatever DHT sensor type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

uint8_t DHTPin = 4;

// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE); 

float Temperature;
float Humidity;

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 1000*60;  //the value is a number of milliseconds
//const unsigned long period = 10;  //the value is a number of milliseconds

#define device1_Name "Light 1"   
#define device2_Name "Light 2"   
#define device3_Name "Bed Lamp"  
#define device4_Name "Fan"
#define device5_Name "AC_IR"


#define buzzer 13
#define pitch 100
#define buzzerDuration 0

#define debouncePeriod 20
#define refreshPeriod 300


#define device1_Relay 12    
#define device2_Relay 14  
#define device3_Relay 27  

#define device4_Relay_Mode_Full     26  
#define device4_Relay_Mode_High     25  
#define device4_Relay_Mode_Medium   32  
#define device4_Relay_Mode_LOW      33 



//LG AC Chandhan
uint16_t device5_AC_ON[] =    {3104, 9688, 520, 1548, 496, 532, 496, 528, 440, 588, 440, 1580, 492, 532, 496, 528, 496, 528, 496, 532, 440, 584, 448, 576, 468, 532, 492, 532, 496, 528, 468, 560, 440, 584, 444, 1576, 468, 552, 496, 508, 516, 1528, 492, 536, 460, 1580, 492, 536, 464, 532, 492, 1552, 492, 1556, 492, 532, 460, 1584, 468};
uint16_t device5_AC_OFF[] =   {3104, 9688, 492, 1556, 516, 532, 464, 564, 440, 560, 488, 1532, 492, 532, 492, 532, 492, 560, 440, 1580, 468, 1556, 516, 508, 516, 536, 460, 544, 456, 568, 464, 536, 492, 532, 512, 512, 488, 540, 492, 532, 492, 508, 492, 536, 492, 1552, 488, 540, 460, 1584, 488, 512, 492, 532, 516, 508, 492, 1556, 464};


#define device1_Button_Code       0xE916FF00
#define device2_Button_Code       0xE619FF00
#define device3_Button_Code       0xF20DFF00
#define device4_Button_Code       0xF30CFF00
#define device5_Button_Code       0xE718FF00

#define deviceUp_Code             0xB946FF00
#define deviceDown_Code           0xEA15FF00

#define device1_Switch 35
#define device2_Switch 34
#define device3_Switch 39
#define device4_Switch 36

#define device4_UP 16
#define device4_DOWN 17


static LightBulb  device1(device1_Name);
static LightBulb  device2(device2_Name);
static LightBulb  device3(device3_Name);

static Fan device4(device4_Name);
static int fan_Speed_Level = 5;

static Switch device5(device5_Name);

static TemperatureSensor temperature("Temperature");
static TemperatureSensor humidity("Humidity");

bool device1_currentState = false;
bool device2_currentState = false;
bool device3_currentState = false;
bool device4_currentState = false;
bool device5_currentState = false;

static int Fan_Speed=5;

String incommingData = ""; // for incoming serial data

void sysProvEvent(arduino_event_t *sys_event)
{
  switch (sys_event->event_id) {
    case ARDUINO_EVENT_PROV_START:
#if CONFIG_IDF_TARGET_ESP32S2
      Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on SoftAP\n", service_name, pop);
      printQR(service_name, pop, "softap");
#else
      Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on BLE\n", service_name, pop);
      printQR(service_name, pop, "ble");
#endif
      break;
    case ARDUINO_EVENT_PROV_INIT:
      wifi_prov_mgr_disable_auto_stop(10000);
      break;
    case ARDUINO_EVENT_PROV_CRED_SUCCESS:
      wifi_prov_mgr_stop_provisioning();
      break;
    default:;
  }
}

void write_callback(Device *device, Param *param, const param_val_t val, void *priv_data, write_ctx_t *ctx)
{
  makeNoice();
  const char *device_name = device->getDeviceName();
  const char *param_name = param->getParamName();
  Serial.println(device_name);
  Serial.println(param_name);
  if (strcmp(device_name, device1_Name) == 0)
  {
    if (strcmp(param_name, "Power") == 0) {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      device1_currentState = val.val.b;
      (device1_currentState == false) ? digitalWrite(device1_Relay, LOW) : digitalWrite(device1_Relay, HIGH);
      param->updateAndReport(val);
    }
  }

  else if (strcmp(device_name, device2_Name) == 0)
  {
    if (strcmp(param_name, "Power") == 0) {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      device2_currentState = val.val.b;
      (device2_currentState == false) ? digitalWrite(device2_Relay, LOW) : digitalWrite(device2_Relay, HIGH);
      param->updateAndReport(val);
    }
  }
  else if (strcmp(device_name, device3_Name) == 0)
  {
    if (strcmp(param_name, "Power") == 0) {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      device3_currentState = val.val.b;
      (device3_currentState == false) ? digitalWrite(device3_Relay, LOW) : digitalWrite(device3_Relay, HIGH);
      param->updateAndReport(val);
    }
  }
    else if (strcmp(device_name, device4_Name) == 0)
  {
    
      if (strcmp(param_name, "Power") == 0) {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      device4_currentState = val.val.b;
      Serial.println(fan_Speed_Level);
      updateFanSpeed();
      param->updateAndReport(val);
    }

      if (strcmp(param_name, "Speed") == 0) {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      fan_Speed_Level = val.val.i;
      Serial.println(fan_Speed_Level);
      updateFanSpeed();
      param->updateAndReport(val);
      }
  }
  else if (strcmp(device_name, device5_Name) == 0)
  {
    if (strcmp(param_name, "Power") == 0) {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      device5_currentState = val.val.b;
      (device5_currentState == false) ? sendRemoteSignalAC(LOW) : sendRemoteSignalAC(HIGH);
      param->updateAndReport(val);
    }
  }
}

void updateFanSpeed()
{
  Serial.print(device4_currentState);
  Serial.print(" ");
  Serial.println(fan_Speed_Level);
  
  if(device4_currentState == HIGH)
  {
      /*if(fan_Speed_Level==0)
      {
        Serial.print("Zero");
        digitalWrite(device4_Relay_Mode_Full, LOW);
        digitalWrite(device4_Relay_Mode_High, LOW);
        digitalWrite(device4_Relay_Mode_Medium, LOW);
        digitalWrite(device4_Relay_Mode_LOW, LOW);
      }
      */
      if(fan_Speed_Level==1)
      {
        Serial.print("One");
        digitalWrite(device4_Relay_Mode_Full, LOW);
        digitalWrite(device4_Relay_Mode_High, LOW);
        digitalWrite(device4_Relay_Mode_Medium, LOW);
        digitalWrite(device4_Relay_Mode_LOW, HIGH);
      }
      else if(fan_Speed_Level==2)
      {
        Serial.print("Two");
        digitalWrite(device4_Relay_Mode_Full, LOW);
        digitalWrite(device4_Relay_Mode_High, LOW);
        digitalWrite(device4_Relay_Mode_Medium, HIGH);
        digitalWrite(device4_Relay_Mode_LOW, HIGH);
      }
      else if(fan_Speed_Level==3)
      {
        Serial.print("Three");
        digitalWrite(device4_Relay_Mode_Full, LOW);
        digitalWrite(device4_Relay_Mode_High, HIGH);
        digitalWrite(device4_Relay_Mode_Medium, HIGH);
        digitalWrite(device4_Relay_Mode_LOW, HIGH);
      }
      else if(fan_Speed_Level==4||fan_Speed_Level==5)
      {
        Serial.print("Four Or Five");
        digitalWrite(device4_Relay_Mode_Full, HIGH);
        digitalWrite(device4_Relay_Mode_High, HIGH);
        digitalWrite(device4_Relay_Mode_Medium, HIGH);
        digitalWrite(device4_Relay_Mode_LOW, HIGH);
      }
      
  }
  else
  {
      Serial.print("Off");
        digitalWrite(device4_Relay_Mode_Full, LOW);
        digitalWrite(device4_Relay_Mode_High, LOW);
        digitalWrite(device4_Relay_Mode_Medium, LOW);
        digitalWrite(device4_Relay_Mode_LOW, LOW);
  }
}

void receive_ir_data() {
     
    if (IrReceiver.decode()) {
                Serial.print(F("Decoded protocol: "));
                Serial.print(getProtocolString(IrReceiver.decodedIRData.protocol));
                Serial.print(F(", decoded raw data: "));
        #if (__INT_WIDTH__ < 32)
                Serial.print(" Condition 1 ");
                Serial.print(IrReceiver.decodedIRData.decodedRawData, HEX);
        #else
                Serial.print(" Condition 2 ");
                PrintULL::print(&Serial, IrReceiver.decodedIRData.decodedRawData, HEX);
        #endif
                Serial.print(F(", decoded address: "));
                Serial.print(IrReceiver.decodedIRData.address, HEX);
                Serial.print(F(", decoded command: "));
                Serial.println(IrReceiver.decodedIRData.command, HEX);
               
    
    unsigned long received_code = IrReceiver.decodedIRData.decodedRawData;
    IrReceiver.resume();
    Serial.println(received_code,HEX);
    switch (received_code){

      
      case device1_Button_Code:
        Serial.print("Button 1 Pressed");
        makeNoice();
        device1_currentState = !device1_currentState;
        Serial.printf("Toggle State to %s.\n", device1_currentState ? "true" : "false");
        (device1_currentState == false) ? digitalWrite(device1_Relay, LOW) : digitalWrite(device1_Relay, HIGH);
        device1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, device1_currentState);
        delay(refreshPeriod);
      break;

      
      case device2_Button_Code:
        Serial.print("Button 2 Pressed");
        makeNoice();
        device2_currentState = !device2_currentState;
        Serial.printf("Toggle State to %s.\n", device2_currentState ? "true" : "false");
        (device2_currentState == false) ? digitalWrite(device2_Relay, LOW) : digitalWrite(device2_Relay, HIGH);
        device2.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, device2_currentState);
        delay(refreshPeriod);
      break;

      
      case device3_Button_Code:
        Serial.print("Button 3 Pressed");
        makeNoice();
        device3_currentState = !device3_currentState;
        Serial.printf("Toggle State to %s.\n", device3_currentState ? "true" : "false");
        (device3_currentState == false) ? digitalWrite(device3_Relay, LOW) : digitalWrite(device3_Relay, HIGH);
        device3.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, device3_currentState);
        delay(refreshPeriod);
      break;

      
      case device4_Button_Code:
        Serial.print("Button 4 Pressed");
        makeNoice();
        device4_currentState = !device4_currentState;
        Serial.printf("Toggle State to %s.\n", device4_currentState ? "true" : "false");
        updateFanSpeed();
        device4.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, device4_currentState);
        delay(refreshPeriod);
      break;



      case device5_Button_Code:
        Serial.print("Button 5 Pressed");
        makeNoice();
        device5_currentState = !device5_currentState;
        Serial.printf("Toggle State to %s.\n", device5_currentState ? "true" : "false");
        (device5_currentState == false) ? sendRemoteSignalAC(LOW) : sendRemoteSignalAC(HIGH);
        device5.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, device5_currentState);
        delay(refreshPeriod);
      break;


      
      case deviceUp_Code:
        Serial.print("Button Up Pressed");
        makeNoice();
        if(fan_Speed_Level<4)
        {
          fan_Speed_Level++;
        }
        Serial.printf("Increasing Fan Speed! ");
        Serial.print(fan_Speed_Level);
        updateFanSpeed();
        device4.updateAndReportParam("Speed", fan_Speed_Level);
        delay(refreshPeriod);
      break;

      
      case deviceDown_Code:
        Serial.print("Button Dlown Pressed");
        makeNoice();
        if(fan_Speed_Level>1)
        {
          fan_Speed_Level--;
        }
        Serial.printf("Increasing Fan Speed! ");
        Serial.print(fan_Speed_Level);
        updateFanSpeed();
        device4.updateAndReportParam("Speed", fan_Speed_Level);
        delay(refreshPeriod);
      break;
   
    }
      
    }


}

void sendRemoteSignalAC(boolean state)
{
  if(state == LOW)
  {
    IrSender.sendRaw(device5_AC_OFF, sizeof(device5_AC_OFF) / sizeof(device5_AC_OFF[0]), khz);
    Serial.printf("AC Off");
  }
  else
  {
    IrSender.sendRaw(device5_AC_ON, sizeof(device5_AC_ON) / sizeof(device5_AC_ON[0]), khz);
    Serial.printf("AC On");
  }
}

void makeNoice()
{
  analogWrite(buzzer,pitch);
  delay(buzzerDuration);
  analogWrite(buzzer,0);
}

void readSensorReadings()
{
  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity
  Serial.print("Temperature : ");
  Serial.print(Temperature);
  Serial.print(" Humidity : ");
  Serial.println(Humidity);

  temperature.updateAndReportParam("Temperature", Temperature);
  humidity.updateAndReportParam("Temperature", Humidity);
}

void setup()
{
  startMillis = millis();
  Serial.begin(115200);

  IrReceiver.begin(IR_Rev);
  IrSender.begin(IR_Send);

  
  dht.begin();
  
  pinMode(device1_Relay,                OUTPUT);
  pinMode(device2_Relay,                OUTPUT);
  pinMode(device3_Relay,                OUTPUT);
  pinMode(device4_Relay_Mode_Full,      OUTPUT);
  pinMode(device4_Relay_Mode_High,      OUTPUT);
  pinMode(device4_Relay_Mode_Medium,    OUTPUT);
  pinMode(device4_Relay_Mode_LOW,       OUTPUT);
  pinMode(buzzer,                       OUTPUT);

  digitalWrite(device1_Relay,               DEFAULT_POWER_MODE);
  digitalWrite(device2_Relay,               DEFAULT_POWER_MODE);
  digitalWrite(device3_Relay,               DEFAULT_POWER_MODE);
  digitalWrite(device4_Relay_Mode_Full,     DEFAULT_POWER_MODE);
  digitalWrite(device4_Relay_Mode_High,     DEFAULT_POWER_MODE);
  digitalWrite(device4_Relay_Mode_Medium,   DEFAULT_POWER_MODE);
  digitalWrite(device4_Relay_Mode_LOW,      DEFAULT_POWER_MODE);

  

  pinMode(device1_Switch, INPUT);
  pinMode(device2_Switch, INPUT);
  pinMode(device3_Switch, INPUT);
  pinMode(device4_Switch, INPUT);
  pinMode(device4_UP, INPUT);
  pinMode(device4_DOWN, INPUT);

  Node my_node;
  my_node = RMaker.initNode("Bedroom Node");

  my_node.addDevice(device1);
  my_node.addDevice(device2);
  my_node.addDevice(device3);
  my_node.addDevice(device4);
  my_node.addDevice(device5);
  my_node.addDevice(temperature);
  my_node.addDevice(humidity);

  device1.addCb(write_callback);
  device2.addCb(write_callback);
  device3.addCb(write_callback);
  device4.addCb(write_callback);
  device4.addSpeedParam(3,ESP_RMAKER_DEF_SPEED_NAME);
  device5.addCb(write_callback);



  device1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, device1_currentState);
  device2.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, device2_currentState);
  device3.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, device3_currentState);
  device4.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, device4_currentState);
  device4.updateAndReportParam("Speed", fan_Speed_Level);
  device5.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, device4_currentState);
 
  readSensorReadings();
  //This is optional
  RMaker.enableOTA(OTA_USING_TOPICS);
  //If you want to enable scheduling, set time zone for your region using setTimeZone().
  //The list of available values are provided here https://rainmaker.espressif.com/docs/time-service.html
  // RMaker.setTimeZone("Asia/Shanghai");
  // Alternatively, enable the Timezone service and let the phone apps set the appropriate timezone
  RMaker.enableTZService();

  RMaker.enableSchedule();

  RMaker.enableScenes();

  RMaker.start();

  WiFi.onEvent(sysProvEvent);
#if CONFIG_IDF_TARGET_ESP32S2
  WiFiProv.beginProvision(WIFI_PROV_SCHEME_SOFTAP, WIFI_PROV_SCHEME_HANDLER_NONE, WIFI_PROV_SECURITY_1, pop, service_name);
#else
  WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, WIFI_PROV_SECURITY_1, pop, service_name);
#endif
}
void loop()
{
currentMillis = millis();
  receive_ir_data();

if (currentMillis - startMillis >= period)
{
  readSensorReadings();
  startMillis = millis();  //initial start time
}

//Sensor input
if(digitalRead(device1_Switch)==1)
{
  delay(debouncePeriod);
  if(digitalRead(device1_Switch)==1)
  {
    
    device1_currentState = !device1_currentState;
    Serial.printf("Toggle State to %s.\n", device1_currentState ? "true" : "false");
    (device1_currentState == false) ? digitalWrite(device1_Relay, LOW) : digitalWrite(device1_Relay, HIGH);
    device1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, device1_currentState);
    makeNoice();
    delay(refreshPeriod);
  }
}

if(digitalRead(device2_Switch)==1)
{
  delay(debouncePeriod);
  if(digitalRead(device2_Switch)==1)
  {
    
    device2_currentState = !device2_currentState;
    Serial.printf("Toggle State to %s.\n", device2_currentState ? "true" : "false");
    (device2_currentState == false) ? digitalWrite(device2_Relay, LOW) : digitalWrite(device2_Relay, HIGH);
    device2.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, device2_currentState);
    makeNoice();
    delay(refreshPeriod);
  }
}

if(digitalRead(device3_Switch)==1)
{
  delay(debouncePeriod);
  if(digitalRead(device3_Switch)==1)
  {
    
    device3_currentState = !device3_currentState;
    Serial.printf("Toggle State to %s.\n", device3_currentState ? "true" : "false");
    (device3_currentState == false) ? digitalWrite(device3_Relay, LOW) : digitalWrite(device3_Relay, HIGH);
    device3.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, device3_currentState);
    makeNoice();
    delay(refreshPeriod);
  }
}

if(digitalRead(device4_Switch)==1)
{
  delay(debouncePeriod);
  if(digitalRead(device4_Switch)==1)
  {
    
    device4_currentState = !device4_currentState;
    Serial.printf("Toggle State to %s.\n", device4_currentState ? "true" : "false");
    device4.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, device4_currentState);
    updateFanSpeed();
    makeNoice();
    delay(refreshPeriod);   
  }
}

if(digitalRead(device4_UP)==1)
{
  delay(debouncePeriod);
  if(digitalRead(device4_UP)==1)
  {
    
    Serial.println("Increase Fan Speed");
    if(fan_Speed_Level<4)
    {
      fan_Speed_Level++;
      updateFanSpeed();
      device4.updateAndReportParam("Speed", fan_Speed_Level);
    }
    makeNoice();
    delay(refreshPeriod);   
  }
}

if(digitalRead(device4_DOWN)==1)
{
  delay(debouncePeriod);
  if(digitalRead(device4_DOWN)==1)
  {
    
    Serial.println("Decrease Fan Speed");
    if(fan_Speed_Level>0)
    {
      fan_Speed_Level--;
      updateFanSpeed();
      device4.updateAndReportParam("Speed", fan_Speed_Level);
    }
    makeNoice();
    delay(refreshPeriod);   
  }
}
  if (digitalRead(gpio_0) == LOW) { //Push button pressed

    // Key debounce handling
    delay(100);
    int startTime = millis();
    while (digitalRead(gpio_0) == LOW) delay(50);
    int endTime = millis();

    if ((endTime - startTime) > 10000) {
      // If key pressed for more than 10secs, reset all
      Serial.printf("Reset to factory.\n");
      RMakerFactoryReset(2);
    } else if ((endTime - startTime) > 3000) {
      Serial.printf("Reset Wi-Fi.\n");
      // If key pressed for more than 3secs, but less than 10, reset Wi-Fi
      RMakerWiFiReset(2);
    } else {

      // Toggle device state
      device1_currentState = !device1_currentState;
      Serial.printf("Toggle State to %s.\n", device1_currentState ? "true" : "false");
      device1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, device1_currentState);
      (device1_currentState == false) ? digitalWrite(device1_Relay, LOW) : digitalWrite(device1_Relay, HIGH);
    }
  }

  if(Serial.available() > 0) {
    incommingData = Serial.readStringUntil('\n');
    Serial.println(incommingData);
    if(incommingData=="1")
    {
      Serial.println("Reset Wifi");
      RMakerWiFiReset(2);
    }
    else if(incommingData=="2")
    {
      Serial.println("Reset Board");
      RMakerFactoryReset(2);
    }
  }
  delay(100);
}
