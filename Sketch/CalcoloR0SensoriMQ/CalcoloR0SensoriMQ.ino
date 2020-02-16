
#include <Dpc.h>
#include <Sensori.h>

Sensori ObjSensore;//Oggetto della classe sensori
/*
const float costantiR0[] = {9.8, 60, 60, 6.5, 9.2, 27, 60, 10, 60};
const String NomeSensori[] = {"MQ2", "MQ3", "MQ4", "MQ5", "MQ6", "MQ7", "MQ8", "MQ9", "MQ135"};
bool sensoriConnessi[] = {true, true, true, true, true, true, true, false, true };
int pin[] = {0, 3, 4, 2, 6, 7, 8, -1, 5}; //array che indica il pin di connessione di un sesnore, -1 corrisponde al sensore non connesso(corrisponderà ad un false nella corrisponde posizione dell array sensoriConnessi.).
//bool sensoreAD[] = {true, true, true, true, true, true, true, true, true}; //Specifica se il pin è digitale o analogico true=analogico, false=digitale.
float R0[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1}; //-1 intende che R0 non è ancora calcolato per il sensore corrispondente
*/

String datiSensoriMQ[][9] = {
  { "Q2", "MQ3", "MQ4", "MQ5", "MQ6", "MQ7", "MQ8", "MQ9", "MQ135"},
  { "True", "True", "True", "True", "True", "True", "True", "True", "True"},
  { "0", "3", "4", "2", "6", "7", "8", "1", "5"},
  { "9.8", "60", "60", "6.5", "9.2", "27", "60", "10", "60"},
  { "-1", "-1", "-1", "-1", "-1", "-1", "-1", "-1", "-1"},
};

void setup() {
  Serial.begin(9600);
}

void loop() {
  CalcoloR0();
  Serial.println("########Inizio scrittura########");
  PrintR0();
  Serial.println("########Fine scrittura########");
  Serial.println();
}

void CalcoloR0()
{
  for (int i = 0; i < 9; i++)
  {
    if ((bool)datiSensoriMQ[1][i])
    {
      datiSensoriMQ[4][i] = ObjSensore.MQ_R0(datiSensoriMQ[2][i].toInt(), datiSensoriMQ[3][i].toFloat());
    }
  }
}
void PrintR0()
{
  for (int i = 0; i < 9; i++)
  {
    Serial.println(datiSensoriMQ[0][i] + "_R0=" + datiSensoriMQ[4][i]);
  }
}

