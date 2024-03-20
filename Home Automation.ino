/*
................................................
:   Title:Simple Home Automation               :
:   Created by: Roy Cuadra                     :
:   Date: 05/11/2023                           :
................................................

*/

//remove or comment this when using esp8266
//#include <WiFi.h>

//Uncomment this when using esp8266 
#include <ESP8266WiFi.h>

const char* ssid = "IoT Home Automation";
const char* password = "admin12345";

WiFiServer server(80);
String header;
String output5State = "off";

const int output5 = 12; // For ESP32: GPIO 12, For ESP8266: D6

const int buzzerPin = 13; //for ESP32: GPIO 13, for esp8266 D6 pin to which the buzzer is connected

void setup() {
  Serial.begin(115200);
  pinMode(output5, OUTPUT);
  digitalWrite(output5, HIGH);
  pinMode(buzzerPin, OUTPUT);
  
  Serial.print("Setting AP (Access Point)...");
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("GET /12/on") >= 0) {
              Serial.println("GPIO 12 on");
              output5State = "on";
              digitalWrite(output5, LOW);
              beep(); 
            } else if (header.indexOf("GET /12/off") >= 0) {
              Serial.println("GPIO 12 off");
              output5State = "off";
              digitalWrite(output5, HIGH);
              beep();
            }

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;background: rgb(2,0,36); background-color:black;}");
            client.println(".button { background-color:blue; border: none;border-radius:30px; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println("body{ padding-top:10px; padding-bottom:10px; border-radius: 20px; border: 1px solid rgb(255, 255, 255); border-style: none; box-shadow: 0 0 30px 0 rgb(71, 70, 70); margin-top: 100px; max-width: 300px ; margin-left: auto; margin-right: auto; background-color: white;  }");
            client.println(".button2 {background-color:red;}</style></head>");
            client.println("<body><h1>Control Panel</h1>");

            if (output5State == "off") {
              client.println("<p><a href=\"/12/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/12/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void beep() {
  digitalWrite(buzzerPin, HIGH); 
  delay(100); 
  digitalWrite(buzzerPin, LOW);
}
