#include <Wire.h>

#include <MutichannelGasSensor.h>

MutichannelGasSensor gasSensor = MutichannelGasSensor();
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  gasSensor.begin(0x04);
  gasSensor.powerOn();
}

void loop() {
  // put your main code here, to run repeatedly:
  gasSensor.measure_NH3();
   Serial.println("Ratio1: ");
  Serial.println(gasSensor.ratio1_pub);
      Serial.println("Ratio2: ");
  Serial.println(gasSensor.ratio2_pub);
      Serial.println("Ratio3: ");
  Serial.println(gasSensor.ratio3_pub);
}
