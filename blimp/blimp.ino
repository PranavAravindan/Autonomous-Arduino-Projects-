// 3-motor RC blimp on an Arduino Nano. Control over a Bluetooth serial module (HC-05).
//
// Manual commands (single chars): F forward, L left, R right, U up, S stop.
//   B (back) and D (down) need H-bridges for reverse; left out of this one-direction build.
//
// Autonomous commands:
//   A  start a short autonomous patrol (cruise, then a gentle turn, repeat)
//   S  (or any other command) abort and return to manual control
//
// Autonomous mode needs no extra hardware. If you add an HC-SR04 ultrasonic sensor
// on the nose, uncomment USE_SONAR below and the blimp will also turn away from
// obstacles. Autonomous runs always end on their own after AUTO_MAX_MS.
#include <SoftwareSerial.h>

// #define USE_SONAR            // uncomment if an HC-SR04 is mounted facing forward

const uint8_t BT_RX = 2, BT_TX = 3;
const uint8_t MOTOR_LEFT = 5, MOTOR_RIGHT = 6, MOTOR_VERT = 9;  // PWM pins via MOSFETs
#ifdef USE_SONAR
const uint8_t TRIG = 7, ECHO = 8;
const int AVOID_CM = 60;                     // turn away when something is closer than this
const unsigned long AVOID_MIN_MS = 600;      // keep turning at least this long before re-checking
const unsigned long PING_EVERY_MS = 100;
#endif

const uint8_t THRUST = 200;                  // manual thrust
const uint8_t CRUISE_THRUST = 150;           // gentler thrust for autonomous flight
const unsigned long TIMEOUT_MS = 500;        // manual: stop if the link goes quiet

// Autonomous patrol timing (starting values: tune for your envelope and motors)
const unsigned long AUTO_MAX_MS = 20000;     // hard cap on one autonomous run
const unsigned long CRUISE_MS = 3000;        // straight segment
const unsigned long TURN_MS = 900;           // gentle turn between segments

SoftwareSerial bt(BT_RX, BT_TX);
unsigned long lastCmd = 0;

enum Mode { MANUAL, AUTO };
enum AutoState { CRUISE, TURN, AVOID };
Mode mode = MANUAL;
AutoState autoState = CRUISE;
unsigned long autoStart = 0, stateStart = 0;

void drive(int left, int right, int vert) {
  analogWrite(MOTOR_LEFT, constrain(left, 0, 255));
  analogWrite(MOTOR_RIGHT, constrain(right, 0, 255));
  analogWrite(MOTOR_VERT, constrain(vert, 0, 255));
}

#ifdef USE_SONAR
unsigned long lastPing = 0;

long readCm() {
  digitalWrite(TRIG, LOW); delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long us = pulseIn(ECHO, HIGH, 25000);
  return us ? us / 58 : 400;                 // no echo = nothing in range
}
#endif

void enterAuto() {
  mode = AUTO;
  autoState = CRUISE;
  autoStart = stateStart = millis();
  bt.println(F("AUTO on"));
}

void exitAuto(const __FlashStringHelper *msg) {
  mode = MANUAL;
  drive(0, 0, 0);
  bt.println(msg);
}

void setAutoState(AutoState s) { autoState = s; stateStart = millis(); }

void runAuto() {
  unsigned long now = millis();
  if (now - autoStart > AUTO_MAX_MS) { exitAuto(F("AUTO done")); return; }

#ifdef USE_SONAR
  if (autoState != AVOID && now - lastPing >= PING_EVERY_MS) {
    lastPing = now;
    if (readCm() < AVOID_CM) setAutoState(AVOID);
  }
#endif

  switch (autoState) {
    case CRUISE:
      drive(CRUISE_THRUST, CRUISE_THRUST, 0);
      if (now - stateStart >= CRUISE_MS) setAutoState(TURN);
      break;
    case TURN:                               // left motor only = turn right
      drive(CRUISE_THRUST, 0, 0);
      if (now - stateStart >= TURN_MS) setAutoState(CRUISE);
      break;
    case AVOID:
      drive(THRUST, 0, 0);                   // turn right until the path clears
#ifdef USE_SONAR
      if (now - stateStart >= AVOID_MIN_MS && now - lastPing >= PING_EVERY_MS) {
        lastPing = now;
        if (readCm() >= AVOID_CM) setAutoState(CRUISE);
      }
#endif
      break;
  }
}

void setup() {
  pinMode(MOTOR_LEFT, OUTPUT); pinMode(MOTOR_RIGHT, OUTPUT); pinMode(MOTOR_VERT, OUTPUT);
#ifdef USE_SONAR
  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);
#endif
  bt.begin(9600);
  drive(0, 0, 0);
}

void loop() {
  if (bt.available()) {
    char c = bt.read();
    if (c == '\r' || c == '\n' || c == ' ') return;   // ignore terminal-app line endings
    lastCmd = millis();

    if (c == 'A') {
      enterAuto();
    } else {
      if (mode == AUTO) exitAuto(F("AUTO off"));      // any other command takes back control
      switch (c) {
        case 'F': drive(THRUST, THRUST, 0); break;
        case 'L': drive(0, THRUST, 0); break;
        case 'R': drive(THRUST, 0, 0); break;
        case 'U': drive(0, 0, THRUST); break;
        case 'S': drive(0, 0, 0); break;
      }
    }
  }

  if (mode == AUTO) runAuto();
  else if (millis() - lastCmd > TIMEOUT_MS) drive(0, 0, 0);   // manual failsafe
}
