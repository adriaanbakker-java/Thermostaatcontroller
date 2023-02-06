#include <PCD8544.h> 

class Menu {
   public:
     void init( PCD8544 * aLCD, byte aPortKey0, byte aPortKey1, byte aPortKey2);
     void checkMenuKey();

   private:
      byte  getMenuKey();
      PCD8544 * myLCD;
      byte portKey0;
      byte portKey1;
      byte portKey2;
  
};
