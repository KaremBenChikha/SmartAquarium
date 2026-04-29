# SmartAquarium

![Java](https://img.shields.io/badge/Java-ED8B00?style=flat-square&logo=java&logoColor=white)
![Android](https://img.shields.io/badge/Android-3DDC84?style=flat-square&logo=android&logoColor=white)
![ESP32](https://img.shields.io/badge/ESP32-E7352C?style=flat-square&logo=espressif&logoColor=white)
![Arduino](https://img.shields.io/badge/Arduino-00979D?style=flat-square&logo=arduino&logoColor=white)
![License](https://img.shields.io/badge/license-MIT-green)

Phone app to monitor and control vitals of fish in a home aquarium via ESP32.

[![Hackster](https://img.shields.io/badge/View_on-Hackster-2E9FE6)](https://www.hackster.io/karem_benchikha/smart-aquarium-d4d2dd)

## Awards

- **2nd Place** — IoT Tunisia National Competition
- **1st Place** — TUNIROBOTS'18 National Projects of Expositions
- **1st Place** — IEEE Biological IoT on Campus Competition

## Features

- **Automated Feeding**: Manual or scheduled fish feeding with configurable portions
- **Smart Lighting**: RGB color palette control + automatic night mode via light sensor
- **Temperature Monitoring**: Waterproof temperature sensor with real-time readings
- **Oxygen Control**: Low/High modes for aquarium aeration
- **Remote Access**: Full control from anywhere via internet-connected mobile app

## Repository Contents

```
SmartAquarium/
├── code/                              # Arduino firmware and Android app source
├── mechanical-design/                 # 3D models and enclosure designs
└── electrical-design/                 # Schematics and PCB layouts
```

## Built With

- **Android App**: Java, MQTT
- **Microcontroller**: ESP32
- **Cloud**: Adafruit IO (MQTT)
- **Sensors**: DS18B20 temperature, LDR light sensor
- **Actuators**: Servo motor, RGB LED strip, air pump

Developed at INSAT by an elite student team, sponsored by Business & Decisions, built inside Aerobotix INSAT Club Labs (Spring 2018).
