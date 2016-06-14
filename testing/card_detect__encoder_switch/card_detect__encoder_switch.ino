#if defined(ARDUINO) 
SYSTEM_MODE(MANUAL);//do not connect to cloud
#else
SYSTEM_MODE(AUTOMATIC);//connect to cloud
#endif
         
void setup()
{
  Serial.begin(9600); //  setup serial
}

void loop()
{
  //// ENC_SW
  Serial.println(((double)analogRead(A3) * 3.3)/4096.0);  // debug value
}
