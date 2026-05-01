# Curl Tracker: Wearable Bicep Curl Coach (Work In Progress)

## Project Description
* **At a High Level:** FlexTracker is a wearable embedded systems project that straps onto your forearm and acts as a personal bicep curl coach. The device counts your reps in real time, evaluates the speed and form of each curl using motion data, and gives the user instant haptic feedback (a small vibration motor) when a curl is performed too fast or with poor form. The data is shown live on a small LCD/OLED display so the user can keep an eye on their rep count and feedback while lifting, and the system is designed to eventually stream data to a connected laptop for graphing and longer-term progress tracking.

* **Why this project is interesting:** Many people who lift weights especially beginners often exercise with incorrect form causing suboptimal muscle growth and potential for injury. This project tackles a real, common problem in the gym with a low-cost, low-power microcontroller. It combines hardware (sensors, soldering, power design), embedded firmware in C/C++, and eventually data visualization on a host computer for a full hardware/software system. 

* **Tools/Technologies used:**
  * **ESP32** microcontroller (Wi-Fi/Bluetooth capable)
  * **MPU6050** 6-axis IMU (accelerometer + gyroscope) for motion sensing
  * **Vibration motor** for haptic feedback
  * **1N4001 flyback diode** across the motor for inductive kickback protection
  * **Schottky diodes** for reverse polarity protection on the battery input
  * **I2C LCD/OLED display** for live readout
  * **AA battery pack** (3x AA) for portable power
  * **Protoboard + soldering** for the current build; KiCad for schematic capture and a future PCB
  * **PlatformIO** C++ for firmware, and eventually Python (matplotlib / PyQt) on the laptop side for graphing streamed data

* **Inputs/Outputs of the project:**
  * **Inputs:** Raw accelerometer and gyroscope data from the MPU6050 over I2C, the user physically performing curls while wearing the device.
  * **Outputs:** Real-time rep count and form feedback on the LCD/OLED, vibration pulses from the motor when a curl is performed too fast or with bad form, and (planned) a serial/Bluetooth data stream to a laptop for graphing rep statistics.

* **Features:** The device reads motion data from the IMU, detects each curl using a simple state machine, and flags reps that are too fast or sloppy. When that happens, the vibration motor buzzes your forearm so you know to slow down, and the LCD keeps a running count of your reps. The battery input has reverse-polarity protection so a flipped battery won't fry anything. Down the line I want to add live data streaming to a laptop, per-set logging, and a 3D-printed case with a Velcro strap so it's actually wearable at the gym.

## Hardware Schematic

The schematic below shows the current circuit: an ESP32 dev module communicating with the MPU6050 IMU over I2C (SDA/SCL on GPIO21/22), a 1kΩ base resistor driving an NPN BJT that switches the vibration motor, a 1N4001 flyback diode across the motor coil, and an I2C LCD breakout sharing the I2C bus with the IMU.

<img width="1061" height="623" alt="image" src="https://github.com/user-attachments/assets/2e6edfe2-e8a7-4b6f-b465-926a0582be03" />

### Description
* **ESP32 (IC2):** Main controller. 3.3V is supplied to the 3V3 pin; GND pins are tied together. GPIO21 and GPIO22 are the I2C SDA/SCL lines, shared between the MPU6050 and the LCD. GPIO25 drives the base of the NPN transistor through R1 (1kΩ) to switch the vibration motor.
* **MPU6050 (top right):** Connected to the ESP32 over I2C. VDD/VLOGIC are tied to 3.3V, GND to ground, SDA/SCL to the shared I2C bus. AD0 is grounded so the IMU sits at the default I2C address (0x68).
* **Vibration motor + flyback diode (D1, 1N4001):** The motor is switched low-side by an NPN BJT. R1 limits base current. D1 is placed reverse-biased across the motor terminals so that when the transistor turns off, the inductive spike from the motor is shunted through the diode instead of flying back into the transistor or the ESP32.
* **LCD_I2C (4-pin connector):** VCC, GND, SDA, SCL — shares the I2C bus with the MPU6050.
* **Power:** 3xAA pack feeds a Schottky diode (not shown in this version of the schematic, on the protoboard) for reverse polarity protection, then into the ESP32's 3.3V rail through the onboard regulator.

## Current Build

<img width="830" height="842" alt="image" src="https://github.com/user-attachments/assets/018fee66-cf24-4c73-bc0f-7035d48fece7" />

### Description
The current physical prototype is a hand-soldered protoboard with the ESP32 dev module, the MPU6050 breakout, the vibration motor driver circuit (NPN transistor, base resistor, flyback diode), and a connector header for the OLED display. Power is provided by a 3xAA battery pack routed through a Schottky diode for reverse-polarity protection. The OLED currently displays raw accelerometer (g) and gyroscope (deg/s) values from the MPU6050 for sanity checking — this will be replaced with the rep counter and form feedback once that firmware module is finished.

## Current Progress

* IMU is wired up and reading clean accelerometer + gyroscope data over I2C, displayed live on the OLED for verification.
* Vibration motor driver circuit is built on the protoboard with proper flyback diode protection, and the ESP32 successfully triggers the motor when a fast movement is detected via a simple acceleration-magnitude threshold.
* Battery power with reverse-polarity protection (Schottky diode) is working, so the device can run untethered.
* KiCad schematic is captured for the current circuit and will be the basis for a future PCB revision.

## Roadmap / Planned Work

* **Rep detection state machine** — replace the current "fast movement → buzz" placeholder with a real up/down phase detector so the device actually counts reps instead of just reacting to motion spikes.
* **More LCD UI features** — show rep count, last rep tempo (in seconds), and a form status indicator (good / too fast / incomplete) instead of raw IMU values.
* **Laptop streaming + graphing** — connect the ESP32 to a laptop (via MQTT broker) and build a small Python dashboard to display rep statistics. 
* **Enclosure + strap** — design a 3D-printed enclosure with a Velcro forearm strap so the device can actually be worn during a workout.

## Installation / Usage

> This project is a work in progress. Firmware and hardware are still evolving.
