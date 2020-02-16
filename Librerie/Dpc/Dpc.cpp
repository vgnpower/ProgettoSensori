#include "Arduino.h"
#include "Dpc.h"
//#include "math.h"

Dpc::Dpc()
{
    sens_RHAl1AM2302 = 2;
    sens_TACl1AM2302 = 2;
}

void Dpc::setupSens(float _sens1, float _sens2)
{
    sens_RHAl1AM2302 = _sens1;
    sens_TACl1AM2302 = _sens2;
}

float Dpc::outputVar(bool _sensore)
{
  if(_sensore)
  {
    return sens_RHAl1AM2302;
  }
  else
  {
    return sens_TACl1AM2302;
  }
}

float Dpc::calcEs()
{
  float primaParte = 7.5 * sens_TACl1AM2302;
  float secondaParte = 237.7 + sens_TACl1AM2302;
  float expnt = primaParte / secondaParte;

  float base1 = 6.11;
  float base2 = pow(10, expnt);
  float risultato = base1 * base2;

  return risultato;
}

float Dpc::calcE()
{
  float ris = (sens_RHAl1AM2302 * calcEs() / 100);

  return ris;
}

float Dpc::calcDPcl1(float _sens_TACl1AM2302, float _sens_RHAl1AM2302)
{
  sens_TACl1AM2302 = _sens_TACl1AM2302;
  sens_RHAl1AM2302 = _sens_RHAl1AM2302;
  float e = calcE();
  float primaParte = -430.22 + (237.7 * log(e));
  float secondaParte = -log(e + 19.08);
  float ris = primaParte / secondaParte;

  return ris;
}
