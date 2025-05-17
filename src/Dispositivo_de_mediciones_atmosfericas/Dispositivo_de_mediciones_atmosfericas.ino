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
#include <Wire.h> // <-- Probablemente haga falta (sale en el ejemplo de AHT20)
#include <AHT20.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_TSL2591.h> // <-- 
#include <MLX90615.h> // <-- infrarrojo
AHT20 aht20;
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // El número aparentemente es el pin que hay que poner
Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor;
  tsl.getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" lux"));
  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" lux"));
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution, 4); Serial.println(F(" lux"));  
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
  delay(500);
}

/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2591
*/
/**************************************************************************/
void configureSensor(void)
{
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  //tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
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
  Serial.println(F("------------------------------------"));
  Serial.print  (F("Gain:         "));
  tsl2591Gain_t gain = tsl.getGain();
  switch(gain)
  {
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
  Serial.print  (F("Timing:       "));
  Serial.print((tsl.getTiming() + 1) * 100, DEC); 
  Serial.println(F(" ms"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
}


/**************************************************************************/
/*
    Program entry point for the Arduino sketch
*/
/**************************************************************************/
void setup(void) 
{
  Serial.begin(9600);
  
  Serial.println(F("Starting Adafruit TSL2591 Test!"));
  
  if (tsl.begin()) 
  {
    Serial.println(F("Found a TSL2591 sensor"));
  } 
  else 
  {
    Serial.println(F("No sensor found ... check your wiring?"));
    while (1);
  }
    
  /* Display some basic information on this sensor */
  displaySensorDetails();
  
  /* Configure the sensor */
  configureSensor();

  // Now we're ready to get readings ... move on to loop()!
}
/*********************************TSL2591*Functions***************************************/
/**************************************************************************/
/*
    Shows how to perform a basic read on visible, full spectrum or
    infrared light (returns raw 16-bit ADC values)
*/
/**************************************************************************/
void simpleRead(void)
{
  // Simple data read example. Just read the infrared, fullspecrtrum diode 
  // or 'visible' (difference between the two) channels.
  // This can take 100-600 milliseconds! Uncomment whichever of the following you want to read
  uint16_t x = tsl.getLuminosity(TSL2591_VISIBLE);
  //uint16_t x = tsl.getLuminosity(TSL2591_FULLSPECTRUM);
  //uint16_t x = tsl.getLuminosity(TSL2591_INFRARED);

  Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
  Serial.print(F("Luminosity: "));
  Serial.println(x, DEC);
}

/**************************************************************************/
/*
    Show how to read IR and Full Spectrum at once and convert to lux
*/
/**************************************************************************/
void advancedRead(void)
{
  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
  Serial.print(F("IR: ")); Serial.print(ir);  Serial.print(F("  "));
  Serial.print(F("Full: ")); Serial.print(full); Serial.print(F("  "));
  Serial.print(F("Visible: ")); Serial.print(full - ir); Serial.print(F("  "));
  Serial.print(F("Lux: ")); Serial.println(tsl.calculateLux(full, ir), 6);
}

/**************************************************************************/
/*
    Performs a read using the Adafruit Unified Sensor API.
*/
/**************************************************************************/
void unifiedSensorAPIRead(void)
{
  /* Get a new sensor event */ 
  sensors_event_t event;
  tsl.getEvent(&event);
 
  /* Display the results (light is measured in lux) */
  Serial.print(F("[ ")); Serial.print(event.timestamp); Serial.print(F(" ms ] "));
  if ((event.light == 0) |
      (event.light > 4294966000.0) | 
      (event.light <-4294966000.0))
  {
    /* If event.light = 0 lux the sensor is probably saturated */
    /* and no reliable data could be generated! */
    /* if event.light is +/- 4294967040 there was a float over/underflow */
    Serial.println(F("Datos inválidos (ajusta el gain o el timing)"));
  }
  else
  {
    Serial.print(event.light); Serial.println(F(" lux"));
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Ready");
  // AHT20
  Wire.begin();
  if (aht40.begin() == false) {
    Serial.println("El módulo AHT20 no ha sido detectado. Por favor mira el cableado. Congelando");
    while(true);
  }
  // TSL2591
  Serial.println(F("Arrancando Adafruit TSL2591!"));
  
  if (tsl.begin()) 
  {
    Serial.println(F("El sensor TSL2591 fue encontrado"));
  } 
  else 
  {
    Serial.println(F("No se ha encontrado ningún sensor TSL2951...? Procediendo a congelar"));
    while (1);
  }
  // BMP280
  // Asegurarte de que el sensor BMP280 funcione con este código
  //if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
  if (!bmp.begin()) 
  {
    Serial.println(F("No se ha podido encontrar un sensor BMP280, asegúrate que el cableado esté bien "
                      "prueba otro pin!"));
    while(1) delay(10); 
  }

  /* Las configuraciones por defecto de la datasheet*/
  // Las he cogido del ejemplo bmp280_forced
  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop() {
  /*******************************************/
  // TSL2591
  // unifiedSensorAPIRead();
  // advancedRead();
  // simpleRead();
  /*******************************************/
  // Usando aht20
  /***********************************
  float temperature = aht20.getTemperature(); // Aunque ya hay un temperature en el bmp, este es más fácil de usar
  float humidity = aht20.getHumidity(); // Este si o sí nos hace falta
  Serial.print("T: ");
  Serial.print(temperature, 2);
  Serial.print(" C\t H: ");
  Serial.print(humidity, 2);
  Serial.println("% RH");
  delay(2000);
  ***********************************/
  // Usando BMP280
    // must call this to wake sensor up and get new measurement data
  // it blocks until measurement is complete
  if (bmp.takeForcedMeasurement()) {
    // can now print out the new measurements
    /************************************/
    // leer temperatura
    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    /************************************/
    // Leer Presión atmosférica
    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
    /*************************************/
    // Leer altitud aproximada
    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(1013.25)); /* Ajustado en tu pronóstico local! */
    Serial.println(" m");
    /*************************************/
    Serial.println();
    delay(2000);
  } else {
    // Mensaje de error
    Serial.println("Medición forzada ha fallado!");
  }

}
