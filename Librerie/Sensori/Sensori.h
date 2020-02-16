#ifndef Sensori_h
#define Sensori_h
#include "Arduino.h"
#include "DHT.h"
#include "DPC.h"
#include"AirQuality.h" //usato da airquality
#include"Arduino.h" //Usato da AirQuality
#include <SoftwareSerial.h> //Usato da MHZ16
#include <MutichannelGasSensor.h> //Usato da MultiChannelGasSensor
#include <Wire.h> //Usato da MultiChannelGasSensor
//#include "SMPWM01A.h"

//Inizio MG811
/************************Hardware Related Macros************************************/
#define         MG_PIN                       (9)     //define which analog input channel you are going to use
#define         BOOL_PIN                     (2)
#define         DC_GAIN                      (8.5)   //define the DC gain of amplifier

/***********************Software Related Macros************************************/
#define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (5)     //define the time interval(in milisecond) between each samples in
                                                     //normal operation

/**********************Application Related Macros**********************************/
//These two values differ from sensor to sensor. user should derermine this value.
#define         ZERO_POINT_VOLTAGE           (0.324) //define the output of the sensor in volts when the concentration of CO2 is 400PPM
#define         REACTION_VOLTGAE             (0.059) //define the voltage drop of the sensor when move the sensor from air into 1000ppm CO2
//#define         ZERO_POINT_X                 (2.602) //lg400=2.602, the start point_on X_axis of the curve
//#define         ZERO_POINT_VOLTAGE           (0.324) //define the output of the sensor in volts when the concentration of CO2 is 400PPM
//#define         MAX_POINT_VOLTAGE            (0.265) //define the output of the sensor in volts when the concentration of CO2 is 10,000PPM
//#define         REACTION_VOLTGAE             (0.059) //define the voltage drop of the sensor when move the sensor from air into 1000ppm CO2
//Fine MG811
#define Vc 4.95//HCHH0
#define sensor Serial1 //MHZ16

class Sensori
{
  public:
    Sensori();
    float AM2302_Temp(),
	AM2302_Hum(),
	MQ_Volt(int _pin),
	MQ_R0(int _pin, float _const),
	MQ_RS(int _pin),
	MQ_Ratio(int _pin, float _const);
    void AM2302_SetUp(int _pin),
    MultiChannelGasSensor_SetUp(),
    PPD42NS_SetUp(int _pin),
    AirQualitySensor_Setup(int _pin);
    float MG811_Volts(),
	MGRead();
	int MG811_CO2(),
	MGGetPercentage(float volts, float *pcurve);
    float HCH0_R0();
    void AirQualitySensor();
    int MHZ16_Temperature(),
    MHZ16_CO2();
    float MultiChannelGasSensor_NH3(),
    MultiChannelGasSensor_CO(),
    MultiChannelGasSensor_NO2(),
    MultiChannelGasSensor_C3H8(),
    MultiChannelGasSensor_C4H10(),
    MultiChannelGasSensor_CH4(),
    MultiChannelGasSensor_H2(),
    MultiChannelGasSensor_C2H5OH(),
	SMPWM01A_PM2(),
    SMPWM01A_PM10(),
    SMPWM01A_PM02CONC(),
	PPD42NS_Concentration();
    unsigned long PPD42NS_Time(int _pin),
	PPD42NS_Duty_Cycle();

      //Inizio AirQualitySensor
      AirQuality airqualitysensor;
      int current_quality =-1;
      //Fine AirQualitySensor
	  //SMPWM01A
      /*unsigned long lpoPM02;//dovevamo lege xke era public
      float concPM02;
      SMPWM01A dust;
	  unsigned long SMPWM01A_PM02VAL();*/
	  //end SMPWM01A
	  
	  //Inizio multichannelgasSensor(Sono le 3 varibile rese pubbliche sulla sua libreria)
	  float ratio1MultiChannel,ratio2MultiChannel,ratio3MultiChannel;
	  float MultiChannelGasSensor_Ratio1(), 
	  MultiChannelGasSensor_Ratio2(), 
	  MultiChannelGasSensor_Ratio3();
	  //FINe multichannel
  private:
    float CO2Curve[3] = {2.602,ZERO_POINT_VOLTAGE,(REACTION_VOLTGAE/(2.602-4))}; //MG811
    //Inizio MHZ16
    const unsigned char cmd_get_sensor[9] =
    {
        0xff, 0x01, 0x86, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x79
    };

    unsigned char dataRevice[9];
    int temperature;
    int CO2PPM;
    //Fine MHZ16
    //Inizio PPD42NS
    int pin = 8;
    unsigned long duration;
    unsigned long starttime;
    unsigned long sampletime_ms = 2000;//sampe 30s&nbsp;;
    unsigned long lowpulseoccupancy = 0;
    float ratio = 0;
    float concentration = 0;
    //Fine PPD42NS
    float MGRead(int mg_pin);
    bool dataRecieve(void);
    DHT dht;//Inizializzazione oggetto AM2302 di DHT
	//Inizio MG811
	 float volts;
	//Fine MG811
	//Multichannel
	MutichannelGasSensor objMultiChannelGasSensor;
	//Multichannel
};

#endif
