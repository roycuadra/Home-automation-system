# ESP32 WirelessControl

This project demonstrates a simple web-based control panel using ESP32 WiFi module. The control panel allows you to turn on and off GPIO5 remotely through a web browser.

## Prerequisites

- Arduino IDE installed on your computer.
- ESP32 board support added to Arduino IDE.
- Required libraries: WiFi.h

## How It Works

The ESP8266 sets up an Access Point (AP) with the specified SSID and password. It hosts a web server that serves a simple HTML page with ON and OFF buttons. When you click the buttons, corresponding HTTP requests are sent to the ESP8266, and it toggles the state of GPIO5 accordingly.

