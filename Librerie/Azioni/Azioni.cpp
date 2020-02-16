#include "Arduino.h"
#include "Azioni.h"

void Azioni::AL1()
{
 Serial.println("Azione AL1");

 Serial.println("Chiusura relè per allarme sonoro");
}

void Azioni::AL2()
{
  Serial.println("Azione AL2");

  Serial.println("Chiusura relè per allarme sonoro");
}

void Azioni::AL3()
{
  Serial.println("Azione AL3");

  Serial.println("");
}

void Azioni::AL4()
{
  Serial.println("Azione AL4");

  Serial.println("");
}
void Azioni::ELG()
{
  Serial.println("Azione ELG");

  Serial.println("");
}
void Azioni::E()
{
  Serial.println("Azione EE");

  Serial.println("Attivazione MRE Velocità Max; Chiusura Totale SR; Chiusura totale SE; Gestione SFcE/SFcl");
}
void Azioni::EE()
{
  Serial.println("Azione EE");

  Serial.println("Attivazione MRE Velocità Max; Attivazione MFR Velocità Max; Chiusura Totale SR; Apertura Totale SE; Gestione SFcE/SFcl");
}
void Azioni::I_OP()
{
  Serial.println("Azione I_OP");

  Serial.println("Attivazione MRI; Gestione SFcE/SFcl");
}
void Azioni::EAQC()
{
  Serial.println("Azione EAQC");

  Serial.println("");
}
void Azioni::IEMQ3AQC()
{
  Serial.println("Azione IEMQ3AQC");

  Serial.println("");
}
void Azioni::IEMQ7AQC()
{
  Serial.println("Azione IEMQ7AQC");

  Serial.println("");
}
void Azioni::IEMQ135AQC()
{
  Serial.println("Azione IEMQ135AQC");

  Serial.println("");
}
void Azioni::FCAI()
{
  Serial.println("Azione FCAI");

  Serial.println("Attivazione MFR Velocità Max; Apertura Totale SFrC; Chiusura Totale SE; APertura totale SR");
}
void Azioni::FMAI()
{
  Serial.println("Azione FMAI");

  Serial.println("Attivazione MFR Velocità Max; Apertura Totale SFrP; Chiusura Totale SE; APertura totale SR");
}
void Azioni::IECOMICS6814AQC()
{
  Serial.println("Azione IECOMICS6814AQC");

  Serial.println("");
}

void Azioni::IENO2MICS6814AQC()
{
  Serial.println("Azione IENO2MICS6814AQC");

  Serial.println("");
}
void Azioni::IENH3MICS6814AQC()
{
  Serial.println("Azione IENH3MICS6814AQC");

  Serial.println("");
}
void Azioni::IEMP503AQC()
{
  Serial.println("Azione IEMP503AQC");

  Serial.println("");
}

void Azioni::IEWSP2110AQC()
{
  Serial.println("Azione IEWSP2110AQC");

  Serial.println("");
}
