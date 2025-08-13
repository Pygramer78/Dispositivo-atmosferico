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
#include <Arduino.h>
#include <Wire.h>
#include <AHT20.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_TSL2591.h>
#include <mlx90615.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_2_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

AHT20 aht20;
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);  // sensorID=2591
Adafruit_BMP280 bmp;
MLX90615 mlx = MLX90615();


char temp[] = { "--.-" };
char pa[] = { "----.--" };
char altitude[] = { "----.--" };
char hum[] = { "--.-" };
/*__/ SOME STUFF \_____________*/

void wait(int seconds) {
  delay(seconds * 1000);
}

void writeAndWait(String newBuffer, int seconds) {
  u8g2.clearBuffer();
  u8g2.drawStr(0, 10, newBuffer);
  u8g2.sendBuffer();
  wait(seconds);
}

/*__/ MLX90615 \________________*/
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

/*__/ TSL2591 \________________*/

// Iniciar
void TSL2591_init(void) {
  Serial.println(F("Buscando TSL2591..."));
  if (tsl.begin()) {
    Serial.println(F("TSL2591 encontrado."));
  } else {
    Serial.println(F("TSL2951 no encontrado. Congelando."));
    while (true)
      ;
  }
}

// Mostrar información sobre el sensor
void TSL2591_displayDetails(void) {
  sensor_t sensor;
  tsl.getSensor(&sensor);
  Serial.print(F("Sensor:       "));
  Serial.println(sensor.name);
  Serial.print(F("Driver Ver:   "));
  Serial.println(sensor.version);
  Serial.print(F("Unique ID:    "));
  Serial.println(sensor.sensor_id);
  Serial.print(F("Max Value:    "));
  Serial.print(sensor.max_value);
  Serial.println(F(" lux"));
  Serial.print(F("Min Value:    "));
  Serial.print(sensor.min_value);
  Serial.println(F(" lux"));
  Serial.print(F("Resolution:   "));
  Serial.print(sensor.resolution, 4);
  Serial.println(F(" lux"));
  Serial.println(F(""));
}

// Configura ganancia y exposición
// TODO determinar configuración óptima para medición de brillo de cielo nocturno
void TSL2591_configure(void) {
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  //tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  tsl.setGain(TSL2591_GAIN_MED);  // 25x gain
  //tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain

  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)

  /* Display the gain and integration time for reference sake */
  Serial.print(F("Gain:         "));
  tsl2591Gain_t gain = tsl.getGain();
  switch (gain) {
    case TSL2591_GAIN_LOW:
      Serial.println(F("1x (Low)"));
      break;
    case TSL2591_GAIN_MED:
      Serial.println(F("25x (Medium)"));
      break;
    case TSL2591_GAIN_HIGH:
      Serial.println(F("428x (High)"));
      break;
    case TSL2591_GAIN_MAX:
      Serial.println(F("9876x (Max)"));
      break;
  }
  Serial.print(F("Timing:       "));
  Serial.print((tsl.getTiming() + 1) * 100, DEC);
  Serial.println(F(" ms"));
  Serial.println(F(""));
}

// Realizar lectura del sensor
// TODO convertir a mag/arcsec^2 y devolver el valor
void TSL2591_read(void) {
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  Serial.print(F("[ "));
  Serial.print(millis());
  Serial.print(F(" ms ] "));
  Serial.print(F("IR: "));
  Serial.print(ir);
  Serial.print(F("  "));
  Serial.print(F("Full: "));
  Serial.print(full);
  Serial.print(F("  "));
  Serial.print(F("Visible: "));
  Serial.print(full - ir);
  Serial.print(F("  "));
  Serial.print(F("Lux: "));
  Serial.println(tsl.calculateLux(full, ir), 6);
}

