// Required Libraries
#include <Servo.h>
#include <LiquidCrystal.h>

// LCD Pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Joystick Pins
#define j1x A8
#define j1y A9
#define j2x A10
#define j2y A11

// Servo Pins
// The servo pins must be connected to PWM-supported ports
// For Arduino Mega, there are 15 PWM outputs from pins 2 to 13 and 44 to 46
#define servo_s1 9  // right
#define servo_s2 6  // left
#define servo_s3 10 // gripper
#define servo_s4 13 // base

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

// Vector to show a menu message only once
// This prevents constantly updating the display
int show_once[] = {0, 0, 0, 0, 0};

// Global Variable Definitions
// Page variable
String page;

// Button states
bool buttonState[] = {false, false, false, false};
bool previousButtonState[] = {false, false, false, false};
unsigned long debounceTime[4];

// Saved sequence number
int sequenceNumber = 0;

// Servo Definitions
Servo s1, s2, s3, s4;

//  TODO: Joystick values are not being used
int x_out = 0, y_out = 0, x_out2 = 0, y_out2 = 0;

// Define int array for saved servo positions
int servo1PosSave[] = {90, 90, 90, 90, 90};
int servo2PosSave[] = {90, 90, 90, 90, 90};
int servo3PosSave[] = {90, 90, 90, 90, 90};
int servo4PosSave[] = {90, 90, 90, 90, 90};

void setup()
{
    // Define servo pins
    s1.attach(servo_s1); // right
    s2.attach(servo_s2); // left
    s3.attach(servo_s3); // gripper
    s4.attach(servo_s4); // base

    // Initial position of the motors
    s1.write(110);
    s2.write(180);
    s3.write(0);
    s4.write(180);

    // Define joystick pins as inputs
    pinMode(j1x, INPUT);
    pinMode(j1y, INPUT);
    pinMode(j2x, INPUT);
    pinMode(j2y, INPUT);

    pinMode(YellowButton, INPUT_PULLUP);
    pinMode(BlueButton, INPUT_PULLUP);
    pinMode(RedButton, INPUT_PULLUP);
    pinMode(GreenButton, INPUT_PULLUP);

    // Initialize the LCD
    lcd.begin(16, 2);

    // Initialize serial communication
    Serial.begin(9600);

    // Initial state
    page = "Start";
}

void loop()
{
    if (page == "Start")
    {
        if (show_once[0] == 0)
        {
            show_once[0] = 1;
            show_once[1] = 0;
            show_once[2] = 0;
            show_once[3] = 0;
            show_once[4] = 0;
            lcd.clear();
            lcd.print("Start Mode");
        }

        chooseModePage();
    }
    else if (page == "Manual")
    {
        if (show_once[1] == 0)
        {
            show_once[0] = 0;
            show_once[1] = 1;
            show_once[2] = 0;
            show_once[3] = 0;
            show_once[4] = 0;
            lcd.clear();
            lcd.print("Manual Mode");
        }

        ManualModePage();
    }
    else if (page == "Automatic")
    {
        if (show_once[2] == 0)
        {
            show_once[0] = 0;
            show_once[1] = 0;
            show_once[2] = 1;
            show_once[3] = 0;
            show_once[4] = 0;
            lcd.clear();
            lcd.print("Automatic Mode");
        }

        AutomaticModePage();
    }
    else if (page == "Record")
    {
        if (show_once[3] == 0)
        {
            show_once[0] = 0;
            show_once[1] = 0;
            show_once[2] = 0;
            show_once[3] = 1;
            show_once[4] = 0;
            lcd.clear();
            lcd.print("Record Mode");
        }

        PartyModePage();
    }
    else if (page == "Execute")
    {
        if (show_once[4] == 0)
        {
            show_once[0] = 0;
            show_once[1] = 0;
            show_once[2] = 0;
            show_once[3] = 0;
            show_once[4] = 1;
            lcd.clear();
            lcd.print("Execute Mode");
        }

        ExecutePage();
    }
}

void chooseModePage()
{
    sequenceNumber = 0;

    buttonState[0] = digitalRead(YellowButton);
    if (!buttonState[0] && previousButtonState[0] && millis() - debounceTime[0] >= 150UL)
    {
        lcd.clear();
        lcd.print("Button Pressed");
        lcd.setCursor(0, 1);
        lcd.print("Manual Mode");
        lcd.setCursor(0, 0);
        page = "Manual";
        debounceTime[0] = millis();
    }
    previousButtonState[0] = buttonState[0];

    buttonState[1] = digitalRead(BlueButton);
    if (!buttonState[1] && previousButtonState[1] && millis() - debounceTime[1] >= 150UL)
    {
        lcd.clear();
        lcd.print("Button Pressed");
        lcd.setCursor(0, 1);
        lcd.print("Automatic Mode");
        lcd.setCursor(0, 0);
        page = "Automatic";
        debounceTime[1] = millis();
    }
    previousButtonState[1] = buttonState[1];
}

