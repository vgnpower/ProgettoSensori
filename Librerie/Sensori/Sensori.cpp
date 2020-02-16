#include "Arduino.h"
#include "DHT.h"
#include "DPC.h"
#include"AirQuality.h" //usato da airquality
#include"Arduino.h" //Usato da AirQuality
#include <SoftwareSerial.h> //Usato da MHZ16
#include <MutichannelGasSensor.h> //Usato da MultiChannelGasSensor
#include <Wire.h> //Usato da MultiChannelGasSensor
#include "MutichannelGasSensor.h" //Usato da MultiChannelGasSensor
#include "Sensori.h"
//#include "SMPWM01A.h"

//Forse mq4 e mq3 la divisione di R0 con 60 può essere sbagliata?
//nome sensore + oututs
Sensori::Sensori()
{
  /*
  Serial.begin(9600);
  // sensore MG811
  pinMode(BOOL_PIN, INPUT);                        //set pin to input
  digitalWrite(BOOL_PIN, HIGH);                    //turn on pullup resistors
  Serial.print("MG-811 Demostration\n");
  //Fine sensore MG811
  //Inizio sensore AirQualitySensor
  //airqualitysensor.init(14);
  //Fine AirQualitySensor
  //Inizio sensore MHZ16
  sensor.begin(9600);
  //Fine sensore MHZ16
  //Inizio MultiChannelGasSensor
  gas.begin(0x04);//the default I2C address of the slave is 0x04
  gas.powerOn();
  //Fine MultiChanelGasSensor
  //Inizio Sensore PPD42NS
  pinMode(8,INPUT);
  //Fine Sensore PPD42NS
  //Inizio Sensore SMPWM01A
  //dust.begin();
 //Fine sensore SMPWM01A*/
}

void Sensori::AM2302_SetUp(int _pin)
{
   dht.setup(_pin);
}

float Sensori::AM2302_Temp()
{
  return dht.getTemperature();
}

float Sensori::AM2302_Hum()
{
  return dht.getHumidity();
}

//sensore _pin, mq
float Sensori::MQ_Volt(int _pin)
{
  float mq_volt, mqValue;
  mqValue = analogRead(_pin);
  mq_volt = mqValue/1024*5.0;
   /*--- Get a average data by testing 100 times ---*/
   for(int x = 0 ; x < 100 ; x++)
   {
       mqValue = mqValue + analogRead(_pin);
   }
   mqValue = mqValue/100.0;
   /*-----------------------------------------------*/
   return mq_volt = mqValue/1024*5.0;
}

float Sensori::MQ_R0(int _pin, float _const)
{
     float mq_volt, RS_air, R0, mqValue;
     mqValue = analogRead(_pin);
     mq_volt = mqValue/1024*5.0;
      /*--- Get a average data by testing 100 times ---*/
      for(int x = 0 ; x < 100 ; x++)
      {
          mqValue = mqValue + analogRead(_pin);
      }
      mqValue = mqValue/100.0;
      /*-----------------------------------------------*/

      mq_volt = mqValue/1024*5.0;
      RS_air = (5.0-mq_volt)/mq_volt; // omit *RL
      return R0 = RS_air/_const; // The ratio of RS/R0 is 9.8 in a clear air from Graph (Found using WebPlotDigitizer)
}

float Sensori::MQ_RS(int _pin)
{
     float mq_volt, RS_air, mqValue;
     mqValue = analogRead(_pin);
     mq_volt = mqValue/1024*5.0;
      /*--- Get a average data by testing 100 times ---*/
      for(int x = 0 ; x < 100 ; x++)
      {
          mqValue = mqValue + analogRead(_pin);
      }
      mqValue = mqValue/100.0;
      /*-----------------------------------------------*/

      mq_volt = mqValue/1024*5.0;
      return RS_air = (5.0-mq_volt)/mq_volt; // omit *RL
}

float Sensori::MQ_Ratio(int _pin, float _const)
{
     float mq_volt, RS_air, R0, mqValue, rapp;
     mqValue = analogRead(_pin);
     mq_volt = mqValue/1024*5.0;
      /*--- Get a average data by testing 100 times ---*/
      for(int x = 0 ; x < 100 ; x++)
      {
          mqValue = mqValue + analogRead(_pin);
      }
      mqValue = mqValue/100.0;
      /*-----------------------------------------------*/

      mq_volt = mqValue/1024*5.0;
      RS_air = (5.0-mq_volt)/mq_volt; // omit *RL
      R0 = _const; // The ratio of RS/R0 is 9.8 in a clear air from Graph (Found using WebPlotDigitizer)
      return rapp = RS_air/R0;
}
//Inizio Sensore MG811
float Sensori::MG811_Volts()
{
  volts=MGRead(MG_PIN);
  return volts;
}
int Sensori::MG811_CO2()
{
  int percentage;
  percentage = MGGetPercentage(volts,CO2Curve);
  return percentage;
}

float Sensori::MGRead(int mg_pin)
{
    int i;
    float v=0;

    for (i=0;i<READ_SAMPLE_TIMES;i++) {
        v += analogRead(mg_pin);
        delay(READ_SAMPLE_INTERVAL);
    }
    v = (v/READ_SAMPLE_TIMES) *5/1024 ;
    return v;
}

int  Sensori::MGGetPercentage(float volts, float *pcurve)
{
   if ((volts/DC_GAIN )>=ZERO_POINT_VOLTAGE) {
      return -1;
   } else {
      return pow(10, ((volts/DC_GAIN)-pcurve[1])/pcurve[2]+pcurve[0]);
   }
}
//Fine Sensore MG811

//Inizio Sensore HCH0
float Sensori::HCH0_R0()
{
  int sensorValue=analogRead(A0);
  float R0=(1023.0/sensorValue)-1;
  return R0;
}
//Fine Sensore HCH0

