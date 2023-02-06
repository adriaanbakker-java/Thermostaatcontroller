#include <PCD8544.h> 
#include "Thermos.h"

class Menu {
   public:
     void init( PCD8544 * aLCD, Thermos * aThermos, byte aPortKey0, byte aPortKey1, byte aPortKey2);
     void checkMenuKey();

   private:

      PCD8544 * myLCD;
      Thermos * myThermos;
      
      byte portKey0;
      byte portKey1;
      byte portKey2;

      byte  getMenuKey();
      void hoofdMenu();
      void actiefSchema();
      void pauzeSchema();


      byte doSelect( char *titel, char *keuze1, char* keuze2, char* keuze3);
      void verstelTemperaturen( byte aSchemaNr );
      void verstelTemperatuur( byte aSchemaNr, boolean aIsTempAan, boolean aIsPlus); 
      void verstelTimeout( byte aSchemaNr );
      void toonTemperaturen( byte aSchemaNr);
};
