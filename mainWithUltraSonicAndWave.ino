/*
  CS 207 - Final Project
  Members: Anna Chu, Rojen Bolito, Aimee Nguyen
  Due:     December 1, 2025
*/

#include <Servo.h>

/* Global Variables */
// Servo motors
#define SERVO_PIN0 9
#define SERVO_PIN1 10
#define SERVO_PIN2 11
#define SERVO_PIN3 12
#define SERVO_PIN4 13

#define TRIG_PIN 8
#define ECHO_PIN 7

/* Button Variables */
#define BUTTON_PIN1 2        // connecting pin to the button
#define BUTTON_PIN2 3
#define BUTTON_PIN3 4

// Finger motors
Servo finger[5];
Servo waveServo;

// Button Last States
bool lastState1 = HIGH;
bool lastState2 = HIGH;
bool lastState3 = HIGH;


// Finger Angles to open and close
const int OPEN = 180;
const int CLOSE = 0;

bool userClose = false;
unsigned long lastGestureTime = 0;

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


float getDistance()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  if (duration == 0) 
  {
    Serial.println("No echo detected");
  }
  else
  {
    float distance = (duration * 0.034) / 2;
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(" cm");
  }
  // return distance;
  delay(500);
}

void waveHand()
{
  for (int i = 90; i <= 130; i++)
  {
    waveServo.write(i);
    delay(10);
  }
  for (int i = 130; i >= 50; i--)
  {
    waveServo.write(i);
    delay(10);
  }
  for (int i = 50; i <= 90; i++)
  {
    waveServo.write(i);
    delay(10);
  }
}

void beckon()
{
  moveHand(OPEN, OPEN, OPEN, OPEN, CLOSE);
  moveHand(OPEN, OPEN, OPEN, OPEN, OPEN);
}



void setup() {
  Serial.begin(9600);
  
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

  // ultrasonic and waving
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  waveServo.attach(8);
  waveServo.write(90);
}

void loop() {
  // getting distance
  // float d = getDistance();
  getDistance();
  // bool nowClose = (d < 30);

  // Coming near to wave
  if (nowClose && !userClose)
  {
    userClose = true;
  }

  // Leave to beckon motion (come here)
  if (!nowClose && userClose)
  {
    userClose = false;
  }

  // IF use, THEN wave until a button press
  if (userClose)
  {
    waveHand();
  }
  else
  {
    if (millis() - lastGestureTime > 60000)      // repeat every 1min
    {
      beckon();
      beckon();
      beckon();
      lastGestureTime = millis();
    }
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
