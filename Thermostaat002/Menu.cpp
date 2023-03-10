#include "Menu.h"
#include "Thermos.h"
  
   
     void Menu::init( PCD8544 * aLCD, 
                      Thermos * aThermos,
                      byte aPortKey0, 
                      byte aPortKey1, 
                      byte aPortKey2) {
        myLCD = aLCD;
        myThermos = aThermos;
     
        
        portKey0 = aPortKey0;
        portKey1 = aPortKey1;
        portKey2 = aPortKey2;
     }

   

     void Menu::hoofdMenu() {
          byte key = doSelect("hoofdmenu", "actiefschema", "pauzeschema", "");
            
             do {
                if (key == 2) {
                   actiefSchema();
                   return;
                }
                if (getMenuKey() == 3) {
                   pauzeSchema();
                   return;
                }
                myDelay(1000);
             } while (getMenuKey() != 1); 
     }

     void Menu::checkMenuKey() {
         if (getMenuKey() == 1) {            
            hoofdMenu();
         }
     }

     void Menu::myDelay(int aDelayMsec) {
        delay(1000);
     }

     byte Menu::getMenuKey() {
        if (digitalRead(portKey0) == HIGH) {
          myDelay(200);
          if (digitalRead(portKey1) == HIGH) return 4; // 1,2 samen ingedrukt
          return 1;
        }
        if (digitalRead(portKey1) == HIGH) {
          myDelay(200);
          if (digitalRead(portKey0) == HIGH) return 4; // 1,2 samen ingedrukt
          if (digitalRead(portKey2) == HIGH) return 5; // 2,3 samen ingedrukt
          return 2;
        }
        if (digitalRead(portKey2) == HIGH) {
          myDelay(200);
          if (digitalRead(portKey1) == HIGH) return 5; // 2,3 samen ingedrukt
          return 3;
        }
        return 0;
     }

     byte Menu::doSelect( char *titel, char *keuze1, char* keuze2, char* keuze3 ) {
         
         myLCD->clear();

         
         myLCD->setCursor(0,0);
         myLCD->print(titel);

         myLCD->setCursor(0,1);
         myLCD->print("1=terug");
         myLCD->setCursor(0, 2);
         myLCD->print("2=");
         myLCD->print(keuze1);
         myLCD->setCursor(0, 3);       
         myLCD->print("3=");
         myLCD->print(keuze2);

         if (strcmp(keuze3,"") != 0) {
           myLCD->print("1+2=");
           myLCD->print(keuze3);
         }
         
         byte keuze;
         myDelay(1000);
         do {
            keuze = getMenuKey();
         } while (keuze == 0);
         myLCD->clear();
         return keuze;
     }

     void Menu::actiefSchema() {
         byte keuze = doSelect("actief schema", "temperaturen", "timeout", "");
         if (keuze == 2) {
             verstelTemperaturen(huidigSchemaActiefNr);
             return;
         }
         if (keuze == 3) {
             verstelTimeout(huidigSchemaActiefNr);
             return;
         }
     }
     
     void Menu::pauzeSchema() {        
         byte keuze = doSelect("pauze schema", "temperaturen", "timeout", "");
         if (keuze == 2) {
             verstelTemperaturen(huidigSchemaPauzeNr);
             return;
         }
         if (keuze == 3) {
             verstelTimeout(huidigSchemaPauzeNr);
             return;
         }
     }

    void Menu::toonTemperaturen( byte aSchemaNr) {
      myLCD->setCursor(0, 1);
      myLCD->print(myThermos->getTempAan(aSchemaNr));
      myLCD->setCursor(0, 3);
      myLCD->print(myThermos->getTempUit(aSchemaNr)); 
    } 

    

    void Menu::verstelTemperatuur( byte aSchemaNr, boolean aIsTempAan, boolean aIsPlus) {
       double temp = aIsTempAan ? myThermos->getTempAan(aSchemaNr) : myThermos->getTempUit(aSchemaNr);
       double increment = aIsPlus ? 0.1 : -0.1;
       if (aIsTempAan) { 
          myThermos->incTempAan(aSchemaNr, increment);
       } else {
          myThermos->incTempUit(aSchemaNr, increment);
       }
       toonTemperaturen(aSchemaNr);
    } 

    void Menu::verstelTemperaturen( byte aSchemaNr ) {
        myLCD->clear();
        
        char sBuffer[10];
        char sTitel[20];  
        myThermos->geefSchemaNaam(sBuffer, aSchemaNr);
        sprintf(sTitel, "%s:%s","temps ",sBuffer);
        myLCD->setCursor(0, 0);
        myLCD->print(sTitel);
        
        myLCD->setCursor(0, 2);
        myLCD->print("2=++ 3=--");
       
        myLCD->setCursor(0, 4);
        myLCD->print("12=++ 23=--");
        
        myLCD->setCursor(0, 5);
        myLCD->print("1=terug");
        
        toonTemperaturen(aSchemaNr);
        
        byte keuze;
        do {
            myDelay(500);
            keuze = getMenuKey();
            if (keuze == 2) verstelTemperatuur(aSchemaNr, true, true);
            if (keuze == 3)  verstelTemperatuur(aSchemaNr, true, false);
            //12
            if (keuze == 4)  verstelTemperatuur(aSchemaNr, false, true);
            //23
            if (keuze == 5)  verstelTemperatuur(aSchemaNr, false, false);
         } while (keuze != 1);
    }


   void Menu::toonTimeout( byte aSchemaNr) {
      myLCD->setCursor(0, 1);
      myLCD->print(myThermos->geefTimeout(aSchemaNr));
    } 

    void Menu::incTimeout( byte aSchemaNr, boolean aPlus, boolean aDelta10) {
        int timeOut = myThermos->geefTimeout(aSchemaNr);
        if (aPlus) { 
          myThermos->incTimeout(aSchemaNr, aDelta10);
       } else {
          myThermos->decTimeout(aSchemaNr, aDelta10);
       }     
    }

     void Menu::verstelTimeout( byte aSchemaNr ) {
        myLCD->clear();
        myLCD->print("timeout:");
        char sBuffer[30];
        myThermos->geefSchemaNaam(sBuffer, aSchemaNr);
        myLCD->print(sBuffer);

        toonTimeout(aSchemaNr);

        myLCD->setCursor(0, 2);
        myLCD->print("2=++ 3=--");
        
        byte keuze;
        int aantalKerenPlus = 0;  // herhaald indrukken ineens plus 10
        int aantalKerenMin = 0;   // analoog min 10
        do {
            myDelay(500);
            keuze = getMenuKey();
            if (keuze == 2) {
                aantalKerenMin = 0;
                aantalKerenPlus++;
                incTimeout(aSchemaNr, true, (aantalKerenPlus > 10));
                toonTimeout(aSchemaNr);
            }
            if (keuze == 3) {
                aantalKerenPlus = 0;
                aantalKerenMin++;
                incTimeout(aSchemaNr, false, (aantalKerenMin > 10));
                toonTimeout(aSchemaNr);
            }
         } while (keuze != 1);
    }
     
      
   
