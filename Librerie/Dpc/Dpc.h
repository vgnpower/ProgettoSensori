#ifndef Dpc_h
#define Dpc_h

#include "Arduino.h"

class Dpc
{
  public:
    Dpc();
	  void setupSens(float _sens1, float _sens2);
    float calcEs();
    float calcE();
    float calcDPcl1(float _sens_TACl1AM2302, float _sens_RHAl1AM2302);
    float outputVar(bool _sensore);
  private:
     float sens_TACl1AM2302;
     float sens_RHAl1AM2302;
};

#endif
