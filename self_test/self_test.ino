#if defined(ARDUINO)
SYSTEM_MODE(MANUAL);//do not connect to cloud
#else
SYSTEM_MODE(AUTOMATIC);//connect to cloud
#endif

#include "Adafruit_GPS.h"
#include "application.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_LSM303.h"

#undef ARDUINO
#define PLATFORM_ID 3
#include "SdFat.h"

// Secondary SPI with DMA
// SCK => D4, MISO => D3, MOSI => D2, SS => D5
SdFat sd(1);
const uint8_t chipSelect = D5;

File myFile;
Adafruit_LSM303 lsm;

int IROVER = A2;
int BAT_MEAS = A4;
int ENC_SWITCH = A3;
int ENCODER_A = A6;
int ENCODER_B = A7;
int POWER_ON_SWITCH = D6;
int POWER_OFF_DETECT = A5;

#define OLED_RESET 7 // LCD_RST = D7
Adafruit_SSD1306 display(OLED_RESET);

static const unsigned char PROGMEM logo16_glcd_bmp[] =
{
  0x00, 0xC0,
  0x01, 0xC0,
  0x01, 0xC0,
  0x03, 0xE0,
  0xF3, 0xE0,
  0xFE, 0xF8,
  0x7E, 0xFF,
  0x33, 0x9F,
  0x1F, 0xFC,
  0x0D, 0x70,
  0x1B, 0xA0,
  0x3F, 0xE0,
  0x3F, 0xF0,
  0x7C, 0xF0,
  0x70, 0x70,
  0x00, 0x30
};

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void test_irover();
void i2c_scan();
void test_batt_volt_measure();
void test_encoder_switch_card_detect();
void test_encoder();
void test_gps();
void test_oled();
void test_sdcard();
void test_acc();
void test_sw_pwdwn();
void detect_power_switch_off();

void setup()
{
  Serial.begin(115200);

  pinMode(IROVER, OUTPUT);
  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);
  pinMode(POWER_ON_SWITCH, OUTPUT);
  pinMode(POWER_OFF_DETECT, INPUT);

  //OLED:

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();

  delay(1000);

  // Clear the buffer.
  display.clearDisplay();

  //END OLED
}

void loop()
{
  test_irover();
  test_sw_pwdwn();
  i2c_scan();
  test_batt_volt_measure();
  test_encoder_switch_card_detect();
  test_encoder();
  test_gps();
  test_oled();
  test_sdcard();
  detect_power_switch_off();
  //test_acc();
}

void test_irover()
{
  for (int i = 0; i < 3; i++)
  {
    Serial.println("\ntest iRover:");

    digitalWrite(IROVER, HIGH);

    delay(1000);

    digitalWrite(IROVER, LOW);

    delay(1000);
  }
}

void i2c_scan()
{
  Serial.println("\nI2C Scanner:");

  Wire.begin();
  delay(1000);

  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;

  for (address = 0; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    //Serial.println(address);

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknow error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(1000);
}

void test_batt_volt_measure()
{
  Serial.println("\nmeasure battery voltage:");
  delay(1000);

  for (int i = 0; i < 50; i++)
  {
    //// bat voltage
    Serial.println(((double)analogRead(BAT_MEAS) * 3.3 * 10.12) / 4096.0); // debug value
  }
}

void test_encoder_switch_card_detect()
{
  Serial.println("\ntest encoder switch:");
  Serial.println("press encoder button!");
  delay(1000);

  if (((double)analogRead(ENC_SWITCH) * 3.3) / 4096.0 > 1.2) Serial.println("SD card inserted!");
  else Serial.println("SD card not detected!");

  for (int i = 0; i < 20; i++)
  {
    //// ENC_SW
    Serial.println(((double)analogRead(ENC_SWITCH) * 3.3) / 4096.0); // debug value
    delay(1000);
  }
}

void test_encoder()
{
  int encoder0Pos = 0;
  bool encoder0PinALast = LOW;
  bool n = LOW;

  Serial.println("\ntest encoder:");
  Serial.println("rotate encoder!");
  Serial.println("Press encoder button to continue!");
  delay(1000);

  while (true)
  {
    n = digitalRead(ENCODER_A);
    if ((encoder0PinALast == LOW) && (n == HIGH))
    {
      if (digitalRead(ENCODER_B) == LOW)
      {
        encoder0Pos--;
      }
      else
      {
        encoder0Pos++;
      }
      Serial.print (encoder0Pos);
      Serial.print ("/");
    }
    encoder0PinALast = n;

    if (((double)analogRead(ENC_SWITCH) * 3.3) / 4096.0 < 0.5) break;
  }
}

void test_gps()
{
  Serial.println("\ntest GPS:");
  Serial.println("Adafruit GPS library basic test!");
  Serial.println("Press encoder button to continue!");

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

  while (true)
  {
    //Serial.println("reading");
    //Serial.print(GPS.read());
    if (Serial2.available())
    {
      char c = Serial2.read();
      if (c) Serial.print(c);
    }

    if (((double)analogRead(ENC_SWITCH) * 3.3) / 4096.0 < 0.5) break;
  }
}

void test_oled()
{
  Serial.println("\ntest OLED:");

  // Clear the buffer.
  display.clearDisplay();

  // draw the first ~12 characters in the font

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  for (uint8_t i = 0; i < 168; i++)
  {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }
  display.display();
  delay(1);

  display.display();
  delay(2000);
  display.clearDisplay();
}

void test_sdcard()
{
  Serial.println("\ntest SD card:");

  // Initialize SdFat or print a detailed error message and halt
  // Use half speed like the native library.
  // Change to SPI_FULL_SPEED for more performance.
  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) {
    sd.initErrorHalt();
  }

  // open the file for write at end like the "Native SD library"
  if (!myFile.open("test.txt", O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("opening test.txt for write failed");
  }
  // if the file opened okay, write to it:
  Serial.print("Writing to test.txt...");
  myFile.println("testing 1, 2, 3.");
  myFile.printf("fileSize: %d\n", myFile.fileSize());

  // close the file:
  myFile.close();
  Serial.println("done.");

  // re-open the file for reading:
  if (!myFile.open("test.txt", O_READ)) {
    sd.errorHalt("opening test.txt for read failed");
  }
  Serial.println("test.txt content:");

  // read from the file until there's nothing else in it:
  int data;
  while ((data = myFile.read()) >= 0) {
    Serial.write(data);
  }
  // close the file:
  myFile.close();
}

void test_acc()
{
  Serial.println("\ntest accelerometer:");

  // Try to initialize and warn if we couldn't detect the chip
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

void test_sw_pwdwn()
{
  // bridge the power off switch
  digitalWrite(POWER_ON_SWITCH, HIGH);
  Serial.println("\ntest software power-down:");
  Serial.println("Power off switch bridged!");
}

void detect_power_switch_off()
{
  if (digitalRead(POWER_OFF_DETECT)) Serial.println("\nPower switch turned off!");
}

