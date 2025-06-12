#ifndef BMP_280
#define BMP_280
#include <Adafruit_BMP280.h>
#include <Arduino.h>
extern Adafruit_BMP280 bmp;
void BMP280_init(void);
void BMP280_configure(void); /*./BMP_280.cpp*/
void BMP280_readTemperature(void);
void BMP280_readPressure(void);
void BMP280_readAltitude(void);
#endif