#ifndef TSL_2951
#define TSL_2951
#include <Adafruit_TSL2591.h>
#include <Arduino.h>
extern Adafruit_TSL2591 tsl;
void TSL2591_init(void);
void TSL2591_displayDetails(void);
void TSL2591_configure(void);
void TSL2591_read(void);

#endif