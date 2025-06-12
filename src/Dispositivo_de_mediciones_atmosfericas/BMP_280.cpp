#include <Adafruit_BMP280.h>
#include <Arduino.h>
#include "BetterBMP280.h"
Adafruit_BMP280 bmp;

void BMP280_init(void) {
  Serial.println(F("Buscando BMP280..."));
  if (bmp.begin()) {
    Serial.println(F("BMP280 encontrado."));
  } else {
    Serial.println(F("BMP280 no encontrado. Congelando."));
    while (true)
      ;
  }
}

void BMP280_configure(void) {
  bmp.setSampling(  //Adafruit_BMP280::MODE_NORMAL,  /* Single measurement, then sleep */
    Adafruit_BMP280::MODE_FORCED,
    Adafruit_BMP280::SAMPLING_X2,  /* Temp. oversampling (eq. to 17-bit resolution, see Table 5) */
    Adafruit_BMP280::SAMPLING_X16, /* Pressure oversampling (eq to 20-bit resolution, see Table 4)*/
    Adafruit_BMP280::FILTER_X4);   /* IIR filtering */
}

void BMP280_readTemperature(void) {
  if (bmp.takeForcedMeasurement())  // despertar al sensor
  {
    // leer temperatura
    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
  } else {
    // Mensaje de error
    Serial.println("Medición ha fallado!");
    while (true)
      ;
  }
}

void BMP280_readPressure(void) {
  if (bmp.takeForcedMeasurement()) {  // despertar al sensor
    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
  } else {
    // Mensaje de error
    Serial.println("Medición ha fallado!");
    while (true)
      ;
  }
}

void BMP280_readAltitude(void) {
  if (bmp.takeForcedMeasurement()) {
    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(1013.25));  // TODO indicar valor actual en hPa a nivel del mar
    Serial.println(" m");
  } else {
    // Mensaje de error
    Serial.println("Medición ha fallado!");
    while (true)
      ;
  }
}