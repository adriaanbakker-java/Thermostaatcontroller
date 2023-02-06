
#ifndef Clock_h
#define Clock_h

class Clock {
  private:
    int uren;
    int minuten;
    int seconden;
    int timerMinuten; 
    int timerSeconden;

  
  public:
    int geefMinuten();
    int geefUren();
    int geefSeconden();
    void resetClock();
    void geefTijdstring(char buffer[]);
    void incSeconds(int aSeconds);


    void startTimer();

    int geefTimerMinuten();
};

#endif
