//This example demonstrates the ESP RainMaker with a standard Switch device.
#include "RMaker.h"
#include "WiFi.h"
#include "WiFiProv.h"

#define DEFAULT_POWER_MODE false
const char *service_name = "Hall Board";
const char *pop = "ajb1132e3";

//GPIO for push button
#if CONFIG_IDF_TARGET_ESP32C3
static int gpio_0 = 9;
static int gpio_switch = 7;
#else
//GPIO for virtual device
static int gpio_0 = 0;
#endif

  //Signal Out / Relays
  static int light1 = 12;
  static int light2 = 14;
  static int light3 = 27;
  static int light4 = 26;
  //static int fan = 16; //(3)
  //static int TV = 16;
  //static int displayBackLight = 16;
  //static int socketSwitch = 16;
  //static int buzzer = 16;

  
  //Signal in / Sensors
  static int light1_Input = 35;
  static int light2_Input = 34;
  static int light3_Input = 39;
  static int light4_Input = 36;

  
  //Input Mode 0-Not in use , 1-Manual , 2-Touch Sensor 
  static int light1_Mode = 1;


/* Variable for reading pin status*/
bool switch_state = true;

bool light1_state = false;

//The framework provides some standard device types like switch, lightbulb, fan, temperaturesensor.
static Switch light1_Switch;

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
    const char *device_name = device->getDeviceName();
    const char *param_name = param->getParamName();

if(strcmp(device_name,"Light 1") == 0)
{
    if(strcmp(param_name, "Power") == 0) {
        Serial.printf("Received value = %s for %s - %s\n", val.val.b? "true" : "false", device_name, param_name);
        light1_state = val.val.b;
        (light1_state == false) ? digitalWrite(light1, LOW) : digitalWrite(light1, HIGH);
        param->updateAndReport(val);
    }
}
}

void setup()
{
    Serial.begin(115200);
    pinMode(gpio_0, INPUT);

    // Switch 1 state
    pinMode(light1, OUTPUT);
    pinMode(light1_Input, INPUT);
    digitalWrite(light1, DEFAULT_POWER_MODE);

    
    Node my_node;
    my_node = RMaker.initNode("Hall Node");

    
    light1_Switch = Switch("Light 1", &light1);
    light1_Switch.addCb(write_callback);
    
    
    
    
    my_node.addDevice(light1_Switch);




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

  if(light1_Mode==1)  // Touch Senssor
  {
      if(digitalRead(light1_Input)==HIGH) { //Push button pressed

        // Key debounce handling
        delay(100);
      if(digitalRead(light1_Input)==HIGH ){
                  // Toggle device state
          light1_state = !light1_state;
          Serial.printf("Toggle State to %s.\n", light1_state ? "true" : "false");
          light1_Switch.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, light1_state);
          (light1_state == false) ? digitalWrite(light1, LOW) : digitalWrite(light1, HIGH);
      }
      }
      delay(1000);
  }
  else if(light1_Mode == 2)
  {
    if(light1_state!=digitalRead(light1_Input)) { //Push button pressed

        // Key debounce handling
        delay(100);
        
      if(digitalRead(light1_Input)==LOW ){
                  // Toggle device state
          light1_state = LOW;
          Serial.printf("Toggle State to %s.\n", light1_state ? "true" : "false");
          light1_Switch.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, light1_state);
          (light1_state == false) ? digitalWrite(light1, LOW) : digitalWrite(light1, HIGH);
      }
      else if(digitalRead(light1_Input)==HIGH ){
                  // Toggle device state
          light1_state = HIGH;
          Serial.printf("Toggle State to %s.\n", light1_state ? "true" : "false");
          light1_Switch.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, light1_state);
          (light1_state == false) ? digitalWrite(light1, LOW) : digitalWrite(light1, HIGH);
      }
  }




  
    if(digitalRead(gpio_0) == LOW) { //Push button pressed

        // Key debounce handling
        delay(100);

        
        int startTime = millis();
        while(digitalRead(gpio_0) == LOW) delay(50);
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
          
      }
    }
    delay(100);
}
  }
