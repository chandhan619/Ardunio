/* rawSend.ino Example sketch for IRLib2
 *  Illustrates how to send a code Using raw timings which were captured
 *  from the "rawRecv.ino" sample sketch.  Load that sketch and
 *  capture the values. They will print in the serial monitor. Then you
 *  cut and paste that output into the appropriate section below.
 */
#include <IRLibSendBase.h>    //We need the base code
#include <IRLib_HashRaw.h>    //Only use raw sender
#include <dht.h>

#define dht_apin D0 // Analog Pin sensor is connected to
 
dht DHT;

IRsendRaw mySender;

void setup() {
  Serial.begin(9600);
  delay(2000); 
  while (!Serial); //delay for Leonardo
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
}
/* Cut and paste the output from "rawRecv.ino" below here. It will 
 * consist of a #define RAW_DATA_LEN statement and an array definition
 * beginning with "uint16_t rawData[RAW_DATA_LEN]= {…" and concludes
 * with "…,1000};"
 */
#define RAW_DATA_LEN 198
uint16_t rawDataOff[RAW_DATA_LEN]={
  6046, 7398, 522, 1698, 470, 1682, 494, 1678, 
  498, 1650, 446, 1726, 498, 1650, 490, 1682, 
  494, 1678, 498, 638, 474, 614, 470, 622, 
  498, 614, 470, 614, 470, 626, 494, 614, 
  470, 618, 442, 1734, 498, 1658, 514, 1654, 
  466, 1710, 494, 1674, 442, 1710, 498, 1650, 
  466, 1710, 494, 642, 466, 626, 494, 618, 
  474, 614, 442, 650, 494, 594, 494, 618, 
  442, 646, 498, 1678, 470, 1686, 518, 1654, 
  466, 1686, 518, 1654, 466, 1690, 514, 1654, 
  466, 1690, 514, 642, 446, 642, 502, 594, 
  494, 614, 474, 618, 442, 650, 498, 590, 
  494, 618, 446, 1714, 518, 1654, 462, 1690, 
  506, 606, 494, 1654, 494, 1686, 494, 594, 
  470, 1682, 518, 622, 494, 594, 466, 626, 
  494, 1682, 494, 594, 466, 626, 494, 1682, 
  470, 618, 466, 1738, 494, 1658, 466, 626, 
  494, 1678, 470, 1682, 494, 602, 514, 1658, 
  466, 1686, 494, 646, 494, 594, 466, 1694, 
  486, 622, 466, 622, 466, 1690, 518, 594, 
  494, 598, 466, 654, 490, 1682, 470, 622, 
  466, 1690, 514, 598, 462, 1710, 446, 646, 
  470, 622, 466, 1734, 442, 650, 470, 1706, 
  442, 646, 442, 1714, 466, 646, 446, 1706, 
  466, 1690, 466, 7466, 438, 1000};

uint16_t rawDataOn[RAW_DATA_LEN]={
  6010, 7450, 494, 1738, 442, 1706, 502, 1646, 
  458, 1710, 502, 1670, 470, 1682, 498, 1674, 
  498, 1674, 442, 674, 498, 614, 474, 614, 
  438, 654, 498, 614, 474, 610, 418, 674, 
  502, 586, 498, 1678, 526, 1670, 478, 1674, 
  498, 1654, 462, 1710, 502, 1646, 438, 1738, 
  498, 1650, 438, 702, 502, 610, 478, 610, 
  442, 650, 502, 586, 502, 614, 414, 674, 
  498, 594, 530, 1646, 466, 1706, 506, 1646, 
  518, 1654, 502, 1654, 490, 1682, 502, 1646, 
  490, 1686, 502, 638, 394, 698, 502, 590, 
  498, 614, 474, 614, 442, 650, 534, 554, 
  494, 622, 418, 1762, 534, 578, 506, 1622, 
  522, 590, 506, 1646, 438, 1718, 554, 582, 
  474, 1650, 490, 654, 494, 1654, 442, 674, 
  506, 1642, 506, 610, 418, 674, 506, 1642, 
  498, 598, 414, 1770, 550, 1614, 478, 618, 
  490, 1682, 474, 1678, 470, 626, 522, 1650, 
  470, 1682, 494, 646, 502, 586, 418, 1738, 
  534, 578, 502, 590, 466, 1686, 526, 570, 
  518, 590, 510, 606, 470, 1706, 506, 586, 
  470, 1686, 518, 594, 466, 1686, 466, 622, 
  466, 630, 490, 1710, 470, 622, 522, 1654, 
  498, 594, 466, 1686, 490, 622, 498, 1658, 
  462, 1694, 510, 7442, 470, 1000};

/*
 * Cut-and-paste into the area above.
 */
   
void loop() {
  
    mySender.send(rawDataOn,RAW_DATA_LEN,36);//Pass the buffer,length, optionally frequency
    Serial.println(F("AC Switched On"));
 delay(5000);
    mySender.send(rawDataOff,RAW_DATA_LEN,36);//Pass the buffer,length, optionally frequency
    Serial.println(F("AC Switched Off"));
  
  delay(5000);
}
