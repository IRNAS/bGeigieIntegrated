#if defined(ARDUINO) 
SYSTEM_MODE(MANUAL);//do not connect to cloud
#else
SYSTEM_MODE(AUTOMATIC);//connect to cloud
#endif
         
void setup()
{
  Serial.begin(9600); //  setup serial

  for(int i = 0; i < 5; i++)
  {
    Serial.println("ready");
    delay(1000);
  }
}

void loop()
{
  //// bat voltage
  Serial.println(((double)analogRead(A4) * 3.3 * 10.12)/4096.0);  // debug value
}
