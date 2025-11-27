/* USING ULTRASONIC SENSOR */

/*
  CS 207 - Final Project
  Members: Anna Chu, Rojen Bolito, Aimee Nguyen
  Due:     December 1, 2025
*/

#include <Servo.h>
#define TRIG_PIN 8
#define ECHO_PIN 7

/* Global Variables */
// Servo motors
#define SERVO_PIN0 9
#define SERVO_PIN1 10
#define SERVO_PIN2 11
#define SERVO_PIN3 12
#define SERVO_PIN4 13

/* Button Variables */
#define BUTTON_PIN1 2        // connecting pin to the button
#define BUTTON_PIN2 3
#define BUTTON_PIN3 4

// Finger motors
Servo finger[5];

// Button Last States
bool lastState1 = HIGH;
bool lastState2 = HIGH;
bool lastState3 = HIGH;


// Finger Angles to open and close
const int OPEN = 180;
const int CLOSE = 0;

void moveHand(int f0, int f1, int f2, int f3, int f4)
{
  int target[5] = {f0, f1, f2, f3, f4};

  for (int i = 0; i < 5; i++)
    {
      finger[i].write(target[i]);
      delay(300);
    }
}


/* Preset Position Functions */
void peaceSign()
{
  moveHand(CLOSE, OPEN, OPEN, CLOSE, CLOSE);
  delay(1000);
  moveHand(OPEN, OPEN, OPEN, OPEN, OPEN);
}

void thumbsUp()
{
  moveHand(OPEN, CLOSE, CLOSE, CLOSE, CLOSE);
  delay(1000);
  moveHand(OPEN, OPEN, OPEN, OPEN, OPEN);
}

void middleFinger()
{
  moveHand(CLOSE, CLOSE, OPEN, CLOSE, CLOSE);
  delay(1000);
  moveHand(OPEN, OPEN, OPEN, OPEN, OPEN);
}


void setup() {
  // setup for ultrasonic sensor
  Serial_begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  finger[0].attach(9);  // Attach servo to pin 10
  finger[1].attach(10);
  finger[2].attach(11);
  finger[3].attach(12);
  finger[4].attach(13);

  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  pinMode(BUTTON_PIN3, INPUT_PULLUP);

  // staring with open hand
  for (int i = 0; i < 5; i++)
  {
    finger[i].write(OPEN);
  }   
}

void loop() {
  // sensor
  digitalWrite(TRIG_PIN, LOW);
  digitalMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  digitalMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);

  if (duration == 0)
  {
    Serial.printLn("No echo detected");
  }
  else
  {
    float distance = (duration * 0.034) / 2;
    Serial.print("Distnce: ");
    Serial.print(distance);
    Serial.print(" cm");
  }
  
  
  // button state
  bool positionPeace = digitalRead(BUTTON_PIN1);
  bool positionThumb = digitalRead(BUTTON_PIN2);
  bool positionMiddle = digitalRead(BUTTON_PIN3);

  // IF button 1-3 are pressed
  if (positionPeace == LOW && lastState1 == HIGH)
  {
    peaceSign();
  }
  else if (positionThumb == LOW && lastState2 == HIGH)
  {
    thumbsUp();
  }
  else if (positionMiddle == LOW && lastState3 == HIGH)
  {
    middleFinger();
  }

  lastState1 = positionPeace;
  lastState2 = positionThumb;
  lastState3 = positionMiddle;


  delay(100);
}
