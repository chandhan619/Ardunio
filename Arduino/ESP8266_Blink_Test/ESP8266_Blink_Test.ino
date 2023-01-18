
#define RELAY0 0
#define RELAY1 2


void setup() {
  pinMode(RELAY0, OUTPUT);
  pinMode(RELAY1, OUTPUT);


}

void loop() {
  
          digitalWrite(RELAY0, HIGH);
          digitalWrite(RELAY1, HIGH);

          delay(1000);
          digitalWrite(RELAY0, LOW);
          digitalWrite(RELAY1, LOW);

          delay(1000);
}
