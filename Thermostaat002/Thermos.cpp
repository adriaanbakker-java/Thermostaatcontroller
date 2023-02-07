#include "arduino.h"
#include "Thermos.h"




void Thermos::init() {
      tempAan = 23.0;
      tempUit = 25.0;
      tempAanPauze = 22.0;
      tempUitPauze = 23.0;
      timeoutActief = 1;   // 1 minuut en dan naar pauzeschema
      timeoutPauze = 3;    // 3 minuten en dan uit
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

void Thermos::incTimeout(byte aSchemaNr) {
    Serial.println("inc Timeout voor schema ");
    Serial.println(aSchemaNr);
    if (aSchemaNr == huidigSchemaActiefNr)
      timeoutActief++;   
    if (aSchemaNr == huidigSchemaPauzeNr)
      timeoutPauze++;   
}

void Thermos::decTimeout(byte aSchemaNr) {
    Serial.println("dec Timeout voor schema ");
    Serial.println(aSchemaNr);
    if (aSchemaNr == huidigSchemaActiefNr)
      if (timeoutActief >0) timeoutActief--;   
    if (aSchemaNr == huidigSchemaPauzeNr)
      if (timeoutPauze >0) timeoutPauze--;      
}
