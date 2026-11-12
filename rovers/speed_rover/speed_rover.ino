// Speed rover: a servo shifts the drivetrain between gear ratios during the run.
// Low gear for traction off the line, then progressively faster ratios.
#include <Servo.h>

const uint8_t MOTOR_PWM = 5, SHIFT_SERVO = 9, START_BTN = 2;
const int GEAR_ANGLE[3] = {20, 90, 160};          // servo position for each ratio
const unsigned long SHIFT_AT_MS[3] = {0, 700, 1500};  // tune from timed trials

Servo shifter;

void setup() {
  pinMode(MOTOR_PWM, OUTPUT);
  pinMode(START_BTN, INPUT_PULLUP);
  shifter.attach(SHIFT_SERVO);
  shifter.write(GEAR_ANGLE[0]);
  while (digitalRead(START_BTN) == HIGH) {}      // wait for button
  delay(1000);
}

void loop() {
  static unsigned long t0 = millis();
  static int gear = 0;
  unsigned long t = millis() - t0;

  if (gear < 2 && t >= SHIFT_AT_MS[gear + 1]) {
    analogWrite(MOTOR_PWM, 120);                 // ease off so the gears mesh
    shifter.write(GEAR_ANGLE[++gear]);
    delay(120);
  }
  analogWrite(MOTOR_PWM, 255);
  if (t > 3000) { analogWrite(MOTOR_PWM, 0); while (1); }
}
