#define relaisport 8

class Kachel {
  private:

   boolean bIsKachelAan = false;
  
 
  public: 
     init(byte aRelaisport) {
      zetKachelUit();
    }

    boolean isKachelAan() {
      return bIsKachelAan;
    }
  
     void zetKachelAan() {
             if (!bIsKachelAan) {
               bIsKachelAan = true;
               digitalWrite(relaisport, HIGH);
               Serial.println("KACHEL GAAT AAN");
             }   
     }
  
  
    void zetKachelUit() {
            if (bIsKachelAan) {
                bIsKachelAan = false;
                digitalWrite(relaisport, LOW);
                Serial.println("KACHEL GAAT UIT");
            }   
    }


};
