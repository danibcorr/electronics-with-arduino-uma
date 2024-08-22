#define PIN_PIEZO A1
#define PIEZO_THRESHOLD 6

int control = 0, time_diff = 0;

void setup() 
{
  pinMode(PIN_PIEZO, INPUT);
  Serial.begin(9600);
}

void loop() 
{
  int sample = analogRead(PIN_PIEZO);
  Serial.println(sample);

  delay(25);
}
