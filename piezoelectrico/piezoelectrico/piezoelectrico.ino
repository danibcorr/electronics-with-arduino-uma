#define PIN_PIEZO A1
#define UMBRAL_PIEZO 6

unsigned long tiempo, tiempo2;
int control = 0, umbral = 250;

void setup() 
{
  pinMode(PIN_PIEZO, INPUT);
  Serial.begin(9600);
}

void loop() 
{
  if(control == 0)
  {
    tiempo = millis();
    control = 1;
  }
  else if(control == 1)
  {
    tiempo2 = millis();

    if(tiempo2 >= tiempo + umbral)
    {
      int muestra = analogRead(PIN_PIEZO);

      muestra -= 100;

      if(muestra < 0) 
      {
        muestra = 0;
      }

      Serial.println(muestra);
    }
  }
}
