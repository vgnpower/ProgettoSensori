#include <Dpc.h>
Dpc h;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  h.setupSens(5,2);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  Serial.println(h.calcDPcl1(5,2));
  delay(2000);
}
