#include <Azioni.h>
#include <Dpc.h>
#include <Sensori.h>

Azioni ObjAzioni = Azioni();
Sensori ObjSensoreInterno = Sensori();//Oggetto della classe sensori in questo caso creo un istanza per quelli interni
Sensori ObjSensoreEsterno = Sensori();//Oggetto della classe sensori in questo caso creao una instanza per quelli estenri

/*
const String NomeSensoriMq[] = {"MQ2", "MQ3", "MQ4", "MQ5", "MQ6", "MQ7", "MQ8", "MQ9", "MQ135"};
bool sensoriConnessiMqInterni[] = {true, true, true, true, true, true, true, false, true};
int pinMQIterni[] = {0, 3, 4, 2, 6, 7, 8, 1, 5}; //array che indica il pin di connessione di un sensore, -1 corrisponde al sensore non connesso(corrisponderà ad un false nella corrisponde posizione dell array sensoriConnessi.).
float RHInterno[] = { 0.2, 0.1, 0.4, 0.1, 0.5, 0.002, 0.002, 0.2, 0.1}; //valori iniziali forniti da METAS per i sensori interni nel'eventualità vhe il valore non sia presente
bool sensoriConnessiMqEsterni[] = {true, true, true, true, true, true, true, false, true};
int pinMQEsterni[] = {0, 3, 4, 2, 6, 7, 8, 1, 5}; //array che indica il pin di connessione di un sensore, -1 corrisponde al sensore non connesso(corrisponderà ad un false nella corrisponde posizione dell array sensoriConnessi.).
float RHEsterno[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1}; //-1 intende che RH(Esterno) non è ancora calcolato per il sensore corrispondente
const float deltaTmin20[] = {1.017, 1.017, 1.017, 1.017, 1.017, 1.017, 1.017, 1.017, 1.017};
const float deltaTmax20[] = {0.003333, 0.003333, 0.003333, 0.003333, 0.003333, 0.003333, 0.003333, 0.003333, 0.003333};
const float deltaRh[] = {0.00192308, 0.00192308, 0.00192308, 0.00192308, 0.00192308, 0.00192308, 0.00192308, 0.00192308, 0.00192308};
float R0[] = { 4.25, 3.30, 0.33, 5.79, 5.91, 2.35, 0.27, 0.93, 0.44};//Calcolato con lo sketch Calcolo_R0_Sensori_MQ
*/

String datiSensoriMQ[][9] = {
  { "MQ2", "MQ3", "MQ4", "MQ5", "MQ6", "MQ7", "MQ8", "MQ9", "MQ135"},
  { "True", "True", "True", "True", "True", "True", "True", "True", "True"},
  { "0", "3", "4", "2", "6", "7", "8", "1", "5"},
  { "0.2", "0.1", "0.4", "0.1", "0.5", "0.002", "0.002", "0.2", "0.1"},
  { "True", "True", "True", "True", "True", "True", "True", "True", "True"},
  { "0", "3", "4", "2", "6", "7", "8", "1", "5"},
  { "-1", "-1", "-1", "-1", "-1", "-1", "-1", "-1", "-1"},
  { "1.017", "1.017", "1.017", "1.017", "1.017", "1.017", "1.017", "1.017", "1.017"},
  { "0.003333", "0.003333", "0.003333", "0.003333", "0.003333", "0.003333", "0.003333", "0.003333", "0.003333"},
  { "0.00192308", "0.00192308", "0.00192308", "0.00192308", "0.00192308", "0.00192308", "0.00192308", "0.00192308", "0.00192308"},
  { "4.25", "3.30", "0.33", "5.79", "5.91", "2.35", "0.27", "0.93", "0.44"}
};

const String NomeSensoriNonMq[] = {"MG811", "HCH0", "MHZ16", "MultiChannelGasSensor", "PPD42NS", "AirQuality", "SMPWM01A"};
bool sensoriConnessiNonMq[] = {true, true, true/*è UART del sens MHZ16*/, false/*I2C*/, false, true/*non si sa*/, true};

