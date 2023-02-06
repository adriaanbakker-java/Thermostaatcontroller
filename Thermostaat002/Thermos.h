
#ifndef Thermos_h
#define Thermos_h

#include "arduino.h"


#define huidigSchemaActief "Actief"
#define huidigSchemaPauze  "Pauze "
#define huidigSchemaUit    "Uit   "
#define huidigSchemaActiefNr 1
#define huidigSchemaPauzeNr  2
#define huidigSchemaUitNr    0


class Thermos {
   public:
      void init();
      double getTempAan(byte aSchemaNr);
      double getTempUit(byte aSchemaNr);
      void  geefSchemaNaam(char buffer[], byte aSchemaNr);
      byte   getHuidigSchemaNr();
      int   geefTimeout(byte aSchemaNr);
      boolean checkSetSchema(int aTimerMinuten);
   private:
      double  tempAan;
      double  tempUit; 
      double  tempAanPauze;
      double  tempUitPauze;  
      byte    huidigSchemaNr;    // 0 = uit, 1 = pauze, 2 = actief
      void switchNaarActief();
      void switchNaarPauze();
      void switchNaarUit();

};
#endif
