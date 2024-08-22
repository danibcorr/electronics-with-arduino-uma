// Necessary libraries
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

int pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0;

// Servo Pins
// Servo pins should be connected to PWM-supported ports
// For the Arduino Mega, we have 15 PWM outputs from 8-bit pins 2 to 13 and 44 to 46
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
#define ButtonYellow 36
#define ButtonBlue 38
#define ButtonRed 40
#define ButtonGreen 42

// Vector to display the startup menu message only once
// This way we are not constantly updating the value on the screen
int show_once[] = {0, 0, 0, 0};

// Global variable definitions
// Page variable
String page;

// Button press states
bool buttonState[] = {false, false, false, false};
bool previousButtonState[] = {false, false, false, false};
unsigned long debounce[4];

// Servo definitions
Servo s1, s2, s3, s4;

void returnToStart()
{
    buttonState[2] = digitalRead(ButtonRed);
    if (!buttonState[2] && previousButtonState[2] && millis() - debounce[2] >= 150UL)
    {
        lcd.clear();
        lcd.begin(16, 2);
        lcd.print("Button pressed");
        lcd.setCursor(0, 1);
        lcd.print("start menu");
        lcd.setCursor(0, 0);
        page = "Start";
        debounce[2] = millis();
    }
    previousButtonState[2] = buttonState[2];
}

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

    // Define joystick pins as input
    pinMode(j1x, INPUT);
    pinMode(j1y, INPUT);
    pinMode(j2x, INPUT);
    pinMode(j2y, INPUT);

    pinMode(ButtonYellow, INPUT_PULLUP);
    pinMode(ButtonBlue, INPUT_PULLUP);
    pinMode(ButtonRed, INPUT_PULLUP);
    pinMode(ButtonGreen, INPUT_PULLUP);

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
            lcd.clear();
            lcd.print("Start mode");
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
            lcd.clear();
            lcd.print("Manual mode");
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
            lcd.clear();
            lcd.print("Automatic mode");
        }

        AutomaticModePage();
    }
    else if (page == "Party")
    {
        if (show_once[3] == 0)
        {
            show_once[0] = 0;
            show_once[1] = 0;
            show_once[2] = 0;
            show_once[3] = 1;
            lcd.clear();
            lcd.print("Party mode");
        }

        PartyModePage();
    }
}

void chooseModePage()
{
    buttonState[0] = digitalRead(ButtonYellow);
    if (!buttonState[0] && previousButtonState[0] && millis() - debounce[0] >= 150UL)
    {
        lcd.clear();
        lcd.print("Button pressed");
        lcd.setCursor(0, 1);
        lcd.print("manual mode");
        lcd.setCursor(0, 0);
        page = "Manual";
        debounce[0] = millis();
    }
    previousButtonState[0] = buttonState[0];

    buttonState[1] = digitalRead(ButtonBlue);
    if (!buttonState[1] && previousButtonState[1] && millis() - debounce[1] >= 150UL)
    {
        lcd.clear();
        lcd.print("Button pressed");
        lcd.setCursor(0, 1);
        lcd.print("automatic mode");
        lcd.setCursor(0, 0);
        page = "Automatic";
        debounce[1] = millis();
    }
    previousButtonState[1] = buttonState[1];
    
    buttonState[3] = digitalRead(ButtonGreen);
    if (!buttonState[3] && previousButtonState[3] && millis() - debounce[3] >= 150UL)
    {
        lcd.clear();
        lcd.print("Button pressed");
        lcd.setCursor(0, 1);
        lcd.print("party mode");
        lcd.setCursor(0, 0);
        page = "Party";
        debounce[3] = millis();
    }
    previousButtonState[3] = buttonState[3];
}

void ManualModePage()
{
    returnToStart();

    //-------------------------------Manual Joystick Control-------------------------------------------------------------------------------------------------------------

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
        for (int i = s3.read(); i < 90; i++)
        {
            if (analogRead(j2y) < 800)
            {
                break;
            }
            
            s3.write(i);
            delay(20);
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
            delay(20);
       }
    }
}

void AutomaticModePage()
{
    returnToStart();
    
    for (pos3 = 110; (pos3 >= 0) && (page == "Automatic"); pos3 -= 1) 
    {
        s1.write(pos3);
        delay(10);
    }

    for (pos1 = 180; (pos1 >= 0) && (page == "Automatic"); pos1 -= 1) 
    {
        s4.write(pos1);
        delay(10);
    }

    for (pos2 = 180; (pos2 >= 135) && (page == "Automatic"); pos2 -= 1) 
    {
        s2.write(pos2);
        delay(10);
    }

    for (pos3 = 0; (pos3 <= 90) && (page == "Automatic"); pos3 += 1)
    {
        s3.write(pos3);
        delay(10);
    }

    delay(1000);

    for (pos2 = 135; (pos2 <= 180) && (page == "Automatic"); pos2 += 1) 
    {
        s2.write(pos2);
        delay(10);
    }

    for (pos1 = 0; (pos1 <= 180) && (page == "Automatic"); pos1 += 1) 
    {
        s4.write(pos1);
        delay(10);
    }

    for (pos3 = 90; (pos3 >= 0) && (page == "Automatic"); pos3 -= 1) 
    {
        s3.write(pos3);
        delay(10);
    }

    for (pos2 = 180; (pos2 >= 135) && (page == "Automatic"); pos2 -= 1) 
    {
        s2.write(pos2);
        delay(10);
    }

    delay(500);
}

void PartyModePage()
{
    returnToStart();
    
    for (pos3 = 110; (pos3 >= 0) && (page == "Party"); pos3 -= 1) 
    {
        s1.write(pos3);
        delay(10);
    }

    for (pos1 = 180; (pos1 >= 0) && (page == "Party"); pos1 -= 1) 
    {
        s4.write(pos1);
        delay(10);
    }

    for (pos2 = 180; (pos2 >= 135) && (page == "Party"); pos2 -= 1) 
    {
        s2.write(pos2);
        delay(10);
    }

    for (pos3 = 0; (pos3 <= 90) && (page == "Party"); pos3 += 1) 
    {
        s3.write(pos3);
        delay(10);
    }

    for (pos2 = 135; (pos2 <= 180) && (page == "Party"); pos2 += 1) 
    {
        s2.write(pos2);
        delay(10);
    }

    for (pos1 = 0; (pos1 <= 180) && (page == "Party"); pos1 += 1) 
    {
        s4.write(pos1);
        delay(10);
    }

    for (pos3 = 90; (pos3 >= 0) && (page == "Party"); pos3 -= 1) 
    {
        s3.write(pos3);
        delay(10);
    }

    for (pos2 = 180; (pos2 >= 135) && (page == "Party"); pos2 -= 1) 
    {
        s2.write(pos2);
        delay(10);
    }
}
