// Pin Definitions
const int TRIG_PIN = 4;             // Arduino pin connected to Ultrasonic Sensor's TRIG pin
const int ECHO_PIN = 2;             // Arduino pin connected to Ultrasonic Sensor's ECHO pin
const int DISTANCE_THRESHOLD = 200;  // Threshold distance in centimeters

const int LED_PIN = 13;             // Pin for recording light
const int SERVO_PIN = 3;            // Pin for servo motor

#include <Servo.h>

Servo myservo1;  // servo object to control servo 1

int pos = 0;    // variable to store the initial servo position (facing forward)

void setup() {
  Serial.begin(115200);     // Initialize serial port
  pinMode(TRIG_PIN, OUTPUT); // Set Arduino pin to output mode
  pinMode(ECHO_PIN, INPUT);  // Set Arduino pin to input mode

  pinMode(LED_PIN, OUTPUT);  // Set recording light pin to output mode
  for (int i = 5; i <= 12; ++i) {
    pinMode(i, OUTPUT);     // Set LED pins to output mode
  }

  myservo1.attach(SERVO_PIN);  // attaches the servo on pin 3 to the servo object
  myservo1.write(pos);         // set the initial position of the servo to face forward
}

void loop() {
  // Blink the recording light [pin 13]
  for (int i = 0; i <= 100; i += 10) {
    analogWrite(LED_PIN, i);
    delay(50);
  }

  for (int i = 100; i >= 0; i -= 10) {
    analogWrite(LED_PIN, i);
    delay(50);
  }

  for (int j = 0; j < 3; ++j) {
    for (int i = 0; i <= 100; i += 10) {
      analogWrite(LED_PIN, i);
      delay(5);
    }

    for (int i = 100; i >= 0; i -= 10) {
      analogWrite(LED_PIN, i);
      delay(5);
    }
  }

  // Ultrasonic Sensor Distance Measurement
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  float duration_us = pulseIn(ECHO_PIN, HIGH);
  float distance_cm = 0.017 * duration_us;

  // Control 8 LEDs based on distance
  for (int i = 5; i <= 12; ++i) {
    digitalWrite(i, distance_cm <= DISTANCE_THRESHOLD ? HIGH : LOW);
  }

  // Control Servo based on distance
  if (distance_cm <= DISTANCE_THRESHOLD) {
    // Calculate the angle for servo based on distance
    int angle = map(distance_cm, 0, DISTANCE_THRESHOLD, 30, 180);
    // Constrain the angle to prevent extreme movements
    angle = constrain(angle, 0, 180);
    // Move the servo to the calculated angle
    myservo1.write(angle);
    delay(30);
  } else {
    // If no person is detected, reset the servo to face forward
    myservo1.write(90);
    //servo motor (without distance sensor)

  for (pos = 0; pos <= 180; pos += 2) {
    myservo1.write(pos);
    delay(40);
  }

  for (pos = 180; pos >= 0; pos -= 2) {
    myservo1.write(pos);
    delay(40);
  }

  }

  // Print distance to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");
  delay(100);
}