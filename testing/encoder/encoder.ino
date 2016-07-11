#if defined(ARDUINO)
SYSTEM_MODE(MANUAL);//do not connect to cloud
#else
SYSTEM_MODE(AUTOMATIC);//connect to cloud
#endif

int encoder0Pos = 0;
bool encoder0PinALast = LOW;
bool n = LOW;

void setup()
{
  pinMode (A6, INPUT);
  pinMode (A7, INPUT);
  Serial.begin (9600);

  for (int i = 0; i < 5; i++)
  {
    Serial.println("ready");
    delay(1000);
  }
}

void loop() {
  n = digitalRead(A6);
  if ((encoder0PinALast == LOW) && (n == HIGH))
  {
    if (digitalRead(A7) == LOW)
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
}