//int pinSensoriNonMqInterni{,,,,,8,14,-1};
//int pinSensoriNonMqEsterni{};
//bool sensoreAD[] = {true, true, true, true, true, true, true, true}; //Specifica se il pin è digitale o analogico true=analogico, false=digitale.
float TAcAM2302DP;//Temperatura in celsius AM2302 sensor in detection Point che sarà per esempio I1(interno) o E1(Esterno1) al posto di DP.
float RhAM2302DP;//Umidità in % in celsius AM2302 sensor in detection Point che sarà per esempio I1(interno) o E1(Esterno1) al posto di DP.
int prioritaInterno[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1}; //0 non ha priorità, 1 ha priorità, -1 nessun azione

void setup() {
  Serial.begin(9600);
  //Inizializzazione Sensori Interni
  ObjSensoreInterno.AM2302_SetUp(2);
  ObjSensoreInterno.MultiChannelGasSensor_SetUp();
  ObjSensoreInterno.PPD42NS_SetUp(8);
  ObjSensoreInterno.AirQualitySensor_Setup(14);
  //ObjSensoreInterno.dust.begin();
  //Fine Inizializzazione sensori Interni
  //Inizializzazione Sensori Esterni
  ObjSensoreEsterno.AM2302_SetUp(2);
  ObjSensoreEsterno.MultiChannelGasSensor_SetUp();
  ObjSensoreEsterno.PPD42NS_SetUp(8);
  ObjSensoreEsterno.AirQualitySensor_Setup(14);
  //ObjSensoreInterno.dust.begin();
  //FIne inizializzazione sensori Esterni
}

void loop() {
  PrintLetturaSensoriInterni();
  Serial.println("");
  PrintLetturaSensoriEsterni();
  ImpostaPriorita();
  ElaboraAzioni();
  delay(6000);
}

void PrintLetturaSensoriInterni()
{
  Serial.println("###########Inizio lettura sensori Interni###########");
  CalcoloRHinterno();
  PrintRHinterno();
  Serial.println("Inizio Lettura sensore MicS6814(MultiChannelGasSensor)");
  ObjSensoreInterno.MultiChannelGasSensor_NH3();//Server per entrare nel metodo che calcola le ratio dei gas parametrizzati
  Serial.print("MicS6814(MultiChannelGasSensor) ratio1: ");
  Serial.println(ObjSensoreInterno.MultiChannelGasSensor_Ratio1());
  Serial.print("MicS6814(MultiChannelGasSensor) ratio2: ");
  Serial.println(ObjSensoreInterno.MultiChannelGasSensor_Ratio2());
  Serial.print("MicS6814(MultiChannelGasSensor) ratio3: ");
  Serial.println(ObjSensoreInterno.MultiChannelGasSensor_Ratio3());
  Serial.println("Fine Lettura sensore MicS6814(MultiChannelGasSensor)");
  Serial.print("Temperatura AM2302: ");
  Serial.println(ObjSensoreInterno.AM2302_Temp());
  Serial.print("Umidità AM2302: ");
  Serial.println(ObjSensoreInterno.AM2302_Hum());
  Serial.print("MG811 volts: ");
  Serial.println(ObjSensoreInterno.MG811_Volts());
  Serial.print("MG811 CO2: ");
  Serial.println(ObjSensoreInterno.MG811_CO2());
  Serial.print("HCH0 r0: ");
  Serial.println(ObjSensoreInterno.HCH0_R0());
  Serial.print("Airquality(MP503): ");
  ObjSensoreInterno.AirQualitySensor();
  Serial.print("Mhz16 temperatura: ");
  Serial.println(ObjSensoreInterno.MHZ16_Temperature());//ritorna sempre 0
  Serial.print("Mhz16 co2: ");
  Serial.println(ObjSensoreInterno.MHZ16_CO2());//ritorna sempre 0
  Serial.print("PPD42NS time: ");
  Serial.println(ObjSensoreInterno.PPD42NS_Time(8));//ritorna sempre 0
  Serial.print("PPD32NS Concetrazione: ");
  Serial.println(ObjSensoreInterno.PPD42NS_Concentration());//ritorna sempre 0
  Serial.print("PPD32NS Duty Cycle: ");
  Serial.println(ObjSensoreInterno.PPD42NS_Duty_Cycle());//ritorna sempre 0
  /*ObjSensoreInterno.MG811();
    ObjSensoreInterno.HCH0();
    ObjSensoreInterno.MHZ16();
    ObjSensoreInterno.MultiChannelGasSensor();
    ObjSensoreInterno.AirQualitySensor();
    ObjSensoreInterno.PPD42NS(8);//Sempre lo stesso risultato
    ObjSensoreInterno.SMPWM01ALettura();*/
  Serial.println("###########Fine lettura sensori Interni###########");
}

