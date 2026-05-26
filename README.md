# Curl Tracker: Wearable Bicep Curl Coach (Work In Progress)

# What is Curl Tracker?

Curl Tracker is a wearable bicep curl tracker that uses an ESP32 Dev Board to read MPU6050 IMU data through I²C. It displays rep data through a SH1106 OLED display and transmits curl metrics over MQTT (Mosquitto). Curl Tracker includes a vibration motor for haptic feedback based on rep speed and curl form. 

## Hardware Schematic

<img width="1061" height="623" alt="image" src="https://github.com/user-attachments/assets/2e6edfe2-e8a7-4b6f-b465-926a0582be03" />

This is the intial schematic of Curl Tracker, made through KiCad. The main highlight of the schematic is the flyback diode for the DC motor to protect against voltage spikes when the power is turned off. After creating the schematic, I verified the design by creating a prototype on a breadboard. 

## Past Builds

<img width="830" height="842" alt="image" src="https://github.com/user-attachments/assets/018fee66-cf24-4c73-bc0f-7035d48fece7" />

<img width="6048" height="3197" alt="IMG_3831" src="https://github.com/user-attachments/assets/787d949a-eb10-4d39-865b-60e42729055c" />

<img width="1659" height="1993" alt="IMG_3801" src="https://github.com/user-attachments/assets/58b5fa50-1344-4bb1-bcdc-7f2f331b18c1" />



## Current Build

<img width="6048" height="4270" alt="IMG_3867" src="https://github.com/user-attachments/assets/69bd071e-0a1e-49c7-9299-3584332e1c1a" />





## Installation / Usage

> This project is a work in progress. Firmware and hardware are still evolving.
