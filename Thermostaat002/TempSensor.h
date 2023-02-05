
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

#include "arduino.h"
class TempSensor {
   public:
      void init(int aSensorPort);
      double calcTemp(int sensorValue);
      int meetTempSensor ();
      
   private:
      int sensorPort;
      
  
};
