# WirelessRelay Wizard

- This project demonstrates a simple web-based control panel using ESP32 or ESP8266. 
- The control panel allows you to turn on and off GPIO12 remotely through a web browser.

## How It Works

The ESP32 or ESP8266 sets up an Access Point (AP) with the specified SSID and password. It hosts a web server that serves a simple HTML page with ON and OFF buttons. When you click the buttons, corresponding HTTP requests are sent to the ESP32 or ESP8266, and it toggles the state of GPIO12 accordingly.

