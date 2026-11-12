// Color-following rover. Classifies ground color with a TCS34725 color sensor:
//   green  -> keep exploring    blue -> turn around    yellow (sand) -> light the LED
#include <Wire.h>
#include <Adafruit_TCS34725.h>

const uint8_t ML_PWM = 5, MR_PWM = 6, LED_PIN = 13;
Adafruit_TCS34725 tcs(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

enum Ground { GREEN, BLUE, YELLOW, UNKNOWN };

Ground classify() {
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  if (c < 50) return UNKNOWN;
  float rn = (float)r / c, gn = (float)g / c, bn = (float)b / c;
  if (bn > rn && bn > gn) return BLUE;
  if (gn > rn && gn > bn) return GREEN;
  if (rn > bn && gn > bn && fabs(rn - gn) < 0.12) return YELLOW;
  return UNKNOWN;
}

void drive(int l, int r) { analogWrite(ML_PWM, l); analogWrite(MR_PWM, r); }

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  if (!tcs.begin()) { Serial.println(F("No sensor")); while (1); }
}

void loop() {
  switch (classify()) {
    case GREEN:  digitalWrite(LED_PIN, LOW); drive(180, 180); break;
    case BLUE:   drive(0, 0); delay(200); drive(200, 0); delay(900); break;  // ~180 turn
    case YELLOW: drive(0, 0); digitalWrite(LED_PIN, HIGH); break;
    default:     drive(120, 120);
  }
  delay(30);
}
