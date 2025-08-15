# 📟 ESP32 + Arduino UNO Wi-Fi Resistance Meter
A dual-microcontroller system that measures an unknown resistor value using an Arduino UNO and displays the result wirelessly via a webpage hosted by an ESP32 in Access Point (AP) mode.

## 📝 Description
This project uses an **Arduino UNO** to measure an unknown resistor value using the **voltage divider method** and then sends the data to an **ESP32** over serial communication.  
The ESP32 hosts a web page that displays the resistance in **real time**, automatically rounding it to the nearest **E24 standard resistor value**.

## 🚀 Features
📡 ESP32 in **Wi-Fi Access Point** mode (no internet needed)  
🔌 **Voltage divider**-based resistance measurement  
📏 **Auto-rounding** to nearest E24 standard value (±5% tolerance)  
📺 Real-time resistance display on a web page (updates every second)  
💻 Minimalist **monospace UI** with bright green text on black background  
🛠️ Simple **Arduino ↔ ESP32** serial communication protocol  

---

## 🔌 Circuit Diagram

<img width="1427" height="590" alt="image" src="https://github.com/user-attachments/assets/8c0832ba-ce9b-4292-adb6-c2472ba916fc" />



<img width="1383" height="874" alt="image" src="https://github.com/user-attachments/assets/ba7c9f0c-75e6-41aa-aa55-c46b9f01b054" />



## 🧾 Components List

| Component               | Quantity |
|------------------------|----------|
| ESP32 Dev Board        | 1        |
| Arduino UNO            | 1        |
| Known resistor (1kΩ)   | 1        |
| Unknown resistor(s)    | 1+       |
| Breadboard             | 1        |
| Jumper wires           | As needed |
| USB cable (ESP32)      | 1        |
| USB cable (UNO)        | 1        |

---

## 📊 Components Connection Chart

### **Arduino UNO → ESP32**
| Arduino Pin | ESP32 Pin   | Purpose            |
|-------------|------------|--------------------|
| TX (Pin 1)  | GPIO16 (RX2) | Send data to ESP32 |
| RX (Pin 0)  | GPIO17 (TX2) | Receive (optional) |
| GND         | GND         | Common ground      |

### **Voltage Divider (Arduino Side)**
| From | Component            | To       |
|------|----------------------|----------|
| 5V   | Rref (1kΩ)           | A0       |
| A0   | Unknown resistor     | GND      |
| A0   | A0 (Analog input)    | Arduino  |

---

## 📦 Required Libraries
No external libraries needed — uses **built-in Arduino & ESP32 core libraries**:
- `WiFi.h` (ESP32 built-in)
- `HardwareSerial` (for `Serial2` communication)

---

## 📈 Results

Once powered on:  
- ESP32 creates a Wi-Fi hotspot **"ESP32_Resistance"** (Password: `12345678`)  
- Opening the ESP32’s IP address in a browser shows:  
  - 📟 **Real-time resistance** in Ω or kΩ  
  - 📏 Rounded to nearest **E24 value**  
  - 🛠 **Special cases**:
    - `"0 Ω"` → short circuit
    - `"Open"` → no connection

<img width="499" height="421" alt="image" src="https://github.com/user-attachments/assets/133c80c6-5068-40f3-8d46-a743d624e34d" />



<img width="454" height="248" alt="image" src="https://github.com/user-attachments/assets/eda4ec4c-7a65-4178-b795-3f5b3ad9d74f" />
 




## 🔧 Code Overview

### **Arduino UNO**
- Reads voltage from `A0`
- Calculates resistance using voltage divider formula:  
  \[
  R_x = R_{ref} \times \frac{V_{out}}{V_{in} - V_{out}}
  \]
- Rounds result to nearest **E24 standard value**
- Sends formatted string via Serial to ESP32

### **ESP32**
- Starts Wi-Fi Access Point
- Hosts HTML page with **JavaScript auto-refresh**
- Reads serial data from Arduino
- Serves resistance data via `/resistance` endpoint

---

## 🛠️ Future Development Ideas
🌐 Add **internet mode** to monitor remotely  
📱 Create **mobile-friendly UI** with responsive design  
📦 Store measurements in **Firebase** or local SD card  
📊 Plot real-time **resistance graphs**  
🔋 Make battery-powered for portability  
🛠 Add **auto-calibration** for different reference resistors  
