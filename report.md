# Project Report — Autonomous Robotic Arm
## Theory of Machines | Mechanical Engineering

---

## 1. Abstract

This project demonstrates a 3-DOF autonomous robotic arm applying core Theory of Machines concepts including kinematic chains, revolute pairs, Kutzbach's criterion, and forward kinematics. The arm detects objects using an HC-SR04 ultrasonic sensor and executes a complete pick-and-place cycle autonomously using Arduino Uno and a PCA9685 PWM servo driver.

---

## 2. Theory of Machines Concepts

### 2.1 Kinematic Chain
The robotic arm forms an open kinematic chain — a serial manipulator where each link is connected to the next by a revolute joint. Unlike a closed chain (such as a four-bar linkage), an open chain has a free end (the end-effector/gripper).

### 2.2 Kinematic Pairs
Each servo joint is a revolute pair (lower kinematic pair) — it allows rotation about a single axis, constraining all other motion.

| Joint | Link | Type |
|---|---|---|
| Joint 1 | Shoulder | Revolute pair |
| Joint 2 | Elbow | Revolute pair |
| Joint 3 | Gripper | Revolute pair |

### 2.3 Degrees of Freedom — Kutzbach's Criterion

For a planar mechanism:
```
DOF = 3(n - 1) - 2j1 - j2

Where:
  n  = number of links (including ground) = 4
  j1 = full joints (revolute)             = 3
  j2 = half joints                        = 0

DOF = 3(4 - 1) - 2(3) - 0
    = 9 - 6
    = 3
```

Each servo motor controls 1 DOF independently.

### 2.4 Forward Kinematics

Given link lengths L1, L2, L3 and joint angles t1, t2, t3:

```
X = L1*cos(t1) + L2*cos(t1+t2) + L3*cos(t1+t2+t3)
Y = L1*sin(t1) + L2*sin(t1+t2) + L3*sin(t1+t2+t3)
```

This can be verified experimentally by measuring the physical position of the gripper tip for known joint angles set in code.

---

## 3. Hardware Description

### 3.1 Components
- Arduino Uno (ATmega328P) — main controller
- PCA9685 16-channel PWM driver — I2C servo control
- HC-SR04 ultrasonic sensor — object detection
- 3x SG90 servo motors — joint actuation
- 2x 9V batteries — isolated power supply

### 3.2 Power Design
Two separate batteries prevent servo current draw from browning out the Arduino:
- Battery 1 powers servos via PCA9685 V+ (up to 3A peak draw)
- Battery 2 powers Arduino via Vin (regulated to 5V on-board)
- Arduino 5V output powers logic only: PCA9685 VCC and HC-SR04 VCC

---

## 4. Software Architecture

### 4.1 State Machine
The firmware implements a finite state machine with 5 states:

| State | Action |
|---|---|
| IDLE | Continuously scans for objects using ultrasonic sensor |
| PICKING | Reaches to fixed pick position and grips object |
| LIFTING | Raises arm and rotates to drop zone |
| PLACING | Lowers arm and releases object |
| HOMING | Returns to home position (90 degrees all joints) |

### 4.2 Object Detection
Double-confirmation scanning prevents false triggers from sensor noise:
```cpp
float d1 = getDistance();
delay(50);
float d2 = getDistance();
bool detected = (d1 < DETECT_DIST && d2 < DETECT_DIST);
```

### 4.3 Servo Control via PCA9685
The PCA9685 communicates with Arduino over I2C (SDA=A4, SCL=A5) and generates PWM signals for each servo. Angle is converted to pulse width:
```cpp
int angleToPulse(int angle) {
  return map(angle, 0, 180, 150, 600);
}
```

---

## 5. Results

- Arm successfully detects objects within 15cm range
- Complete pick-and-place cycle executes in approximately 8-10 seconds
- Double-confirmation scan eliminates false triggers
- State machine provides structured, debuggable control visible on Serial Monitor

---

## 6. Conclusion

The project successfully demonstrates application of Theory of Machines concepts in a working embedded system. Kutzbach's criterion correctly predicts DOF = 3 for the 3-joint arm configuration. The forward kinematics equations were verified by physically measuring end-effector position at known joint angles programmed in the firmware.
