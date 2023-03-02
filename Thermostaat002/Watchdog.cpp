#include "Watchdog.h"
#include<avr/wdt.h> /* Header for watchdog timers in AVR */

void Watchdog::init() {
   // disable de watchdog tot na opstarten
   wdt_disable();  /* Disable the watchdog and wait for more than 2 seconds */
   delay(3000);  /* Done so that the Arduino doesn't keep resetting infinitely in case of wrong configuration */
   wdt_enable(WDTO_8S);  /* Enable the watchdog with a timeout of 2 seconds */
}

// delay en reset watchdog timer
void Watchdog::myDelay(int aDelayMsec) {
  wdt_reset();  /* Reset the watchdog */
  delay(aDelayMsec); 
}


void Watchdog::disable() {
     wdt_disable();     
}


void Watchdog::enable() {
     wdt_enable(WDTO_8S);     
}
