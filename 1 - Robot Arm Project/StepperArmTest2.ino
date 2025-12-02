#include <Stepper.h>

// ====== Motor setup ======
const int STEPS_PER_REV = 2048;           // 28BYJ-48 typical
const int QUARTER_TURN  = STEPS_PER_REV / 4;

// Motor A: ESP32-S → ULN2003 #1 (1-3-2-4 order)
#define A_IN1 18
#define A_IN2 19
#define A_IN3 21
#define A_IN4 22
Stepper motorA(STEPS_PER_REV, A_IN1, A_IN3, A_IN2, A_IN4);

// Motor B: ESP32-S → ULN2003 #2 (1-3-2-4 order)
#define B_IN1 23
#define B_IN2 25
#define B_IN3 26
#define B_IN4 27
Stepper motorB(STEPS_PER_REV, B_IN1, B_IN3, B_IN2, B_IN4);

// ====== Blue LED (onboard) ======
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif
#define LED_PIN LED_BUILTIN

const unsigned BLINK_ON_MS    = 250;
const unsigned BLINK_OFF_MS   = 250;
const unsigned BLINK_PAUSE_MS = 1200;

void blinkPatternTask() {
  static uint8_t stage = 0;
  static unsigned long nextAt = 0;
  unsigned long now = millis();
  if (now < nextAt) return;

  switch (stage) {
    case 0: digitalWrite(LED_PIN, HIGH); nextAt = now + BLINK_ON_MS;  stage = 1; break;
    case 1: digitalWrite(LED_PIN, LOW);  nextAt = now + BLINK_OFF_MS; stage = 2; break;
    case 2: digitalWrite(LED_PIN, HIGH); nextAt = now + BLINK_ON_MS;  stage = 3; break;
    default: digitalWrite(LED_PIN, LOW); nextAt = now + BLINK_PAUSE_MS; stage = 0; break;
  }
}

// Step helper that keeps LED pattern alive while stepping
void moveStepsKeepingBlink(Stepper &m, int steps) {
  int dir = (steps >= 0) ? 1 : -1;
  int remaining = abs(steps);
  while (remaining--) {
    m.step(dir);          // one step at current RPM
    blinkPatternTask();   // maintain LED pattern
  }
  delay(200);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Start conservatively; raise to ~12–15 if smooth
  motorA.setSpeed(10);    // RPM
  motorB.setSpeed(10);    // RPM
}

void loop() {
  blinkPatternTask();

  // --- Motor A sequence: 90° CCW → back → 90° CW → back ---
  moveStepsKeepingBlink(motorA, -QUARTER_TURN); // 90° CCW
  moveStepsKeepingBlink(motorA,  +QUARTER_TURN); // back to origin
  moveStepsKeepingBlink(motorA,  +QUARTER_TURN); // 90° CW
  moveStepsKeepingBlink(motorA, -QUARTER_TURN);  // back to origin

  // --- Motor B sequence (independent): 90° CW → back → 90° CCW → back ---
  moveStepsKeepingBlink(motorB,  +QUARTER_TURN); // 90° CW
  moveStepsKeepingBlink(motorB, -QUARTER_TURN);  // back to origin
  moveStepsKeepingBlink(motorB, -QUARTER_TURN);  // 90° CCW
  moveStepsKeepingBlink(motorB,  +QUARTER_TURN); // back to origin

  delay(400);
}
