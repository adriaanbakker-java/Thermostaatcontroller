#include "Clock.h"
#include "arduino.h"
#define minutesPerHour 60
#define secondsPerMinute 60
#define hoursPerDay 24
    
    int Clock::geefMinuten() {
      return minuten;
    }
    int Clock::geefUren() {
      return uren;
    }

    int Clock::geefSeconden() {
      return seconden;
    }
    void Clock::resetClock() {
      seconden = 0;
      minuten = 0;
      uren = 0;
      timerMinuten = 0;
      timerSeconden = 0;
    }
   
    void Clock::geefTijdstring(char buffer[]) {
        char sHours[5];
        char sMinutes[5];
        char sSeconden[5];
        
        if (uren < 10) {
          sprintf(sHours, "0%d", uren);
        } else {
          sprintf(sHours, "%d", uren);
        }
        if (minuten < 10) {
          sprintf(sMinutes, "0%d", minuten);
        } else {
          sprintf(sMinutes, "%d", minuten);
        }
        if (seconden < 10) {
          sprintf(sSeconden, "0%d", seconden);
        } else {
          sprintf(sSeconden, "%d", seconden);
        }
        sprintf(buffer, "%s:%s:%s", sHours, sMinutes, sSeconden);
    }

    void Clock::incSeconds(int aSeconden) {
       if (timerMinuten < 24*60) {
         timerSeconden += aSeconden;
         timerMinuten += timerSeconden / secondsPerMinute;
         timerSeconden %= secondsPerMinute;
       }
       
       seconden += aSeconden;
       minuten += seconden / secondsPerMinute;
       seconden %= secondsPerMinute;
       uren += minuten / minutesPerHour;
       minuten %= minutesPerHour;
       uren %= hoursPerDay;
    }

    void Clock::startTimer() {
        timerMinuten = 0;
        timerSeconden = 0;
    }

    int Clock::geefTimerMinuten() {
        return timerMinuten;
    }