void PrintLetturaSensoriEsterni()
{
  Serial.println("###########Inizio lettura sensori Esterni###########");
  CalcoloRHesterno();
  PrintRHesterno();
  Serial.println("Inizio Lettura sensore MicS6814(MultiChannelGasSensor)");
  ObjSensoreEsterno.MultiChannelGasSensor_NH3();//Server per entrare nel metodo che calcola le ratio dei gas parametrizzati
  Serial.print("MicS6814(MultiChannelGasSensor) ratio1: ");
  Serial.println(ObjSensoreEsterno.MultiChannelGasSensor_Ratio1());
  Serial.print("MicS6814(MultiChannelGasSensor) ratio2: ");
  Serial.println(ObjSensoreEsterno.MultiChannelGasSensor_Ratio2());
  Serial.print("MicS6814(MultiChannelGasSensor) ratio3: ");
  Serial.println(ObjSensoreEsterno.MultiChannelGasSensor_Ratio3());
  Serial.println("Fine Lettura sensore MicS6814(MultiChannelGasSensor)");
  Serial.print("Temperatura AM2302: ");
  Serial.println(ObjSensoreEsterno.AM2302_Temp());
  Serial.print("Umidità AM2302: ");
  Serial.println(ObjSensoreEsterno.AM2302_Hum());
  Serial.print("MG811 volts: ");
  Serial.println(ObjSensoreEsterno.MG811_Volts());
  Serial.print("MG811 CO2: ");
  Serial.println(ObjSensoreEsterno.MG811_CO2());
  Serial.print("HCH0 r0: ");
  Serial.println(ObjSensoreEsterno.HCH0_R0());
  Serial.print("Airquality(MP503): ");
  ObjSensoreEsterno.AirQualitySensor();
  Serial.print("Mhz16 temperatura: ");
  Serial.println(ObjSensoreEsterno.MHZ16_Temperature());//ritorna sempre 0
  Serial.print("Mhz16 co2: ");
  Serial.println(ObjSensoreEsterno.MHZ16_CO2());//ritorna sempre 0
  Serial.print("PPD42NS time: ");
  Serial.println(ObjSensoreEsterno.PPD42NS_Time(8));//ritorna sempre 0
  Serial.print("PPD32NS Concetrazione: ");
  Serial.println(ObjSensoreEsterno.PPD42NS_Concentration());//ritorna sempre 0
  Serial.print("PPD32NS Duty Cycle: ");
  Serial.println(ObjSensoreEsterno.PPD42NS_Duty_Cycle());//ritorna sempre 0
  /*ObjSensoreEsterno.MG811();
    ObjSensoreEsterno.HCH0();
    ObjSensoreEsterno.MHZ16();
    ObjSensoreEsterno.MultiChannelGasSensor();
    ObjSensoreEsterno.AirQualitySensor();
    ObjSensoreEsterno.PPD42NS(8);//Sempre lo stesso risultato
    ObjSensoreEsterno.SMPWM01ALettura();*/
  Serial.println("###########Fine lettura sensori Esterni###########");
}