/*__/ BMP280 \________________*/
void BMP280_init(void) {
  Serial.println(F("Buscando BMP280..."));
  if (bmp.begin()) {
    Serial.println(F("BMP280 encontrado."));
    writeAndWait("BMP280 encontrado.", 1);
  } else {
    Serial.println(F("BMP280 no encontrado. Congelando."));
    writeAndWait("BMP280 no encontrado. Congelando.", 10);
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
    String newBuffer = "Temperature = " + dtostfr(bmp.readTemperature(), 2, 1, temp) + " *C";
    writeAndWait(newBuffer, 3);
  } else {
    // Mensaje de error
    Serial.println("Medición ha fallado!");
    writeAndWait("Medición ha fallado!", 10);
    while (true)
      ;
  }
}

void BMP280_readPressure(void) {
  if (bmp.takeForcedMeasurement()) {  // despertar al sensor
    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
    String newBuffer = "Pressure = " + dtostrf(bmp.readPressure(), 4, 2, pa) + " Pa";
    writeAndWait(newBuffer, 3);
  } else {
    // Mensaje de error
    Serial.println("Medición ha fallado!");
    writeAndWait("Medición ha fallado!", 10);
    while (true)
      ;
  }
}

void BMP280_readAltitude(void) {
  if (bmp.takeForcedMeasurement()) {
    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(1013.25));  // TODO indicar valor actual en hPa a nivel del mar
    Serial.println(" m");
    String newBuffer = "Altitud Aprox = " + dtostrf(bmp.readAltitude(1013.25), 4, 2, altitude) + " m";
    writeAndWait(newBuffer, 3);
  } else {
    // Mensaje de error
    Serial.println("Medición ha fallado!");
    writeAndWait("Medición ha fallado", 10);
    while (true)
      ;
  }
}

/*__/ AHT20 \________________*/


void AHT20_init(void) {
  Wire.begin();
  Serial.println(F("Buscando AHT20..."));
  if (aht20.begin()) {
    Serial.println("AHT20 encontrado!");
    writeAndWait("AHT20 encontrado", 1);
  } else {
    Serial.println("AHT20 no encontrado, congelando...");
    writeAndWait("AHT20 no encontrado, congelando", 10);
    while (true)
      ;
  }
}

void AHT20_readTemperature(void) {
  float temperature = aht20.getTemperature();
  Serial.print("T: ");
  Serial.print(temperature, 2);
  Serial.print(" *C");
  String newBuffer = "T: " + dtostrf(temperature, 3, 1, temp) + " *C";
  writeAndWait(newBuffer, 3);
}

void AHT20_readHumidity(void) {
  float humidity = aht20.getHumidity();
  Serial.print("Humidity: ");
  Serial.print(humidity, 2);
  Serial.print(" %");
  String newBuffer = "Humidity: " + dtostrf(humidity, 2, 1, hum) + " %";
  writeAndWait(newBuffer, 3);
}

void AHT20_read(void) {
  AHT20_readTemperature();
  AHT20_readHumidity();
}
/*__/ Programa principal \________________*/

void setup() {
  Serial.begin(19200);
  Serial.print("Inicializando...");
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr); // TODO mirar si hay más Fonts
  writeAndWait("Inicializando...", 1);
  AHT20_init();
  // BMP280
  BMP280_init();
  BMP280_configure();  // Config en línea 169
  // Valores basados en "Table 7. Use case: handheld device low-power" del datasheet del BMP280

  // TODO buscar otro sensor de luz
  // TSL2591
  TSL2591_init();
  TSL2591_displayDetails();
  TSL2591_configure();

  // MLX90615
  MLX90615_init();
}

void loop() {
  u8g2.clearBuffer();
  // AHT20
  AHT20_readTemperature();
  AHT20_readHumidity();

  // BMP280
  BMP280_readTemperature();
  BMP280_readPressure();
  BMP280_readAltitude();
  Serial.println();

  // TSL2591
  TSL2591_read();

  // TODO cambiar por MLX90614
  // MLX90615
  MLX90615_readObjectTemperature();
  MLX90615_readAmbientTemperature();

  // TODO Determinar delay adecuado
  wait(2);
  u8g2.clearBuffer();
}
