#include "TempSensor.h"
#include "arduino.h"

void TempSensor::init(int aSensorPort) {
  sensorPort = aSensorPort;
}


double TempSensor::calcTemp(int sensorValue) {
 int tot = sensorValue;
 double Rnc = (1023.00 -tot)/tot *100;
    double lnRnc = log(Rnc);
    
// 4 feb 2023   567 komt overeen met 17,8 graden
//              421 komt overeen met  7,1 graden
    double B =   4385.497029;
    double lnA = log(0.0000229); 
    
    double TKelvin = B / (lnRnc - lnA );
    return TKelvin - 273.15;
}




int TempSensor::meetTempSensor () {
    int val;
  double tot = 0;
  int nrtimes = 5;
  for (int i = 1; i <= nrtimes; i++) {
       val = analogRead(sensorPort);
      delayMicroseconds(200);
      val = analogRead(sensorPort);
      tot += val;
    }
    tot = tot/nrtimes;

    return tot;
}
