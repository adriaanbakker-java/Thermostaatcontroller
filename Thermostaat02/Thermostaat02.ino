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
// aan de NTC weerstand knooppunt 200 kOhm spanningsdeler, NTC zit aan de plus. 200 kOhm aan de gnd
// analoge poort A0, fysieke pin 23, verbonden met NTC weerstand

// Deze specifieke NTC heeft bij kamertemperatuur een weerstand van ongeveer 200K
// Voor de beste resultaten moet het bereik van de NTC weerstand ervoor zorgend dat we ergens tussen de 
// 0 en de 5 volt uitkomen. Bij 30 graden zitten we rond 150 kOhm. Bij 6 graden naar verwachting ergens
// rond de 300 ohm. Beste waarde voor de andere weerstand in deze spanningsdeler
// is ergens rond de 200K om een maximale resolutie te krijgen zijnde 175 stapjes van de 1024 stapjes in
// de AD meting van de analoge poort:
// bij 20 graden verschil, ongeveer 7 stapjes per graad Celsius, dus 0,14 graden per stapje. 
// Wil je hogere resolutie dan zou je moeten versterken via een transistor.


#define portBewegingssensor A1
// bewegingssensor heeft drie pootjes 

// NOKIA LCD 5110
// van rechts naar links achterzijde met pinrij lcd boven
// via weerstanden is de lcd beschermd, level shifter daardoor niet nodig

//                   pin 1 lcd -      ground
// .                 pin 2 lcd  -     backlight, via 330 ohm naar vcc
//                   pin 3 lcd  -     vcc (5 volt)
// poort 3 / pin 5   pin 4 lcd  -     clk - via 10k
// poort 4 / pin 6   pin 5 lcd -      din - via 10k
// poort 5 / pin 11  pin 6 lcd -      dc - via 10k
// poort 7 / pin 13  pin 7 lcd -      sce - via 1k -- let op sce gaat naar poort 7 pin 13, niet naar poort 6 
// poort 6 / pin 12  pin 8 lcd -      rst - via 10k 
// 

#define portLCD_clk 3
#define portLCD_din 4
#define portLCD_dc  5
#define portLCD_sce 7
#define portLCD_rst 6

//AT24C256 driver CPP file
#include "Arduino.h"
#include "Wire.h"
#include "AT24C256.h"

#define LCD_CMD 0 
const byte rst_pin = portLCD_rst;  // pin 1 LCD via 10k naar poort 6 arduino -- paars 
const byte sce_pin = portLCD_sce;  // pin 2 LCD via 1k naar poort 7 arduino-- grijs
const byte dc_pin  = portLCD_dc;  // pin 3 LCD via 10k naar poort 5 arduino -- blauw
const byte din_pin = portLCD_din;  // pin 4 LCD via 10k naar poort 4 arduino -- groen
const byte clk_pin = portLCD_clk;  // pin 5 LCD via 10k naar poort 3 arduino -- geel
//         vcc_pin          pin 6 LCD naar 3.3 V pin van arduino, GEEN 5 volt!!
//         led_pin          pin 7 via 330 ohm naar midden van 1k potmeter, laag is backlight aan
//         gnd_pin          pin 8 van LCD naar GND pin van arduino



#define portLedOut 13
// poort 13 pin 19

// i2c eeprom componentje heeft 4 pootjes
// 1 = gnd
// 2 = vcc
// 3 = sda - fysieke pin 27 atmega
// 4 = scl - fysieke pin 28 atmega



AT24C256 eeprom(0x50, &Wire);

#define CAANTAL 10 
// aantal cellen van eeprom voor het testen

void printArray(byte aData[]) {
    for (int i = 1; i<=CAANTAL; i++) {
      Serial.print(i);
      Serial.print(":");
      Serial.print(aData[i-1]);  
      Serial.print(",");
    }
    Serial.println();
}

// LCD
#include <PCD8544.h> 
 PCD8544 lcd;

#include "Kachel.h"
Kachel kachel; 

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
}

double calcTemp(int sensorValue) {
 int tot = sensorValue;
 double Rnc = (1023.00 -tot)/tot *100;
    double lnRnc = log(Rnc);
    
// 4 feb 2023   567 komt overeen met 17,8 graden
//              421 komt overeen met  7,1 graden
    double B =   4385.497029;
    double lnA = log(0.0000229); 
    
    double TKelvin = B / (lnRnc - lnA );
    return TKelvin - 273.15;
}

int meetTempSensor () {
    int val;
  double tot = 0;
  int nrtimes = 20;
  for (int i = 1; i <= nrtimes; i++) {
       val = analogRead(portSensor);
      delayMicroseconds(200);
      val = analogRead(portSensor);
      Serial.print(val);
      Serial.print(' ');
      tot += val;
    }
    Serial.println();
    tot = tot/nrtimes;

    return tot;
}


//There are two memory banks in the LCD, data/RAM and commands. This 
//function sets the DC pin high or low depending, and then sends
//the data byte
void LCDWrite(byte data_or_command, byte data) {
  digitalWrite(dc_pin, data_or_command); //Tell the LCD that we are writing either to data or a command

  //Send the data
  digitalWrite(sce_pin, LOW);
  shiftOut(din_pin, clk_pin, MSBFIRST, data);
  digitalWrite(sce_pin, HIGH);
}


//------------ checkEeprom() ----------------
void checkEeprom() {
  byte data[CAANTAL];
   
    // alles op nul zetten
  for (int i = 1; i<=CAANTAL; i++) {
      data[i-1] = i;  
  }
  Serial.println("schrijven:");
  eeprom.write(0, (uint8_t*)data, sizeof(data));
  delay(100);
  printArray(data);
  
  // alles op nul zetten
  for (int i = 1; i<=CAANTAL; i++) {
      data[i-1] = 0;  
  }

  Serial.println("teruglezen:");
  eeprom.read(0, (uint8_t*)data, sizeof(data));
  delay(100);

  printArray(data);
  }

int i;
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
    Serial.println("LCD testprogramma testlcd NOKIA 5110..");

  delay(1000);
  lcd.begin(84, 48);
  LCDWrite( LCD_CMD, 0xBf ); 
  welcome();

  
  
  eeprom.begin();
  checkEeprom();
 
  kachel.init(portRelais);

  i=0;
  lcd.clear();
}




// the loop function runs over and over again forever
//----------------------------------------------------
void loop() {
  i = i+1;
  i = i%20;
  lcd.setCursor(0, 0);
  lcd.print("---------");
  lcd.print(i);
  lcd.print(" ");

  int senseValue = meetTempSensor();

  lcd.setCursor(0, 1);
  if (i>10) {
      lcd.print("ON ");
      kachel.zetKachelAan();
  } else {
      lcd.print("OFF");
      kachel.zetKachelUit();
  }
  lcd.setCursor(0, 2);
  lcd.print(senseValue);
  lcd.print(" ");
  double temp = calcTemp(senseValue);
  lcd.print(temp);
  delay(1000);
}
