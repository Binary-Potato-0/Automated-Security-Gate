# Automated Touchless Security Gate

An embedded systems project featuring a state-machine-driven security interlock built with an Arduino Nano. This system utilizes an ultrasonic distance sensor to trigger an automated servo mechanism and enforces a strict spatial lockout protocol before allowing subsequent triggers.

## ⚙️ Hardware Components
*   **Microcontroller:** Arduino Nano (ATmega328P)
*   **Sensor:** HC-SR04 Ultrasonic Distance Sensor
*   **Actuator:** Standard 5V Servo Motor
*   **Indicators:** Red/Green LEDs & Active Buzzer

## 🧠 System Logic (State Machine)
The core logic is built around a non-blocking state machine utilizing `millis()` for timer management, preventing the main loop from hanging during delays.

1.  **READY State:** System armed. Red LED solid (servo inactive). Triggered when an object is detected within **40 cm**.
2.  **ACTIVE State:** Servo actuates to 90 degrees. Green LED solid. Buzzer sounds. Timer starts for a strict **2.5-second** window.
3.  **LOCKOUT Transition:** Servo closes. Green LED and buzzer flash 3 times to confirm successful closure.
4.  **LOCKED State:** System ignores all triggers within the operational radius. Red LED solid. Requires the area to be cleared to a distance of **100 cm** or greater to initiate a reset.
5.  **RESET Transition:** Red LED and buzzer flash 3 times. System returns to READY state.

## 🛠️ Signal Filtering
Raw ultrasonic data is inherently noisy due to air turbulence and varied surface reflections. This system implements a **Simple Moving Average (SMA)** filter across 5 localized pulses per loop iteration to stabilize the `pulseIn()` data before evaluating state conditions.

## 👨‍💻 Author
**Mostafa Eldeeb**
