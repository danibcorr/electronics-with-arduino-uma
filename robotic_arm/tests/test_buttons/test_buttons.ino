// Required libraries
#include <Servo.h> 

// Button Pins
// Define variables for button values
// Yellow button = Manual
// Blue button = Automatic
// Red button = Back
// Green button = Execute recorded movements
#define YellowButton 36
#define BlueButton 38
#define RedButton 40
#define GreenButton 42

// Button press states
bool buttonState[] = {false, false, false, false};
bool previousButtonState[] = {false, false, false, false};
unsigned long debounceTime[4];

void setup() 
{
  pinMode(YellowButton, INPUT_PULLUP);
  pinMode(BlueButton, INPUT_PULLUP);
  pinMode(RedButton, INPUT_PULLUP);
  pinMode(GreenButton, INPUT_PULLUP);

  // Initialize serial communication
  Serial.begin(9600);
}

void loop()
{
  buttonState[0] = digitalRead(YellowButton);
  if (!buttonState[0] && previousButtonState[0] && millis() - debounceTime[0] >= 200UL)
  {
    Serial.println("YellowButton");
    debounceTime[0] = millis();
  }      
  previousButtonState[0] = buttonState[0];

  buttonState[1] = digitalRead(RedButton);
  if (!buttonState[1] && previousButtonState[1] && millis() - debounceTime[1] >= 200UL)
  {
    Serial.println("RedButton");
    debounceTime[1] = millis();
  }      
  previousButtonState[1] = buttonState[1];

  buttonState[2] = digitalRead(BlueButton);
  if (!buttonState[2] && previousButtonState[2] && millis() - debounceTime[2] >= 200UL)
  {
    Serial.println("BlueButton");
    debounceTime[2] = millis();
  }      
  previousButtonState[2] = buttonState[2];

  buttonState[3] = digitalRead(GreenButton);
  if (!buttonState[3] && previousButtonState[3] && millis() - debounceTime[3] >= 200UL)
  {
    Serial.println("GreenButton");
    debounceTime[3] = millis();
  }      
  previousButtonState[3] = buttonState[3];
}
