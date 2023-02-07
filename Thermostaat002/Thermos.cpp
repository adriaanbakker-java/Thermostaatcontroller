#include "arduino.h"
#include "Thermos.h"




void Thermos::init() {
      tempAan = 18.0;
      tempUit = 19.0;
      tempAanPauze = 15.0;
      tempUitPauze = 16.0;
      timeoutActief = 5;   // 5 minuten en dan naar pauzeschema
      timeoutPauze = 120;    // 120 minuten en dan uit
      huidigSchemaNr = huidigSchemaActiefNr;
}

double Thermos::getTempAan(byte aSchemaNr) {
    switch (aSchemaNr) {
    case huidigSchemaActiefNr: return tempAan;
    case huidigSchemaPauzeNr: return tempAanPauze;
    case huidigSchemaUitNr: return 0.0;
    default: return 0.0;  
   }
}

double Thermos::getTempUit(byte aSchemaNr) {
    switch (aSchemaNr) {
    case huidigSchemaActiefNr: return tempUit;
    case huidigSchemaPauzeNr: return tempUitPauze;
    case huidigSchemaUitNr: return 0.0;
    default: return 0.0;  
   }
}

void Thermos::geefSchemaNaam(char buffer[], byte aSchemaNr) {
   switch (aSchemaNr) {
    case huidigSchemaActiefNr: sprintf(buffer, huidigSchemaActief); return;
    case huidigSchemaPauzeNr: sprintf(buffer, huidigSchemaPauze); return;
    case huidigSchemaUitNr: sprintf(buffer, huidigSchemaUit); return;
    default: sprintf(buffer, "UNKNOWN"); return;  
   }
}

byte Thermos::getHuidigSchemaNr() {
    return huidigSchemaNr;
}

void Thermos::setSchemaActief() {
    huidigSchemaNr = huidigSchemaActiefNr;
}

boolean Thermos::checkSetSchema(int aTimerMinuten) {
   int timeout = geefTimeout(huidigSchemaNr);
   Serial.print("checkSetSchema");
   Serial.print(" ");
   Serial.print(timeout);
   Serial.print(" ");
   Serial.println(aTimerMinuten);
   
   switch (huidigSchemaNr) {
    case huidigSchemaActiefNr: 
      if (aTimerMinuten >= timeout) {
          switchNaarPauze();
          return true;
      }; 
      return false;
    case huidigSchemaPauzeNr: 
      if (aTimerMinuten >= timeout) {
          switchNaarUit();
          return true;
      }; 
      return false;
    case huidigSchemaUitNr: 
      return false;
    default:  
      return false;  
   }
}


int Thermos::geefTimeout(byte aSchemaNr) {
  switch (aSchemaNr) {
    case huidigSchemaActiefNr: 
      return timeoutActief; 
    case huidigSchemaPauzeNr: 
      return timeoutPauze;
    case huidigSchemaUitNr: return 0;
    default:  return -1;  
   }
}

void Thermos::switchNaarPauze() {
   Serial.println("switch naar pauze");
   huidigSchemaNr = huidigSchemaPauzeNr;
}


void Thermos::switchNaarUit() {
   Serial.println("switch naar uit");
   huidigSchemaNr = huidigSchemaUitNr;
}

void Thermos::incTempAan(byte aSchemaNr, double aIncrement) {
    if (aSchemaNr == huidigSchemaActiefNr) {
       if (tempAan + aIncrement <= tempUit - 0.4)
             tempAan += aIncrement;
    } else   
    if (aSchemaNr == huidigSchemaPauzeNr) {
       if (tempAanPauze + aIncrement <= tempUitPauze - 0.4)
             tempAanPauze += aIncrement;
    } 
}

void Thermos::incTempUit(byte aSchemaNr, double aIncrement) {
    if (aSchemaNr == huidigSchemaActiefNr) {
        if (tempUit + aIncrement >= tempAan + 0.4)
             tempUit += aIncrement;
    } else   if (aSchemaNr == huidigSchemaPauzeNr) {
        if (tempUitPauze + aIncrement >= tempAanPauze + 0.4)
             tempUitPauze += aIncrement;
    } 
}

void Thermos::incTimeout(byte aSchemaNr, boolean aDelta10) {
    Serial.println("inc Timeout voor schema ");
    Serial.println(aSchemaNr);
    int delta = aDelta10? 10 : 1;
    if (aSchemaNr == huidigSchemaActiefNr)
      timeoutActief += delta;   
    if (aSchemaNr == huidigSchemaPauzeNr)
      timeoutPauze += delta;   
}

void Thermos::decTimeout(byte aSchemaNr, boolean aDelta10) {
    Serial.println("dec Timeout voor schema ");
    Serial.println(aSchemaNr);
    int delta = aDelta10? 10 : 1;
    if (aSchemaNr == huidigSchemaActiefNr)
      if (timeoutActief-delta > 0) timeoutActief -= delta;   
    if (aSchemaNr == huidigSchemaPauzeNr)
      if (timeoutPauze-delta > 0) timeoutPauze -= delta;      
}
