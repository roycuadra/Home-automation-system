# ⚡ WirelessRelay Wizard

## 🎬 Video Demonstration

Check out the live demo:  
👉 [Video Demonstration](https://www.dropbox.com/scl/fi/2l01fe7jwbvobmozqn8aw/VID20231105195200.mp4?rlkey=d0561vvrgo3jbvtmx15pl6jf9&st=lil6m604&dl=0) 

---

## 📖 Description

This project demonstrates a **simple web-based control panel** using an **ESP8266 or ESP32**.  
It lets you wirelessly turn a **relay module ON or OFF** using a browser, no internet needed!

---

## 🚀 How It Works

- The ESP8266/ESP32 creates a **Wi-Fi Access Point** with a custom SSID and password.
- A simple **web server** runs on port 80.
- The server displays an HTML interface with **ON/OFF buttons**.
- Clicking a button sends a **GET request** to toggle GPIO12 (D4 on ESP8266).
- A **buzzer** gives audio feedback every time the button is pressed.

---

## 🔌 Wiring Guide

| Component         | ESP8266 Pin | GPIO | Description               |
|------------------|-------------|------|---------------------------|
| Relay IN         | D4          | 2    | Controls device ON/OFF    |
| Buzzer +         | D1          | 5    | Buzzer feedback           |
| Relay VCC        | 5V          | —    | Power supply              |
| Buzzer – / Relay GND | GND     | —    | Common ground             |

⚠️ Use the **NO (Normally Open)** terminal of the relay for correct operation.

---

## 🖼️ Schematic Diagram

> 📷 **Schematic Diagram Screenshot**

![Schematic Diagram](schematic.png)

*Make sure to replace the placeholder above with your actual image path.*

---

## 🧪 Getting Started

### ✅ Requirements

- ESP8266 NodeMCU or ESP32
- 1× Relay Module
- 1× Active Buzzer Module
- Jumper wires, USB cable

### 🔧 Setup Steps

1. **Connect the components** as per the schematic above.
2. **Upload the Arduino code** using the Arduino IDE.
   - Use the correct board (ESP8266 or ESP32).
   - Install the required libraries.
3. **Connect to the Wi-Fi Access Point** created by the device:
   - **SSID:** `IoT Home Automation`
   - **Password:** `admin12345`
4. Open your browser and go to `192.168.4.1`.
5. Use the web interface to toggle the relay.

---

## 📜 License

This project is licensed under the **MIT License**.  
See the [LICENSE](LICENSE) file for details.

---

## 🙋‍♂️ Author

Created by **Roy Cuadra**  
📅 Date: *05/11/2023*
