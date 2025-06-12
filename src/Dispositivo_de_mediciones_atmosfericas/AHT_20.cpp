#include "BetterAHT20.h"
#include <AHT20.h>
AHT20 aht20;

void AHT20_init(void) {
  Wire.begin();
  Serial.println(F("Buscando AHT20..."));
  if (aht20.begin()) {
    Serial.println("AHT20 encontrado!");
  } else {
    Serial.println("AHT20 no encontrado, congelando...");
    while (true)
      ;
  }
}

void AHT20_readTemperature(void) {
  float temperature = aht20.getTemperature();
  Serial.print("T: ");
  Serial.print(temperature, 2);
  Serial.print(" *C");
}

void AHT20_readHumidity(void) {
  float humidity = aht20.getHumidity();
  Serial.print("Humidity: ");
  Serial.print(humidity, 2);
  Serial.print(" %");
}

void AHT20_read(void) {
  AHT20_readTemperature();
  AHT20_readHumidity();
}