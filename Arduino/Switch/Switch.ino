
#define SWITCH1 D1  //GPIO5  //D1
#define SWITCH2 D2  //GPIO4  //D2
#define SWITCH3 D6  //GPIO9 //SDD3
#define SWITCH4 D8  //GPIO10  //SDD2


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  Serial.print(digitalRead(SWITCH1));
  Serial.print(digitalRead(SWITCH2));
  Serial.print(digitalRead(SWITCH3));
  Serial.println(digitalRead(SWITCH4));
  
}
