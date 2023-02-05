
#include "arduino.h"
class Kachel {
   public:
       // Member functions declaration
      double getVolume(void);
      void setLength( double len );
      void setBreadth( double bre );
      void setHeight( double hei );
      void init(int aRelaisport);
      void zetKachelUit();
      bool isKachelAan();
      void zetKachelAan();
   private:
      double length;      // Length of a box
      double breadth;     // Breadth of a box
      double height;      // Height of a box

      int  relaisport;
      bool bIsKachelAan;
  
};
