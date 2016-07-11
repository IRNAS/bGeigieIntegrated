#if defined(ARDUINO)
SYSTEM_MODE(MANUAL);//do not connect to cloud
#else
SYSTEM_MODE(AUTOMATIC);//connect to cloud
#endif

//#include <Wire.h>
#include "application.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_LSM303.h"

Adafruit_LSM303 lsm;

void setup()
{
  Serial.begin(9600);

  for (int i = 0; i < 10; i++)
  {
    Serial.println("ready");
    delay(1000);
  }
}

void loop()
{
  // Try to initialise and warn if we couldn't detect the chip
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM303. Check your wiring!");
  }

  Serial.println("test");
  lsm.read();
  Serial.print("Accel X: "); Serial.print((int)lsm.accelData.x); Serial.print(" ");
  Serial.print("Y: "); Serial.print((int)lsm.accelData.y);       Serial.print(" ");
  Serial.print("Z: "); Serial.println((int)lsm.accelData.z);     Serial.print(" ");
  Serial.print("Mag X: "); Serial.print((int)lsm.magData.x);     Serial.print(" ");
  Serial.print("Y: "); Serial.print((int)lsm.magData.y);         Serial.print(" ");
  Serial.print("Z: "); Serial.println((int)lsm.magData.z);       Serial.print(" ");
  delay(100);
}
