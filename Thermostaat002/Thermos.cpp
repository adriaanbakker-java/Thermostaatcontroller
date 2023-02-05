#include "arduino.h"
#include "Thermos.h"


void Thermos::init() {
      tempAan = 19.0;
      tempUit = 21.0;
      tempAanPauze = 15.0;
      tempUitPauze = 16.0;
      huidigSchemaNr = huidigSchemaPauzeNr;
}

double Thermos::getTempAan() {
    switch (huidigSchemaNr) {
    case huidigSchemaActiefNr: return tempAan;
    case huidigSchemaPauzeNr: return tempAanPauze;
    case huidigSchemaUitNr: return 0.0;
    default: return 0.0;  
   }
}

double Thermos::getTempUit() {
    switch (huidigSchemaNr) {
    case huidigSchemaActiefNr: return tempUit;
    case huidigSchemaPauzeNr: return tempUitPauze;
    case huidigSchemaUitNr: return 0.0;
    default: return 0.0;  
   }
}

void Thermos::geefHuidigSchemaNaam(char buffer[]) {
   switch (huidigSchemaNr) {
    case huidigSchemaActiefNr: sprintf(buffer, huidigSchemaActief); return;
    case huidigSchemaPauzeNr: sprintf(buffer, huidigSchemaPauze); return;
    case huidigSchemaUitNr: sprintf(buffer, huidigSchemaUit); return;
    default: sprintf(buffer, "UNKNOWN"); return;  
   }
}
