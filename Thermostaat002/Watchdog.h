#ifndef Watchdog_h
#define Watchdog_h


#include "arduino.h"
class Watchdog {
   public:
      void init();
      void myDelay(int aDelayMsec);
      void enable();
      void disable();
   private:
    
  
};

#endif
