// Pin Definitions
#define PIN_NTC A1
#define PIN_MIC A0
#define PIN_RELE 10

// State Definitions
#define NTC_READ_STATUS 0
#define RELE_ON_STATUS 1
#define MIC_READ_STATUS 2
#define RELE_OFF_STATUS 3

#define NTC_THRESHOLD 400
#define MIC_THRESHOLD 510
#define NTC_AMBIENT_TEMP 650
#define BIAS 25

int ntc_wait = 0;
int state = NTC_READ_STATUS;
int current_temp = 0, last_temp = 0;

void state_machine() 
{
  switch(state) 
  {
    case NTC_READ_STATUS:
    { 
      current_temp = analogRead(PIN_NTC);
      
      if((current_temp < NTC_THRESHOLD && ntc_wait == 0) || (current_temp + BIAS - last_temp < 0)) 
      {
        ntc_wait = 1;
        state = RELE_ON_STATUS;
        Serial.print("NTC_READ --> RELE_ON\n");
      }
      else if(ntc_wait == 1 && current_temp > NTC_THRESHOLD)
      {
        ntc_wait = 0;
      }
      
      break;
    }
    
    case RELE_ON_STATUS:
    
      digitalWrite(PIN_RELE, HIGH);
      state = MIC_READ_STATUS;
      Serial.print("RELE_ON_STATUS --> MIC_READ_STATUS\n");
      
      break;
    
    case MIC_READ_STATUS:
    
      if(analogRead(PIN_MIC) < MIC_THRESHOLD) 
      {
        state = RELE_OFF_STATUS;
        Serial.print("MIC_READ_STATUS --> RELE_OFF_STATUS");
      }
     
      break;

    case RELE_OFF_STATUS:
    
      digitalWrite(PIN_RELE, LOW);
      Serial.print("RELE_OFF_STATUS --> NTC_READ_STATUS\n");
      last_temp = analogRead(PIN_NTC);
      state = NTC_READ_STATUS;
      
      break;

    default:
    
      break;
  }
}

void setup() 
{
  Serial.begin(9600);
  pinMode(PIN_NTC, INPUT);
  pinMode(PIN_MIC, INPUT);
  pinMode(PIN_RELE, OUTPUT);
  digitalWrite(PIN_RELE, LOW);
}

void loop() 
{ 
  Serial.print(analogRead(PIN_NTC));
  Serial.print("\t");
  Serial.print(analogRead(PIN_MIC));
  Serial.print("\n");
  
  state_machine();
}
