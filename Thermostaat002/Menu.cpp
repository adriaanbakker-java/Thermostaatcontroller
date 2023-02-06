#include "Menu.h"

     void Menu::init( PCD8544 * aLCD, byte aPortKey0, byte aPortKey1, byte aPortKey2) {
        myLCD = aLCD;
        myLCD->setCursor(0,4);
        myLCD->print("1=menu");

        portKey0 = aPortKey0;
        portKey1 = aPortKey1;
        portKey2 = aPortKey2;
     }

     void Menu::checkMenuKey() {
         if (getMenuKey() == 1) {            
             myLCD->clear();
             myLCD->setCursor(0, 0);
             myLCD->print("1 <--");
             myLCD->setCursor(0, 1);
             myLCD->print("2 temp");
             myLCD->setCursor(0, 2);
             myLCD->print("3 timeout");
             delay(1000);

             do {
                if (getMenuKey() == 2) {
                   Serial.println("temp menu");
                }
                if (getMenuKey() == 3) {
                   Serial.println("timeout menu");
                }
                delay(1000);
             } while (getMenuKey() != 1); 
         }
     }

     byte Menu::getMenuKey() {
        if (digitalRead(portKey0) == HIGH) return 1;
        if (digitalRead(portKey1) == HIGH) return 2;
        if (digitalRead(portKey2) == HIGH) return 3;
        return 0;
     }
      
   