//Inizio metodi Air QualitySensor
void Sensori::AirQualitySensor_Setup(int _pin)
{//pin 14
  airqualitysensor.init(_pin);
}
void Sensori::AirQualitySensor(){
    current_quality=airqualitysensor.slope();
    if (current_quality >= 0)// if a valid data returned.
    {
        if (current_quality==0)
            Serial.println("High pollution! Force signal active");
        else if (current_quality==1)
            Serial.println("High pollution!");
        else if (current_quality==2)
            Serial.println("Low pollution!");
        else if (current_quality ==3)
            Serial.println("Fresh air");
    }
}
//Fine metodi Air QualitySensor

//Inizio Sensore MHZ16
int Sensori::MHZ16_Temperature()
{
  if(dataRecieve())
  {
    return temperature;
  }
}

int Sensori::MHZ16_CO2()
{
  if(dataRecieve())
  {
    return CO2PPM;
  }
}

bool Sensori::dataRecieve(void)
{
    byte data[9];
    int i = 0;

    //transmit command data
    for(i=0; i<sizeof(cmd_get_sensor); i++)
    {
        sensor.write(cmd_get_sensor[i]);
    }
    delay(10);
    //begin reveiceing data
    if(sensor.available())
    {
        while(sensor.available())
        {
            for(int i=0;i<9; i++)
            {
                data[i] = sensor.read();
            }
        }
    }

    for(int j=0; j<9; j++)
    {
      return data[j];
    }

    if((i != 9) || (1 + (0xFF ^ (byte)(data[1] + data[2] + data[3] + data[4] + data[5] + data[6] + data[7]))) != data[8])
    {
        return false;
    }

    CO2PPM = (int)data[2] * 256 + (int)data[3];
    temperature = (int)data[4] - 40;

    return true;
}
//Fine Sensore MHZ16

//Inizio Sensore MultiChannelGasSensor
void Sensori::MultiChannelGasSensor_SetUp(){
  objMultiChannelGasSensor = MutichannelGasSensor();
  objMultiChannelGasSensor.begin(0x04);
  objMultiChannelGasSensor.powerOn();
}

float Sensori::MultiChannelGasSensor_Ratio1()
{
	ratio1MultiChannel = objMultiChannelGasSensor.ratio1_pub;
	return objMultiChannelGasSensor.ratio1_pub;
}

float Sensori::MultiChannelGasSensor_Ratio2()
{
	ratio2MultiChannel = objMultiChannelGasSensor.ratio2_pub;
	return objMultiChannelGasSensor.ratio2_pub;
}

float Sensori::MultiChannelGasSensor_Ratio3()
{
	ratio3MultiChannel = objMultiChannelGasSensor.ratio3_pub;
	return objMultiChannelGasSensor.ratio3_pub;
}

float Sensori::MultiChannelGasSensor_NH3()
{
  float c;
  c=objMultiChannelGasSensor.measure_NH3();
  if(c>=0) return c;
  else return -1;
}
float Sensori::MultiChannelGasSensor_CO()
{
  float c;
  c=objMultiChannelGasSensor.measure_CO();
  if(c>=0) return c;
  else return -1;
}
float Sensori::MultiChannelGasSensor_NO2()
{
  float c;
  c=objMultiChannelGasSensor.measure_NO2();
  if(c>=0) return c;
  else return -1;
}
float Sensori::MultiChannelGasSensor_C3H8()
{
  float c;
  c=objMultiChannelGasSensor.measure_C3H8();
  if(c>=0) return c;
  else return -1;
}
float Sensori::MultiChannelGasSensor_C4H10()
{
  float c;
  c=objMultiChannelGasSensor.measure_C4H10();
  if(c>=0) return c;
  else return -1;
}
float Sensori::MultiChannelGasSensor_CH4()
{
  float c;
  c=objMultiChannelGasSensor.measure_CH4();
  if(c>=0) return c;
  else return -1;
}
float Sensori::MultiChannelGasSensor_H2()
{
  float c;
  c=objMultiChannelGasSensor.measure_H2();
  if(c>=0) return c;
  else return -1;
}
float Sensori::MultiChannelGasSensor_C2H5OH()
{
  float c;
  c=objMultiChannelGasSensor.measure_C2H5OH();
  if(c>=0) return c;
  else return -1;
}
//Fine Sensore MultiChannelGasSensor

//Inizio Sensore PPD42NS
void Sensori::PPD42NS_SetUp(int _pin){
  //lavora sul pin 8
  duration = pulseIn(_pin, LOW);
}
unsigned long Sensori::PPD42NS_Time(int _pin)
{
  duration = pulseIn(_pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
  return lowpulseoccupancy;
}
float Sensori::PPD42NS_Concentration()
{
  if ((millis()-starttime) >= sampletime_ms)
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62;
  }
    lowpulseoccupancy = 0;//Stava sotto il return
  starttime = millis();//Stava sotto il return
  return concentration;
}

unsigned long Sensori::PPD42NS_Duty_Cycle()
{
  unsigned long duty;
  duty = lowpulseoccupancy/sampletime_ms;
  return duty;
}
//Fine Sensore PPD42NS

//Inizio Sensore SMPW01A 
/*Non si riesce a gestire il vettore degli interrupt
float Sensori::SMPWM01A_PM2()
{
  return dust.getPM2();
}
float Sensori::SMPWM01A_PM10()
{
  return dust.getPM10();
}
unsigned long int Sensori::SMPWM01A_PM02VAL()
{
  return lpoPM02;
}
float Sensori::SMPWM01A_PM02CONC()
{
  return concPM02;
}*/
//Fine Sensore SMPWM01A
