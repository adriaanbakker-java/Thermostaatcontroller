
#include "Kachel.h"

void Kachel::init(int aRelaisport) {
      relaisport = aRelaisport;
      bIsKachelAan = false;
}


void Kachel::zetKachelUit() {
            if (bIsKachelAan) {
                bIsKachelAan = false;
                digitalWrite(relaisport, LOW);
                Serial.println("KACHEL GAAT UIT");
            }   
}


bool Kachel::isKachelAan() {
      return bIsKachelAan;
}
  
void Kachel::zetKachelAan() {
             if (!bIsKachelAan) {
               bIsKachelAan = true;
               digitalWrite(relaisport, HIGH);
               Serial.println("KACHEL GAAT AAN");
             }   
}
  
  