void CalcoloRHinterno()
{
  TAcAM2302DP = ObjSensoreInterno.AM2302_Temp();
  RhAM2302DP = ObjSensoreInterno.AM2302_Hum();

  Serial.print("Temperatura ");
  Serial.println(TAcAM2302DP);
  Serial.print("hum: ");
  Serial.println((RhAM2302DP));

  for (int i = 0; i < 9; i++)
  {
    if ((bool)datiSensoriMQ[1][i])
    {
      datiSensoriMQ[3][i] = ObjSensoreInterno.MQ_Ratio(datiSensoriMQ[2][i].toInt(), datiSensoriMQ[10][i].toFloat());
      Serial.print("Ratio del sensore " + datiSensoriMQ[0][i] + ": ");
      Serial.println(datiSensoriMQ[3][i]);
      datiSensoriMQ[3][i] = datiSensoriMQ[3][i].toFloat() - ((RhAM2302DP - 65) * datiSensoriMQ[9][i].toFloat());
      if (TAcAM2302DP < 20)
      {
        Serial.println("Temperatura < 20");
        datiSensoriMQ[3][i] = datiSensoriMQ[3][i].toFloat() * pow(datiSensoriMQ[7][i].toFloat(), 20 - TAcAM2302DP);
      }
      else
      {
        if (TAcAM2302DP > 20)
        {
          Serial.println("Temperatura > 20");
          datiSensoriMQ[3][i] = datiSensoriMQ[3][i].toFloat() - ((TAcAM2302DP - 20) * datiSensoriMQ[8][i].toFloat());
        }
      }
    }
  }
}
void PrintRHinterno()
{
  for (int i = 0; i < 9; i++)
  {
    if ((bool)datiSensoriMQ[1][i])
    {
      Serial.println(datiSensoriMQ[0][i] + "_RH=" + datiSensoriMQ[3][i]);
    }
  }
}

void CalcoloRHesterno()
{
  TAcAM2302DP = ObjSensoreEsterno.AM2302_Temp();
  RhAM2302DP = ObjSensoreEsterno.AM2302_Hum();

  Serial.print("Temperatura ");
  Serial.println(TAcAM2302DP);
  Serial.print("hum: ");
  Serial.println((RhAM2302DP));

  for (int i = 0; i < 9; i++)
  {
    if ((bool)datiSensoriMQ[4][i])
    {
      datiSensoriMQ[6][i] = String(ObjSensoreEsterno.MQ_Ratio(datiSensoriMQ[2][i].toInt(), datiSensoriMQ[10][i].toFloat()));
      Serial.print("Ratio del sensore " + datiSensoriMQ[0][i] + ": ");
      Serial.println(datiSensoriMQ[6][i]);
      datiSensoriMQ[6][i] = datiSensoriMQ[6][i].toFloat() - ((RhAM2302DP - 65) * datiSensoriMQ[9][i].toFloat());
      if (TAcAM2302DP < 20)
      {
        Serial.println("Temperatura < 20");
        datiSensoriMQ[6][i] = datiSensoriMQ[6][i].toFloat() * pow(datiSensoriMQ[7][i].toFloat(), 20 - TAcAM2302DP);
      }
      else
      {
        if (TAcAM2302DP > 20)
        {
          Serial.println("Temperatura > 20");
          datiSensoriMQ[6][i] = datiSensoriMQ[6][i].toFloat() - ((TAcAM2302DP - 20) * datiSensoriMQ[8][i].toFloat());
        }
      }
    }
  }
}

void PrintRHesterno()
{
  for (int i = 0; i < 9; i++)
  {
    if ((bool)datiSensoriMQ[4][i])
    {
      Serial.println(datiSensoriMQ[0][i] + "_RH=" + datiSensoriMQ[6][i]);
    }
  }
}

ISR(TIMER1_OVF_vect)
{
  if (ObjSensoreInterno.airqualitysensor.counter == 61) //set 2 seconds as a detected duty
  {
    ObjSensoreInterno.airqualitysensor.last_vol = ObjSensoreInterno.airqualitysensor.first_vol;
    ObjSensoreInterno.airqualitysensor.first_vol = analogRead(A0);
    //Serial.print("Lettura Analogica");
    //Serial.print(A0);
    ObjSensoreInterno.airqualitysensor.counter = 0;
    ObjSensoreInterno.airqualitysensor.timer_index = 1;
    PORTB = PORTB ^ 0x20;
  }
  else
  {
    ObjSensoreInterno.airqualitysensor.counter++;
  }
}

