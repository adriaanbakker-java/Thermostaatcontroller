#include "arduino.h"
#include "Thermos.h"


#define timerwaardeActief 1
// 15 minuten op actief na detectie persoon, daarna pauze
#define timerwaardePauze 3 
// 4 uur lang op pauze, daarna naar uit


void Thermos::init() {
      tempAan = 23.0;
      tempUit = 25.0;
      tempAanPauze = 22.0;
      tempUitPauze = 23.0;
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
      return timerwaardeActief; 
    case huidigSchemaPauzeNr: 
      return timerwaardePauze;
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
