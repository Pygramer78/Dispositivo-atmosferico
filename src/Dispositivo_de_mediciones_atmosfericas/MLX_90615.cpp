#include "BetterMLX90615.h"
#include <Arduino.h>
MLX90615 mlx = MLX90615();
bool MLX90615_begin(void) {
  int id;
  mlx.begin();
  id = mlx.get_id();
  if (id == -1) {
    return false;
  }
  Serial.print("Sensor ID number = ");
  Serial.println(mlx.get_id(), HEX);
  return true;
}

void MLX90615_init(void) {
  Serial.println(F("Buscando MLX90615..."));
  if (MLX90615_begin()) {
    Serial.println(F("MLX90615 encontrado."));
  } else {
    Serial.println(F("MLX90615 no encontrado. Congelando."));
    while (true)
      ;
  }
}
// Realizar lecturas
void MLX90615_readObjectTemperature(void) {
  Serial.print("Object temperature: ");
  Serial.println(mlx.get_ambient_temp());
}

void MLX90615_readAmbientTemperature(void) {
  Serial.print("Ambient temperature: ");
  Serial.println(mlx.get_object_temp());
}