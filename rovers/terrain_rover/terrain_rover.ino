// Terrain-exploring rover: ultrasonic obstacle avoidance plus a rack-and-pinion
// arm (servo) that lowers a soil moisture sensor. Stream-bed detection with a
// camera ran on a separate system; here "bed found" is a simple trigger you can swap.
#include <Servo.h>

const uint8_t TRIG = 7, ECHO = 8;
const uint8_t ML_FWD = 5, ML_REV = 6, MR_FWD = 9, MR_REV = 10;
const uint8_t ARM_SERVO = 11, MOISTURE_PIN = A1;
const int OBSTACLE_CM = 20;
const int ARM_UP = 20, ARM_DOWN = 150;
const unsigned long TEST_EVERY_MS = 8000;

Servo arm;
unsigned long lastTest = 0;

long readCm() {
  digitalWrite(TRIG, LOW); delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long us = pulseIn(ECHO, HIGH, 25000);
  return us ? us / 58 : 400;
}

void motors(int l, int r) {   // -255..255
  analogWrite(ML_FWD, l > 0 ? l : 0);  analogWrite(ML_REV, l < 0 ? -l : 0);
  analogWrite(MR_FWD, r > 0 ? r : 0);  analogWrite(MR_REV, r < 0 ? -r : 0);
}

void takeSoilReading() {
  motors(0, 0);
  arm.write(ARM_DOWN); delay(1500);
  int raw = analogRead(MOISTURE_PIN);
  Serial.print(F("soil_raw=")); Serial.println(raw);
  arm.write(ARM_UP); delay(1500);
}

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);
  arm.attach(ARM_SERVO); arm.write(ARM_UP);
}

void loop() {
  if (millis() - lastTest > TEST_EVERY_MS) { takeSoilReading(); lastTest = millis(); }
  if (readCm() < OBSTACLE_CM) {
    motors(-180, -180); delay(400);
    motors(200, -200);  delay(random(300, 700));   // turn a random amount
  } else {
    motors(180, 180);
  }
  delay(40);
}
