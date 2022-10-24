#define PIN_PIEZO A1
#define UMBRAL_PIEZO 6
#define LED 2

void setup() 
{
  // put your setup code here, to run once:
  pinMode(PIN_PIEZO, INPUT);
  pinMode(LED, OUTPUT);

  Serial.begin(9600);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  int muestra = analogRead(PIN_PIEZO);

  muestra -= 118;

  if(muestra < 0) 
  {
    muestra = 0;
  }
  
  Serial.println(muestra);

  if(muestra < UMBRAL_PIEZO)
  {
    digitalWrite(LED, HIGH);
  }
  else 
  {
    digitalWrite(LED, LOW);
  }

  delay(100);
}