void ImpostaPriorita()
{
  if (datiSensoriMQ[3][0].toFloat() <= 2)//Priorità MQ2
  {
    prioritaInterno[0] = 1;
  }
  if (datiSensoriMQ[3][1].toFloat() <= 4)//Priorità MQ3
  {
    prioritaInterno[1] = 0;
  }
  if (datiSensoriMQ[3][2].toFloat() <= 2.5)//Priorità MQ4
  {
    prioritaInterno[2] = 1;
  }
  if (datiSensoriMQ[3][3].toFloat() <= 0.95)//Priorità MQ5
  {
    prioritaInterno[3] = 1;
  }
  if (datiSensoriMQ[3][4].toFloat() <= 2.5)//Priorità MQ6
  {
    prioritaInterno[4] = 1;
  }
  if (datiSensoriMQ[3][5].toFloat() <= 4)//Priorità MQ7
  {
    prioritaInterno[5] = 0;
  }
  if (datiSensoriMQ[3][6].toFloat() <= 1)//Priorità MQ8
  {
    prioritaInterno[6] = 1;
  }
  if (datiSensoriMQ[3][7].toFloat() <= 4)//Priorità MQ9
  {
    prioritaInterno[7] = 1;
  }
  if (datiSensoriMQ[3][8].toFloat() <= 3)//Priorità MQ13
  {
    prioritaInterno[8] = 0;
  }
  //Per tutti gli altri sensori bisogna controllare bene i valori da verificare perche in qualche sensore è più di uno.
}

void ElaboraAzioni()
{
  //Verifica se esistono azioni con priorità 1
  bool priorita1 = false;
  int j = 0;
  while ((j < 9) && (!priorita1))
  {
    if (prioritaInterno[j] == 1)
    {
      priorita1 = true;
    }
    j++;
  }

  if (priorita1)
  {
    ElaboraPriorita1();
  }
  else
  {
    ElaboraPriorita0();
  }
  for (int i = 0; i < 9; i++)
  {
    prioritaInterno[i] = -1;
  }
}

void ElaboraPriorita0()
{
  for ( int i = 0; i < 9; i++)
  {
    if (prioritaInterno[i] == 0)
    {
      if (i == 1)
      {
        ObjAzioni.IEMQ3AQC();
      }
      if (i == 5)
      {
        ObjAzioni.IEMQ7AQC();
      }
      if (i == 8)
      {
        ObjAzioni.IEMQ135AQC();
      }
    }
  }
}

void ElaboraPriorita1()
{
  for ( int i = 0; i < 9; i++)
  {
    if (prioritaInterno[i] == 1)
    {
      if (i == 0)
      {
        ObjAzioni.AL1();
        ObjAzioni.ELG();
        ObjAzioni.EE();
        ObjAzioni.EAQC();
      }
      if (i == 2)
      {
        ObjAzioni.AL1();
        ObjAzioni.ELG();
        ObjAzioni.EE();
        ObjAzioni.EAQC();
      }
      if (i == 3)
      {
        ObjAzioni.AL1();
        ObjAzioni.ELG();
        ObjAzioni.EE();
        ObjAzioni.EAQC();
      }
      if (i == 4)
      {
        ObjAzioni.AL1();
        ObjAzioni.ELG();
        ObjAzioni.EE();
        ObjAzioni.EAQC();
      }
      if (i == 6)
      {
        ObjAzioni.AL2();
        ObjAzioni.EE();
        ObjAzioni.I_OP();
        ObjAzioni.EAQC();
      }
      if (i == 7)
      {
        ObjAzioni.AL1();
        ObjAzioni.ELG();
        ObjAzioni.EE();
        ObjAzioni.EAQC();
      }
    }
  }
}

