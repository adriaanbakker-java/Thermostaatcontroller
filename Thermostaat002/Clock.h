

class Clock {
  private:
    int uren;
    int minuten;
    int seconden;
   
  public:
    int geefMinuten();
    int geefUren();
    int geefSeconden();
    void resetClock();
    void geefTijdstring(char buffer[]);
    void incSeconds(int aSeconds);
};
