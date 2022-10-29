#define PIN_PIEZO A1
#define UMBRAL_PIEZO 6

int control = 0, dif_tiempo = 0;

void setup() 
{
  pinMode(PIN_PIEZO, INPUT);
  Serial.begin(9600);
}

void loop() 
{
  int muestra = analogRead(PIN_PIEZO);
  Serial.println(muestra);

  delay(25);
}
