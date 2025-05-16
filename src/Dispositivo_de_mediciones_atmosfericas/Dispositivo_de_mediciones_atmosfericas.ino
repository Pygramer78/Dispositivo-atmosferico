/**********************************************************************************
Este es uno de los códigos fuentes de el dispositivo de mediciones atmosféricas.
Está especialmente usado para la presión, temperatura, humedad, x.

De Pygramer78[Álvaro](yo) y Sergio-dr(tú)
***********************************************************************************/

/***********************************************
Links de repositorios github para librerías:
=====================================================================================
AHT20: https://github.com/dvarrel/AHT20
TSL2591: https://github.com/adafruit/Adafruit_TSL2591_Library
BMP280: https://github.com/adafruit/Adafruit_BMP280_Library
MLX90615: https://github.com/Seeed-Studio/Digital_Infrared_Temperature_Sensor_MLX90615
======================================================================================
***********************************************/

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
