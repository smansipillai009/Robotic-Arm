# Circuit Connections — Robotic Arm

## Pin Mapping

| Component | Pin | Wire Colour |
|---|---|---|
| PCA9685 SDA | Arduino A4 | Green |
| PCA9685 SCL | Arduino A5 | Green |
| PCA9685 VCC | 5V rail | Red |
| PCA9685 GND | GND rail | Black |
| PCA9685 V+ | Battery 1 (+) | Orange |
| HC-SR04 TRIG | Arduino A1 | Yellow |
| HC-SR04 ECHO | Arduino A0 | Yellow |
| HC-SR04 VCC | 5V rail | Red |
| HC-SR04 GND | GND rail | Black |
| Servo 1 signal | PCA CH1 | Purple |
| Servo 2 signal | PCA CH2 | Purple |
| Servo 3 signal | PCA CH3 | Purple |
| Battery 1 (+) | PCA9685 V+ | — |
| Battery 1 (-) | GND rail | Black |
| Battery 2 (+) | Arduino Vin | — |
| Battery 2 (-) | GND rail | Black |

## Servo Wiring (each servo has 3 wires)

| Servo wire | Connect to |
|---|---|
| Brown / Black | PCA9685 GND |
| Red | PCA9685 V+ |
| Orange / Yellow | PCA9685 CH1, CH2, or CH3 |

## Critical Notes

1. All GND pins must share a common rail — Arduino GND, PCA9685 GND, both battery negatives
2. Never power servos from Arduino 5V — use Battery 1 via PCA9685 V+
3. Arduino 5V only powers logic (PCA9685 VCC, HC-SR04 VCC)
4. A4 and A5 are hardware I2C pins on Arduino Uno — do not use for anything else
5. A0 and A1 are used as digital pins for ultrasonic sensor — this is fully supported
