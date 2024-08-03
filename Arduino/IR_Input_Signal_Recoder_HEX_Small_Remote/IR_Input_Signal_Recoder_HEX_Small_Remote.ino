#include <IRremote.hpp>
#define IR_Rev 15

void setup()
{
  IrReceiver.begin(IR_Rev);
  Serial.begin(115200);
  
}

void loop()
{
  receive_ir_data();
  //Serial.println("Reading IR Data");
  delay(100);
}

void receive_ir_data() {
    if (IrReceiver.decode()) {
        Serial.print(F("Decoded protocol: "));
        Serial.print(getProtocolString(IrReceiver.decodedIRData.protocol));
        Serial.print(F(", decoded raw data: "));
#if (__INT_WIDTH__ < 32)
        Serial.print(IrReceiver.decodedIRData.decodedRawData, HEX);
#else
        PrintULL::print(&Serial, IrReceiver.decodedIRData.decodedRawData, HEX);
#endif
        Serial.print(F(", decoded address: "));
        Serial.print(IrReceiver.decodedIRData.address, HEX);
        Serial.print(F(", decoded command: "));
        Serial.println(IrReceiver.decodedIRData.command, HEX);
        IrReceiver.resume();
    }
}
