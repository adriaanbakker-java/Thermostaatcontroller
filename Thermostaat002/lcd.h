// LCD
#include <PCD8544.h> 
 PCD8544 lcd;


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




#define LCD_CMD 0 
const byte rst_pin = portLCD_rst;  // pin 1 LCD via 10k naar poort 6 arduino -- paars 
const byte sce_pin = portLCD_sce;  // pin 2 LCD via 1k naar poort 7 arduino-- grijs
const byte dc_pin  = portLCD_dc;  // pin 3 LCD via 10k naar poort 5 arduino -- blauw
const byte din_pin = portLCD_din;  // pin 4 LCD via 10k naar poort 4 arduino -- groen
const byte clk_pin = portLCD_clk;  // pin 5 LCD via 10k naar poort 3 arduino -- geel
//         vcc_pin          pin 6 LCD naar 3.3 V pin van arduino, GEEN 5 volt!!
//         led_pin          pin 7 via 330 ohm naar midden van 1k potmeter, laag is backlight aan
//         gnd_pin          pin 8 van LCD naar GND pin van arduino



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


void lcdInit() {
  lcd.begin(84, 48);
  LCDWrite( LCD_CMD, 0xBf ); 

}
