ESP32 Dual TB6600 Stepper Motor Controller

Control two NEMA stepper motors using an ESP32 + two TB6600 drivers.
This project provides simple and reliable motion control for robotics, CNC joints, automation experiments, and gearbox-driven actuators.

📌 Features

Controls two independent stepper motors

Compatible with TB6600 / TB6560 / DM542 step/direction drivers

Tested with:

ESP32 DevKit

TB6600 drivers

NEMA 17 + planetary gearbox

Supports:

Direction control

Adjustable speed

Adjustable number of rotations

Includes onboard LED feedback (double blink on startup + heartbeat)

📁 File Structure
ESP32_TB6600_StepperControl/
│
├── DualStepperControl.ino   # Main program
├── README.md                # Project documentation
└── /docs                    # Wiring diagrams, notes (optional)

🔌 Wiring Overview

Both TB6600 drivers must share GND with the ESP32.

ESP32 → TB6600 #1 (Motor 1)
ESP32 Pin	TB6600 Pin
GPIO 19	PUL+ (STEP+)
GPIO 18	DIR+
GPIO 21	ENA+ (optional)
GND	PUL− / DIR− / ENA−
ESP32 → TB6600 #2 (Motor 2)
ESP32 Pin	TB6600 Pin
GPIO 25	PUL+ (STEP+)
GPIO 26	DIR+
GPIO 21	ENA+ (shared with M1)
GND	PUL− / DIR− / ENA−
+V/-V

Component	Voltage
TB6600 Motor Power (V+ / V−)	12–24 V DC
ESP32 Logic Power	USB or 5 V rail
