
#include "arduino.h"
class Kachel {
   public:
      void init(int aRelaisport);
      void zetKachelUit();
      bool isKachelAan();
      void zetKachelAan();
   private:
      int  relaisport;
      bool bIsKachelAan;
  
};
