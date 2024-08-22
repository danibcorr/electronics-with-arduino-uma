#include <Servo.h>

Servo myservo;  // Create a Servo object named myservo
Servo myservo2; // Create another Servo object named myservo2

int pos = 0;    // Variable to store the servo position

void setup() 
{
  myservo.attach(22);  // Attach the first servo to pin 22
  myservo2.attach(24); // Attach the second servo to pin 24
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) 
  {
    myservo.write(pos);        // Set the position of the first servo
    myservo2.write(pos);       // Set the position of the second servo
    delay(5);                  // Wait for 5 milliseconds
  }
  for (pos = 180; pos >= 0; pos -= 1) 
  { 
    myservo.write(pos);        // Set the position of the first servo
    myservo2.write(pos);       // Set the position of the second servo
    delay(15);                 // Wait for 15 milliseconds
  }
}
