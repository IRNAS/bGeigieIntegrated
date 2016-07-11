#if defined(ARDUINO)
SYSTEM_MODE(MANUAL);//do not connect to cloud
#else
SYSTEM_MODE(AUTOMATIC);//connect to cloud
#endif

int swt = D6;

void setup()
{
  pinMode(swt, OUTPUT);
}

void loop()
{
  digitalWrite(swt, HIGH);
}
