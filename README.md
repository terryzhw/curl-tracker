# Curl Tracker: Wearable Bicep Curl Coach (Work In Progress)

# What is Curl Tracker?

Curl Tracker is a wearable bicep curl tracker that uses an ESP32 Dev Board to read MPU6050 IMU data through I²C. It displays rep data through a SH1106 OLED display and transmits curl metrics over MQTT (Mosquitto). Curl Tracker includes a vibration motor for haptic feedback based on rep speed and curl form. 

## State Machine
<img width="1" height="1" alt="image" src="https://github.com/user-attachments/assets/bccca7d2-2131-49b5-8a33-95defc3ed4c1" />

<br>

Simple state machine showing how reps are counted for Curl Tracker.

## Hardware Schematic

<img width="1048" height="720" alt="image" src="https://github.com/user-attachments/assets/4b443dde-6515-4ee3-806c-5ae9223b752c" />


This is the intial schematic of Curl Tracker, made through KiCad. I added a flyback protection circuit to prevent sudden voltage spikes caused by the haptic DC motor. I also included a MCP1700-3302E voltage regulator to step down the 3.7V LiPo Battery to 3.3V. After creating the schematic, I verified the design by creating a prototype on a breadboard. 

## Past Builds

<img width="830" height="842" alt="image" src="https://github.com/user-attachments/assets/018fee66-cf24-4c73-bc0f-7035d48fece7" />

<br>

This was the first iteration of Curl Tracker, soldered on a perfboard. I powered Curl Tracker with 3 AA rechargable lithium batteries for a total of 4.5V. This iteration was not ideal due to the weight of the 3 AA batteries. 

<img width="6048" height="3197" alt="IMG_3831" src="https://github.com/user-attachments/assets/787d949a-eb10-4d39-865b-60e42729055c" />

<br> 

This was the second iteration of Curl Tracker, now powered through a 3.7V LiPo battery. I connected it with a TP4056 Charge Module so that I could charge the Lipo battery through USB C. It also includes overcharge protection for the Lipo battery. I soldered a mechanical switch to the ground of the TP4056 Charge Module for an easy On-Off switch. This greatly reduced the weight caused by the 3 AA batteries. 

<img width="1659" height="1993" alt="IMG_3801" src="https://github.com/user-attachments/assets/58b5fa50-1344-4bb1-bcdc-7f2f331b18c1" />

<br> 

This was the first design for the case of Curl Tracker, made through Autodesk Fusion. This was just a proof of concept so that I could get a feel for how the final design could look in real-life. This was way too bulky, with the walls too thick and tall. It also had no holes for the TP4056 Charge Module and ESP32 programming USB C ports. 

## Current Build

<img width="6048" height="4270" alt="IMG_3867" src="https://github.com/user-attachments/assets/69bd071e-0a1e-49c7-9299-3584332e1c1a" />

<br>

I soldered the MPU6050 and the SH1106 OLED Display to a separate perfboard and soldered the connections for a more permanent build. This gives more stability for the MPU6050 which needs to be in the correct orientation for accurate readings. 


<br>


## Plans

- I plan to implement the MQTT protocol using Mosquitto to send data to a laptop/app for graphing and analysis.
- I plan to fully design the enclosure for Curl Tracker with Autodesk Fusion.

<br>

## Installation / Usage

> This project is a work in progress. Firmware and hardware are still being made.
