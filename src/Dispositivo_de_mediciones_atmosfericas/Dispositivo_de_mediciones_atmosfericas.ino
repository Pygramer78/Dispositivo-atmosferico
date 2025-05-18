/**********************************************************************************
Este es uno de los códigos fuentes de el dispositivo de mediciones atmosféricas.
Está especialmente usado para la presión, temperatura, humedad, x.

De Pygramer78[Álvaro](yo) y Sergio-dr(tú)
***********************************************************************************/

/* Links de repositorios github para librerías:
  AHT20: https://github.com/dvarrel/AHT20
  TSL2591: https://github.com/adafruit/Adafruit_TSL2591_Library
  BMP280: https://github.com/adafruit/Adafruit_BMP280_Library
  MLX90615: https://github.com/Seeed-Studio/Digital_Infrared_Temperature_Sensor_MLX90615
*/
#include <Wire.h>
#include <AHT20.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_TSL2591.h>
#include <MLX90615.h>

AHT20 aht20;
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);  // sensorID=2591
Adafruit_BMP280 bmp;
// TODO: MLX90615


/*__/ TSL2591 \________________*/

// Muestra información sobre el sensor
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
void BMP280_readTemperature(bool delay = false; int time = 2000;) {
  if (bmp.takeForcedMeasurement())  // Mira si el takeForcedMeasurement() => true o a 1
  {
    /************************************/
    // leer temperatura
    Serial.print(F("Temperature = "));    // Un serial print normal
    Serial.print(bmp.readTemperature());  // Hace serial print de la función readTemperature()
    Serial.println(" *C");                // otro serial print
    /************************************/
  }
  else {
      // Mensaje de error
      Serial.println("Medición ha fallado!");
      while(true);
  }
  if (delay) {
      delay(time);
  }
  
}

void BMP280_readPressure(bool delay = false; int time = 2000;) {
  if (bmp.takeForcedMeasurements()) {
    /********************************/
    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());  // devuelve la presión atmósférica
    Serial.println(" Pa");
    /********************************/
  }
  else {
    // Mensaje de error
    Serial.println("Medición ha fallado!");
    while(true);
  }
  if (delay) {
    delay(time);
  }
}

void BMP280_readAltitude(bool delay = false; int time = 2000;) 
{
  if (bmp.takeForcedMeasurements)
  {
    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(1013.25)); /* Ajustado en tu pronóstico local! */
    Serial.println(" m");
  }
  else {
    // Mensaje de error
    Serial.println("Medición ha fallado!");
    while(true);
  }
  if (delay) {
    delay(time);
  }
}

/*__/ AHT20 \________________*/
float temperature = aht20.getTemperature();
float humidity = aht20.getHumidity();
void AHT20_readTemperature(bool delay = false; int time = 2000;) 
{
  Serial.print("T: "); // Mucho más fácil que usar el BMP280, pero quizás menos completo
  Serial.print(temperature, 2); // Simplemente escribe la temperatura
  Serial.print(" C\t H: ");
}

void AHT20_readHumidity(bool delay = false; int time = 2000;)
{
  Serial.print("Humidity: ");
  Serial.print(humidity, 2);
  Serial.print(" % RH")
}
void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando...");

  // AHT20
  Wire.begin();
  Serial.println(F("Buscando AHT20..."));
  if (aht20.begin() == false) {
    Serial.println("No se ha detectado ningún sensor AHT20. Congelando.");
    while (true);
  }
  // TODO función similar a TSL2591_displayDetails() para AHT20?

  // BMP280
  Serial.println(F("Buscando BMP280..."));
  if (bmp.begin()) {
    Serial.println(F("Sensor BMP280 encontrado."));
  } else {
    Serial.println(F("No se ha detectado ningun sensor BMP280. Congelando."));
    while (true)
      ;
  }

  /* Las configuraciones por defecto del datasheet*/
  // Las he cogido del ejemplo bmp280_forced
  // TODO revisar opciones (Sergio) y convertir esto en una función BMP280_configure(), como con TSL2591
  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  // TSL2591
  Serial.println(F("Buscando TSL2591..."));
  if (tsl.begin()) {
    Serial.println(F("Sensor TSL2591 encontrado."));
  } else {
    Serial.println(F("No se ha detectado ningún sensor TSL2951. Congelando."));
    while (true)
      ;
  }
  TSL2591_displayDetails();
  TSL2591_configure();

  // TODO MLX90615
}

void loop() {
  // AHT20
  // TODO convertir esto en una función AHT20_read()
  temperature = aht20.getTemperature();  // Aunque ya hay un temperature en el bmp, este es más fácil de usar
  humidity = aht20.getHumidity();        // Este si o sí nos hace falta
  AHT20_readTemperature();
  AHT20_radHumidity();
  // Usando BMP280
  // TODO convertir esto en una función BMP280_read()
  // must call this to wake sensor up and get new measurement data
  // it blocks until measurement is complete
  // can now print out the new measurements
  // leer temperatura
  BMP280_readTemperature();
  // Leer Presión atmosférica
  BMP280_readPressure();
  // Leer altitud aproximada
  BMP280_readAltitude();
  // Párrafo
  Serial.println();
}
else {
  // Mensaje de error
  Serial.println("Medición ha fallado!");
}

// TSL2591
TSL2591_read();

// TODO Determinar delay adecuado
delay(2000);
}
