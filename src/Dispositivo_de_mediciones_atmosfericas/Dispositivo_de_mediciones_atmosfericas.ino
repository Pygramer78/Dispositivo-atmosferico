/**********************************************************************************
Este es uno de los códigos fuentes de el dispositivo de mediciones atmosféricas.
Está especialmente usado para la presión, temperatura, humedad, x.

De Pygramer78[Álvaro](yo) y Sergio-dr(tú)
***********************************************************************************/


#include <AHT20.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_TSL2591.h>
#include <I2cMaster.h> // <-- Estas dos librerías son las mismas
#include <MLX90615.h> // <--


void setup() {
  Serial.begin(9600);
  Serial.println("Ready");
}

void loop() {
  

}
