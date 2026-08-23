#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// ── Servo channels (PCA9685 ports 1, 2, 3) ──────────────
#define SHOULDER  1
#define ELBOW     2
#define GRIPPER   3

// ── PWM pulse range for SG90 ─────────────────────────────
#define SERVOMIN  150
#define SERVOMAX  600

// ── Ultrasonic pins (analog pins used as digital) ────────
const int trigPin = A1;
const int echoPin = A0;

// ── Detection threshold ──────────────────────────────────
const float DETECT_DIST = 15.0;

// ── State machine ────────────────────────────────────────
enum class ArmState { IDLE, PICKING, LIFTING, PLACING, HOMING };
ArmState currentState = ArmState::IDLE;

// ────────────────────────────────────────────────────────
int angleToPulse(int angle) {
  return map(angle, 0, 180, SERVOMIN, SERVOMAX);
}

void moveServo(uint8_t ch, int from, int to, int spd = 12) {
  int step = (from < to) ? 1 : -1;
  for (int a = from; a != to + step; a += step) {
    pwm.setPWM(ch, 0, angleToPulse(a));
    delay(spd);
  }
}

float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long dur = pulseIn(echoPin, HIGH, 30000);
  if (dur == 0) return 999.0;
  return (dur * 0.0343) / 2.0;
}

bool objectPresent() {
  float d1 = getDistance();
  delay(50);
  float d2 = getDistance();
  bool detected = (d1 < DETECT_DIST && d2 < DETECT_DIST);
  Serial.print("Scan: "); Serial.print(d1);
  Serial.print("cm | "); Serial.print(d2);
  Serial.println(detected ? "cm → DETECTED" : "cm → clear");
  return detected;
}

// ── Arm positions ─────────────────────────────────────────
void goHome() {
  Serial.println("[HOME]");
  moveServo(GRIPPER,  30, 90);
  moveServo(ELBOW,    90, 90);
  moveServo(SHOULDER, 90, 90);
}

void reachToPick() {
  Serial.println("[REACH]");
  moveServo(SHOULDER, 90, 55);
  moveServo(ELBOW,    90, 135);
}

void grip() {
  Serial.println("[GRIP]");
  moveServo(GRIPPER, 90, 30);
  delay(400);
}

void liftUp() {
  Serial.println("[LIFT]");
  moveServo(ELBOW,   135, 90);
  moveServo(SHOULDER, 55, 150);
  delay(300);
}

void release() {
  Serial.println("[RELEASE]");
  moveServo(GRIPPER, 30, 90);
  delay(400);
}

// ── State machine ─────────────────────────────────────────
void runStateMachine() {
  switch (currentState) {

    case ArmState::IDLE:
      Serial.println("IDLE — scanning...");
      if (objectPresent()) {
        delay(300);
        currentState = ArmState::PICKING;
      }
      break;

    case ArmState::PICKING:
      Serial.println("STATE: PICKING");
      reachToPick();
      delay(400);
      grip();
      currentState = ArmState::LIFTING;
      break;

    case ArmState::LIFTING:
      Serial.println("STATE: LIFTING");
      liftUp();
      currentState = ArmState::PLACING;
      break;
 case ArmState::PLACING:
      Serial.println("STATE: PLACING");
      delay(500);
      release();
      currentState = ArmState::HOMING;
      break;

    case ArmState::HOMING:
      Serial.println("STATE: HOMING");
      goHome();
      delay(1500);
      currentState = ArmState::IDLE;
      break;
  }
}

// ── Setup & loop ──────────────────────────────────────────
void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(50);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
   Serial.println("=== Robotic Arm Ready ===");
  Serial.println("Place object within 15cm to trigger.");
  goHome();
  delay(2000);
}

void loop() {
  runStateMachine();
  delay(200);
}