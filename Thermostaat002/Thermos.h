#include "arduino.h"


#define huidigSchemaActief "A"
#define huidigSchemaPauze  "P"
#define huidigSchemaUit    "X"
#define huidigSchemaActiefNr 2
#define huidigSchemaPauzeNr  1
#define huidigSchemaUitNr    0


class Thermos {
   public:
      void init();
      double getTempAan();
      double getTempUit();
      void  geefHuidigSchemaNaam(char buffer[]);
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
