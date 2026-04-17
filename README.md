# 4-DOF Autonomous Robotic Arm
### Theory of Machines — Mechanical Engineering College Project

An autonomous pick-and-place robotic arm applying core Theory of Machines concepts. The arm detects objects using an HC-SR04 ultrasonic sensor and executes a full pick-and-place cycle autonomously — no manual input required.

---

## Theory of Machines Concepts

| Concept | Application |
|---|---|
| Degrees of Freedom | Kutzbach's criterion — DOF = 3 |
| Kinematic chain | Open serial chain (3 revolute joints) |
| Revolute pairs | Each servo = lower kinematic pair |
| Forward Kinematics | End-effector position from joint angles |
| Workspace analysis | Reachable zone of the gripper tip |

### DOF Calculation (Kutzbach's Criterion)
```
DOF = 3(n - 1) - 2j1
    = 3(4 - 1) - 2(3)
    = 9 - 6
    = 3
```
Where n = 4 links (including ground), j1 = 3 revolute joints.

---

## Hardware

| Component | Qty | Purpose |
|---|---|---|
| Arduino Uno | 1 | Main microcontroller |
| PCA9685 PWM Driver | 1 | Controls all 3 servos via I2C |
| HC-SR04 Ultrasonic | 1 | Object detection (autonomous trigger) |
| SG90 Servo Motor | 3 | Shoulder, Elbow, Gripper joints |
| 9V Battery | 2 | Separate power for board and servos |
| Breadboard + wires | 1 | Connections |

---

## Pin Mapping

| Component | Pin | Notes |
|---|---|---|
| PCA9685 SDA | A4 | I2C data |
| PCA9685 SCL | A5 | I2C clock |
| HC-SR04 TRIG | A1 | Digital output |
| HC-SR04 ECHO | A0 | Digital input |
| Servo 1 Shoulder | PCA CH1 | Via PCA9685 |
| Servo 2 Elbow | PCA CH2 | Via PCA9685 |
| Servo 3 Gripper | PCA CH3 | Via PCA9685 |

---

## Wiring

- Battery 1 (9V) powers all servos via PCA9685 V+
- Battery 2 (9V) powers Arduino via Vin
- Arduino 5V powers logic: PCA9685 VCC and HC-SR04 VCC
- All GND pins share one common rail on the breadboard (critical)

---

## Software Setup

Install via Arduino IDE Library Manager:
- Adafruit PWM Servo Driver Library
- Adafruit BusIO (auto-installed as dependency)

Upload steps: Tools > Board > Arduino Uno, Tools > Port > your COM port, then upload src/robotic_arm.ino

---

## State Machine

```
IDLE -> object within 15cm? -> PICKING -> LIFTING -> PLACING -> HOMING -> IDLE
```

Tune DETECT_DIST: open Serial Monitor at 9600 baud, place object at pick spot, note the printed distance, set DETECT_DIST to ~2cm above that value.

---

## Forward Kinematics

```
X = L1*cos(t1) + L2*cos(t1+t2) + L3*cos(t1+t2+t3)
Y = L1*sin(t1) + L2*sin(t1+t2) + L3*sin(t1+t2+t3)
```

---

## File Structure

```
robotic-arm-tom/
├── src/
│   └── robotic_arm.ino
├── docs/
│   └── report.md
├── diagrams/
│   └── circuit.md
└── README.md
```
