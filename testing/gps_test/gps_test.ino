#if defined(ARDUINO)
SYSTEM_MODE(MANUAL);//do not connect to cloud
#else
SYSTEM_MODE(AUTOMATIC);//connect to cloud
#endif

#include "Adafruit_GPS.h"
#include "application.h"

void setup()
{
  // connect at 115200 so we can read the GPS fast enuf and
  // also spit it out
  Serial.begin(115200);

  for (int i = 0; i < 10; i++)
  {
    Serial.println("test");
    delay(1000);
  }

  Serial.println("Adafruit GPS library basic test!");

  Serial2.begin(9600);

  Adafruit_GPS GPS(&Serial2);

  // 9600 NMEA is the default baud rate for MTK - some use 4800
  GPS.begin(9600);

  // You can adjust which sentences to have the module emit, below

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data for high update rates!
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // uncomment this line to turn on all the available data - for 9600 baud you'll want 1 Hz rate
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_ALLDATA);

  // Set the update rate
  // Note you must send both commands below to change both the output rate (how often the position
  // is written to the serial line), and the position fix rate.
  // 1 Hz update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PMTK_API_SET_FIX_CTL_1HZ);
  // 5 Hz update rate- for 9600 baud you'll have to set the output to RMC or RMCGGA only (see above)
  //GPS.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);
  //GPS.sendCommand(PMTK_API_SET_FIX_CTL_5HZ);
  // 10 Hz update rate - for 9600 baud you'll have to set the output to RMC only (see above)
  // Note the position can only be updated at most 5 times a second so it will lag behind serial output.
  //GPS.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);
  //GPS.sendCommand(PMTK_API_SET_FIX_CTL_5HZ);

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);
}

void loop() // run over and over again
{
  //Serial.println("reading");
  //Serial.print(GPS.read());
  if (Serial2.available())
  {
    char c = Serial2.read();
    if (c) Serial.print(c);
  }
}
