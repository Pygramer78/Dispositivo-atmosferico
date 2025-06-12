#ifndef AHT_20
#define AHT_20
#include <AHT20.h>
#include <Arduino.h>
extern AHT20 aht20;
void AHT20_init(void);
void AHT20_readTemperature(void);
void AHT20_readHumidity(void);
void AHT20_read(void);

#endif