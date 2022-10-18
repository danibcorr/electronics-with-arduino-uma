// Definición de los pines
#define PIN_NTC A1
#define PIN_MIC A0
#define PIN_RELE 10

// Definición de los estados
#define NTC_READ_STATUS 0
#define RELE_ON_STATUS 1
#define MIC_READ_STATUS 2
#define RELE_OFF_STATUS 3

#define NTC_UMBRAL 400
#define MIC_UMBRAL 510
#define NTC_TEMP_AMBIENTE 650
#define BIAS 25

int espera_ntc = 0;
int estado = NTC_READ_STATUS;
int act_temp = 0, ult_temp = 0;

void maquina () 
{
  switch(estado) 
  {
    case NTC_READ_STATUS:
    { 
      act_temp = analogRead(PIN_NTC);
      
      if((act_temp < NTC_UMBRAL && espera_ntc == 0) || (act_temp + BIAS - ult_temp < 0)) 
      {
        espera_ntc = 1;
        estado = RELE_ON_STATUS;
        Serial.print("NTC_READ --> RELE_ON\n");
      }
      else if(espera_ntc == 1 && act_temp > NTC_UMBRAL)
      {
        espera_ntc = 0;
      }
      
      break;
    }
    
    case RELE_ON_STATUS:
    
      digitalWrite(PIN_RELE, HIGH);
      estado = MIC_READ_STATUS;
      Serial.print("RELE_ON_STATUS --> MIC_READ_STATUS\n");
      
      break;
    
    case MIC_READ_STATUS:
    
      if(analogRead(PIN_MIC) < MIC_UMBRAL) 
      {
        estado = RELE_OFF_STATUS;
        Serial.print("MIC_READ_STATUS --> RELE_OFF_STATUS");
      }
     
      break;

    case RELE_OFF_STATUS:
    
      digitalWrite(PIN_RELE, LOW);
      Serial.print("RELE_OFF_STATUS --> NTC_READ_STATUS\n");
      ult_temp = analogRead(PIN_NTC);
      estado = NTC_READ_STATUS;
      
      break;

    default:
    
      break;
  }
}

void setup() 
{
  // put your setup code here, to run once:
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
  
  maquina();
}
