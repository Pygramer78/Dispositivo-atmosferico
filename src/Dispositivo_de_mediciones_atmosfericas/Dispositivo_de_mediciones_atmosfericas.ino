/*__/ Declaraciones \________________*/
/**********************************************************************************
Este es uno de los códigos fuentes de el dispositivo de mediciones atmosféricas.
Está especialmente usado para la presión, temperatura, humedad, x.

De Pygramer78[Álvaro](yo) y Sergio-dr(tú)
***********************************************************************************/

/* Links de repositorios github para librerías:
  AHT20: https://github.com/dvarrel/AHT20
  TSL2591: https://github.com/adafruit/Adafruit_TSL2591_Library
  BMP280: https://github.com/adafruit/Adafruit_BMP280_Library
  MLX90615: https://github.com/skiselev/MLX90615
*/
#include <Wire.h>
#include "BetterBMP280.h"
#include "BetterAHT20.h"
#include "BetterMLX90615.h"
#include "BetterTSL2951.h"

/*__/ Programa principal \________________*/

void setup() {
  Serial.begin(19200);
  Serial.println("Iniciando...");
  // AHT20_init();
  // BMP280
  BMP280_init();
  BMP280_configure();  // Config en línea 169
  // Valores basados en "Table 7. Use case: handheld device low-power" del datasheet del BMP280


  // TSL2591
  /*
  TSL2591_init();
  TSL2591_displayDetails();
  TSL2591_configure();
  */

  // MLX90615
  MLX90615_init();
}

void loop() {
  // AHT20
  /*
  AHT20_readTemperature();
  AHT20_readHumidity();
  */

  // BMP280
  BMP280_readTemperature();
  BMP280_readPressure();
  BMP280_readAltitude();
  Serial.println();

  // TSL2591
  //TSL2591_read();

  // MLX90615
  // MLX90615_readObjectTemperature();
  // MLX90615_readAmbientTemperature();

  // TODO Determinar delay adecuado
  delay(2000);
}
