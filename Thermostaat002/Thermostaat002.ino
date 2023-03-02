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

   2 maart 2023 11:00

   De controller in het toilet BLIJFT crashen, af en toe. Met als gevolg dat de kachel blijft branden. Ook is het relaisje
   kapot gegaan. Het relaisje is vervangen en inmiddels voorzien van een beschermend schakelingetje (een zogenaamd snubber circuit).
   
   Hoe ik het ook heb geprobeerd, met een hogere voedingsspanning, een tussengeschakelde arduino print (omdat die voorzien is
   van een voedingsstabilisator), een extra condensator van 1000 microFarad, het blijft af en toe crashen.
   Ik probeer het nu met de watchdog faciliteit waarmee de atmega standaard is uitgerust, zodat bij een crash het programma opnieuw opstart.

  Het is vreemd, want de controller die ik in de slaapkamer gebruikt is nog niet een keer gecrasht. 
  Misschien omdat ik daar van een atmega gebruik maak die nog op de arduino print zelf zit. Voor het overige zijn de schakelingen identiek of
  vrijwel identiek.
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

#define portBewegingssensor A2
// bewegingssensor heeft drie pootjes 
// om overspraak te voorkomen op A0, die een hele kleine stroom krijgt van de NTC weerstand (in orde van 200 kOhm)

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

#include "Watchdog.h"
Watchdog watchdog;

void welcome() {
   lcd.setCursor(0,0);
  lcd.print("thermostaat");
  lcd.setCursor(0, 1);
  lcd.print(" 002.001");
  lcd.setCursor(0,2);
  lcd.print("toilet");
  lcd.setCursor(0,3);
  lcd.print("controller");
  lcd.setCursor(0,4);
  lcd.print(":) :) :) ;)");
  watchdog.myDelay(2000); 
}




double avgTemp;
int nAvg;
// bij detecteren beweging verspringt de temperatuur.
// om dit te voorkomen hanteren we een voortschrijdend gemiddelde over de laatste 10 seconden
// avg = gemiddelde over laatste 10 perioden
// avg := (9 * avg + temp)/10
// eerste keer hanteren we als avgTemp de gewone meting

//------------------- setup -----------------------
void setup() {

  watchdog.init();
  
 
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(portLedOut, OUTPUT);
  pinMode(portRelais, OUTPUT);
  pinMode(portSensor, INPUT);
  digitalWrite(portSensor, LOW);
  pinMode(portBewegingssensor, INPUT);
  digitalWrite(portBewegingssensor, LOW);
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

  digitalWrite(portLedOut, HIGH);
  // rijtje menuknoppen zit precies andersom op de print
  mijnMenu.init(&lcd, &mijnThermos, portMenu2, portMenu1, portMenu0);

  nAvg = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("starting...");
  
}


boolean isLedOn = false;
  char sBuffer[30];  




// the loop function runs over and over again forever
//----------------------------------------------------
void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);

  
  int senseValue = mijnTempSensor.meetTempSensor();
  double temp = mijnTempSensor.calcTemp(senseValue);

  nAvg++;

  if (nAvg == 1) avgTemp = temp;
  avgTemp = (9.0 * avgTemp + temp ) / 10.0;
  temp = avgTemp;
  
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

  watchdog.myDelay(1000);

  watchdog.disable();
  mijnMenu.checkMenuKey();
  watchdog.enable();

  isLedOn = not(isLedOn);
  if (isLedOn) digitalWrite(portLedOut, HIGH);
  if (!isLedOn) digitalWrite(portLedOut, LOW);

  mijnClock.incSeconds(1);
  if (mijnClock.geefSeconden() %20 == 0) {
    boolean bSwitched = mijnThermos.checkSetSchema(mijnClock.geefTimerMinuten());
    if (bSwitched) {
        mijnClock.startTimer();
    }
  }

 
  
}
