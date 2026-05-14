#include <Arduino.h>
#include <Servo.h>

const int trigPin = 3;
const int echoPin = 4;
const int buzzerPin = 5;
const int servoPin = 6;
const int greenLed = 7; // Green = Servo Working
const int redLed = 8;   // Red = Servo Not Working

Servo myServo;

enum SystemState {READY, ACTIVE, LOCKED};
SystemState currentState = READY;

unsigned long startTime = 0;
const long waitTime = 2500; 

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  
  myServo.attach(servoPin);
  myServo.write(0); 
}

void loop() {
  // Get smoothed distance
  long duration = 0;
  for(int i=0; i<5; i++){
    digitalWrite(trigPin, LOW); delayMicroseconds(2);
    digitalWrite(trigPin, HIGH); delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration += pulseIn(echoPin, HIGH, 30000);
  }
  float distance = ( (duration/5) * 0.034) / 2;

  switch (currentState) {
    
    case READY:
      digitalWrite(redLed, HIGH);    // Servo not working = Red
      digitalWrite(greenLed, LOW);
      digitalWrite(buzzerPin, LOW);
      myServo.write(0);
    
      if (distance > 0 && distance < 40) {
        startTime = millis();
        currentState = ACTIVE;
      }
      break;

    case ACTIVE:
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, HIGH);  // Servo working = Green
      digitalWrite(buzzerPin, HIGH); 
      myServo.write(90);

      if (millis() - startTime >= waitTime) {
        myServo.write(0);
        digitalWrite(buzzerPin, LOW);
        digitalWrite(greenLed, LOW); 
        
        // GREEN FLASH 3 TIMES + BUZZ 
        for(int i = 0; i < 3; i++) {
          digitalWrite(greenLed, HIGH);
          digitalWrite(buzzerPin, HIGH);
          delay(200);
          digitalWrite(greenLed, LOW);
          digitalWrite(buzzerPin, LOW);
          delay(200);
        }
        
        currentState = LOCKED;
      }
      break;

    case LOCKED:
      digitalWrite(redLed, HIGH);
      digitalWrite(greenLed, LOW); 
      
      if (distance >= 100) {
        // RED FLASH 3 TIMES + BUZZ 
        for(int i = 0; i < 3; i++) {
          digitalWrite(redLed, HIGH);
          digitalWrite(buzzerPin, HIGH);
          delay(200);
          digitalWrite(redLed, LOW);
          digitalWrite(buzzerPin, LOW);
          delay(200);
        }
        currentState = READY;
      }
      break;
  }

  delay(50);
}