void ManualModePage()
{
    // Display joystick values on the serial port
    /*
    Serial.print(analogRead(j1x));
    Serial.print("\r\t");
    Serial.print(analogRead(j2x));
    Serial.print("\r\t");
    Serial.print(analogRead(j1y));
    Serial.print("\r\t");
    Serial.println(analogRead(j2y));
    */

    sequenceNumber = 0;

    buttonState[2] = digitalRead(RedButton);
    if (!buttonState[2] && previousButtonState[2] && millis() - debounceTime[2] >= 150UL)
    {
        lcd.clear();
        lcd.begin(16, 2);
        lcd.print("Button Pressed");
        lcd.setCursor(0, 1);
        lcd.print("Start Menu");
        lcd.setCursor(0, 0);
        page = "Start";
        debounceTime[2] = millis();
    }
    previousButtonState[2] = buttonState[2];

    //-------------------------------Manual Joystick Control------------------------------------------------

    if (analogRead(j1x) >= 800)
    {
        for (int i = s1.read(); i < 110; i++)
        {
            if (analogRead(j1x) < 800)
            {
                break;
            }
            s1.write(i);
            delay(20);
        }
    }
    else if (analogRead(j1x) <= 300)
    {
        for (int i = s1.read(); i > 0; i--)
        {
            if (analogRead(j1x) > 300)
            {
                break;
            }
            s1.write(i);
            delay(20);
        }
    }

    if (analogRead(j1y) >= 800)
    {
        for (int i = s2.read(); i < 180; i++)
        {
            if (analogRead(j1y) < 800)
            {
                break;
            }
            s2.write(i);
            delay(20);
        }
    }
    else if (analogRead(j1y) <= 300)
    {
        for (int i = s2.read(); i > 135; i--)
        {
            if (analogRead(j1y) > 300)
            {
                break;
            }
            s2.write(i);
            delay(20);
        }
    }

    if (analogRead(j2x) >= 800)
    {
        for (int i = s4.read(); i < 180; i++)
        {
            if (analogRead(j2x) < 800)
            {
                break;
            }
            s4.write(i);
            delay(30);
        }
    }
    else if (analogRead(j2x) <= 300)
    {
        for (int i = s4.read(); i > 100; i--)
        {
            if (analogRead(j2x) > 300)
            {
                break;
            }
            s4.write(i);
            delay(30);
        }
    }

    if (analogRead(j2y) >= 800)
    {
        for (int i = s3.read(); i < 180; i++)
        {
            if (analogRead(j2y) < 800)
            {
                break;
            }
            s3.write(i);
            delay(30);
        }
    }
    else if (analogRead(j2y) <= 300)
    {
        for (int i = s3.read(); i > 0; i--)
        {
            if (analogRead(j2y) > 300)
            {
                break;
            }
            s3.write(i);
            delay(30);
        }
    }
}

void AutomaticModePage()
{
    buttonState[2] = digitalRead(RedButton);
    if (!buttonState[2] && previousButtonState[2] && millis() - debounceTime[2] >= 150UL)
    {
        lcd.clear();
        lcd.begin(16, 2);
        lcd.print("Button Pressed");
        lcd.setCursor(0, 1);
        lcd.print("Start Menu");
        lcd.setCursor(0, 0);
        page = "Start";
        debounceTime[2] = millis();
    }
    previousButtonState[2] = buttonState[2];

    buttonState[3] = digitalRead(GreenButton);
    if (!buttonState[3] && previousButtonState[3] && millis() - debounceTime[3] >= 150UL)
    {
        lcd.clear();
        lcd.begin(16, 2);
        lcd.print("Button Pressed");
        lcd.setCursor(0, 1);
        lcd.print("Executing");
        lcd.setCursor(0, 0);
        page = "Execute";
        debounceTime[3] = millis();
    }
    previousButtonState[3] = buttonState[3];
}

void PartyModePage()
{
    // Automatic sequence saved

    sequenceNumber = 0;

    buttonState[2] = digitalRead(RedButton);
    if (!buttonState[2] && previousButtonState[2] && millis() - debounceTime[2] >= 150UL)
    {
        lcd.clear();
        lcd.begin(16, 2);
        lcd.print("Button Pressed");
        lcd.setCursor(0, 1);
        lcd.print("Start Menu");
        lcd.setCursor(0, 0);
        page = "Start";
        debounceTime[2] = millis();
    }
    previousButtonState[2] = buttonState[2];
}

void ExecutePage()
{
    // Execute the sequence saved

    buttonState[2] = digitalRead(RedButton);
    if (!buttonState[2] && previousButtonState[2] && millis() - debounceTime[2] >= 150UL)
    {
        lcd.clear();
        lcd.begin(16, 2);
        lcd.print("Button Pressed");
        lcd.setCursor(0, 1);
        lcd.print("Start Menu");
        lcd.setCursor(0, 0);
        page = "Start";
        debounceTime[2] = millis();
    }
    previousButtonState[2] = buttonState[2];
}
