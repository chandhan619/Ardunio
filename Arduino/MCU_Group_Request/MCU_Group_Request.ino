
// C++ code
//

char input[] = "+CMT: \"+234xxxxxxxxxx\",,\"2021/08/23,15:50:39+01\" My first text for embedded system project";
char test[] = "abc,egf";

void setup() {
  Serial.begin(9600);
 // Serial.println(input);
  Serial.println(getValue(input,'"',1));
  Serial.println(getValue(input,'"',4));
}

void loop() {
  

}



String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
