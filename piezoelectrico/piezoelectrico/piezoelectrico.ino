#define PIN_PIEZO A1
#define UMBRAL_PIEZO 6

void setup() 
{
  pinMode(PIN_PIEZO, INPUT);
  Serial.begin(9600);
}

void loop() 
{
  int muestra = analogRead(PIN_PIEZO);

  muestra -= 118;

  if(muestra < 0) 
  {
    muestra = 0;
  }
  
  Serial.println(muestra);
  delay(100);
}
