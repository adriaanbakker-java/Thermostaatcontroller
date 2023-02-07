/*
  Dit is een testprogramma om de standaardprint te testen.
  Standaardprint bevat: 
   - NOKIA LCD 5110, contactbusje 8 pins, van rechts naar links zijn dit
     gnd, backlight (niet aangesloten), vcc, clk, din, dc, sce en rst
   - NTC weerstand - deze heeft zijn eigen instelparameters voor omzetten naar temp, contactbusje 2 pins
   - Drie menuknoppen, bovenaan print, van links naar rechts is dit menu2, menu1, menu0
   - LED die knippert op het ritme waarmee ook het relais wordt bediend, is vast
   - Relais dat een 220 volt aansluiting kan bedienen bij 10 A (Tongling, 5VDC), contactbusje 3 pins,
     links is aarde, midden is signaal, rechts is vcc, 
     pinnen links op relais zijn resp. boven links is vcc, midden is signaal, onder gnd
   - EEprom extern 32 kByte, contactbusje 4 pins, gnd is linker pin, busje is onderaan de print
     resp. gnd, vcc, sda, scl
   - contactbusje 5 polig voor uitlezen log en voor het herprogrammeren, van links naar rechts is dit
     rst, rx, tx, vcc, gnd
   - 2 pins contactbusje voor de voeding, onder is gnd, boven is vcc

   Zie voor de verdere beschrijving het document "temperatuur vastleggen met Arduino"

   Deze code geldt verder als basis voor de thermostaatcontroller die onder meer ingezet wordt in het toilet en
   als datalogger. Niet alle componenten zijn "verplicht":

   - Bij de datalogger zijn de NTC weerstand, menuknoppen en de eeprom component noodzakelijk, 
     rest is optioneel. Door bij een reboot (stroom aansluiten) de menuknop ingedrukt te houden wordt hetzij 
     gelogd naar seriele poort
   - Bij de thermostaatcontroller zijn de NTC weerstand, menuknoppen en het display noodzakelijk
   - contactbusjes voor de verschillende componenten, led en menuknoppen zijn uiteraard altijd aanwezig op de print
*/

#define portRelais  8
// pin 14 hoog is relais aan, relaispinnen bovenaanzicht pinnen links: boven is vcc, midden signaal, onder gnd

#define portMenu0  9
// pin 15  is menu0 via 10k aan knooppunt, switch tussen knooppunt en vcc, 10k tussen knooppunt en gnd 
#define portMenu1 10
// pin 16 analoog aan menu0
#define portMenu2 11
// pin 17 analoog aan menu0

#define portSensor A0

#define portBewegingssensor A1
// bewegingssensor heeft drie pootjes 

#define portLedOut 13
// poort 13 pin 19


#include "Kachel.h"
Kachel mijnKachel;

#include "lcd.h"

#include "TempSensor.h"
TempSensor mijnTempSensor;

#include "Clock.h"
Clock mijnClock;

#include "Thermos.h"
Thermos mijnThermos;

#include "Menu.h"
Menu mijnMenu;

void welcome() {
   lcd.setCursor(0,0);
  lcd.print("WELCOME ..");
  lcd.setCursor(0, 1);
  lcd.print(" adriaans");
  lcd.setCursor(0,2);
  lcd.print("testlcd");
  lcd.setCursor(0,3);
  lcd.print("controller");
  lcd.setCursor(0,4);
  lcd.print(":) :) :) ;)");
    delay(1000); 
}





//------------------- setup -----------------------
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(portLedOut, OUTPUT);
  pinMode(portRelais, OUTPUT);
  pinMode(portSensor, INPUT);

  pinMode(portMenu0, INPUT);
  pinMode(portMenu1, INPUT);
  pinMode(portMenu2, INPUT);
  
  Serial.begin(9600);
  Serial.println("Thermostaat toilet versie 002");

  lcdInit();
  welcome();
 
  mijnKachel.init(portRelais);
  mijnTempSensor.init(portSensor);

  mijnClock.resetClock();

  mijnThermos.init();
  
  lcd.clear();

  // rijtje menuknoppen zit precies andersom op de print
  mijnMenu.init(&lcd, &mijnThermos, portMenu2, portMenu1, portMenu0);
  

}



  char sBuffer[30];  

// the loop function runs over and over again forever
//----------------------------------------------------
void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);

  
  int senseValue = mijnTempSensor.meetTempSensor();
  double temp = mijnTempSensor.calcTemp(senseValue);
  lcd.print(temp);
  lcd.print("      ");
  lcd.print(senseValue);
 
  lcd.setCursor(0, 1);
  mijnClock.geefTijdstring(sBuffer);
  lcd.print(sBuffer);
  
  lcd.print("   ");

  if (mijnThermos.getHuidigSchemaNr() == huidigSchemaUitNr) {
      mijnKachel.zetKachelUit();
  } else {
      if (temp < mijnThermos.getTempAan(mijnThermos.getHuidigSchemaNr())) {
        mijnKachel.zetKachelAan();
      }
    
      if (temp > mijnThermos.getTempUit(mijnThermos.getHuidigSchemaNr())) {
        mijnKachel.zetKachelUit();
      }
  }
  
  if (mijnKachel.isKachelAan()) {
      lcd.print("AAN");
  } else {
      lcd.print("UIT");
  }

  



  lcd.setCursor(0, 3);
  mijnThermos.geefSchemaNaam(sBuffer, mijnThermos.getHuidigSchemaNr());
  lcd.print(sBuffer);
  lcd.print(" ");
  lcd.print(mijnClock.geefTimerMinuten());
  lcd.print(" ");
  int huidigSchemaNr = mijnThermos.getHuidigSchemaNr();
  lcd.print(mijnThermos.geefTimeout(huidigSchemaNr));

  lcd.setCursor(0, 4);
  lcd.print(mijnThermos.getTempAan(mijnThermos.getHuidigSchemaNr()));
  lcd.print(" ");
  lcd.print(mijnThermos.getTempUit(mijnThermos.getHuidigSchemaNr()));
  
  lcd.setCursor(0,5);
  lcd.print("1=menu");


  lcd.setCursor(0, 2);
  byte bewSensor = analogRead(portBewegingssensor);
  if (bewSensor > 20) {
      lcd.print("BEWEGING");
      mijnClock.startTimer();
      mijnThermos.setSchemaActief();
  };

  delay(1000);

  mijnMenu.checkMenuKey();

  mijnClock.incSeconds(1);
  if (mijnClock.geefSeconden() %20 == 0) {
    boolean bSwitched = mijnThermos.checkSetSchema(mijnClock.geefTimerMinuten());
    if (bSwitched) {
        mijnClock.startTimer();
    }
  }

 
  
}
