/*
................................................
:   Title: Home Automation (Single Toggle Button)
:   Created by: Roy Cuadra
:   Date: 08/22/2025
................................................
*/

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

// Access Point credentials (open)
const char* ssid = "HOME AUTOMATION_AP";

// Captive portal DNS
const byte DNS_PORT = 53;
DNSServer dnsServer;

// Async Web Server & WebSocket
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");  

// Relay pin and state
const int output5 = 2;   // D4 pin
String output5State = "off";

// HTML page (toggle button + glow)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Home Automation System</title>
  <style>
    html, body { margin: 0; height: 100%; font-family: Helvetica; background-color:#121212; }
    body { display:flex; align-items:center; justify-content:center; min-height:100vh; }
    .container { padding:20px; border-radius:20px; width:320px; background-color:#1e1e1e;
                 box-shadow:0 0 30px rgba(0,0,0,0.6); color:#e0e0e0; text-align:center; transition:all 0.3s ease-in-out; }
    h1 { margin:0 0 10px; font-size:28px; color:#ffffff; }
    .tagline { font-size:14px; color:#9e9e9e; margin-bottom:20px; }
    .button { border:none; border-radius:30px; padding:16px 40px; font-size:30px; cursor:pointer; margin:10px;
              color:white; background-color:#555; transition: all 0.3s; }
    .button.on { background-color:#2196f3; box-shadow:0 0 15px #2196f3; }
    .button.off { background-color:#f44336; box-shadow:0 0 15px #f44336; }
    .status { font-size:20px; margin-top:20px; color:#bdbdbd; }
    footer { margin-top:20px; font-size:12px; color:#616161; }

    /* Multi-color breathing glow */
    @keyframes rgbGlow {
      0%   { box-shadow: 0 0 10px #ff0000, 0 0 20px #ff0000, 0 0 40px #ff0000; border-color:#ff0000; }
      25%  { box-shadow: 0 0 20px #00ff00, 0 0 40px #00ff00, 0 0 80px #00ff00; border-color:#00ff00; }
      50%  { box-shadow: 0 0 30px #0000ff, 0 0 60px #0000ff, 0 0 100px #0000ff; border-color:#0000ff; }
      75%  { box-shadow: 0 0 20px #ff00ff, 0 0 40px #ff00ff, 0 0 80px #ff00ff; border-color:#ff00ff; }
      100% { box-shadow: 0 0 10px #ff0000, 0 0 20px #ff0000, 0 0 40px #ff0000; border-color:#ff0000; }
    }
    .glow {
      animation: rgbGlow 6s infinite ease-in-out;
      border: 1px solid #fff;
    }
  </style>
</head>
<body>
  <div class="container" id="panel">
    <h1>Home Automation System</h1>
    <p class="tagline">Making Homes Smarter with Wi-Fi</p>

    <button id="toggleBtn" class="button off">Turn ON</button>
    <p class="status">Relay is <b id="relayStatus">off</b></p>

    <footer>Created by Roy Cuadra</footer>
  </div>

  <script>
    var gateway = `ws://${window.location.hostname}/ws`;
    var websocket;

    window.addEventListener('load', onLoad);

    function onLoad() {
      initWebSocket();
      document.getElementById('toggleBtn').addEventListener('click', toggleRelay);
    }

    function initWebSocket() {
      websocket = new WebSocket(gateway);
      websocket.onopen    = () => console.log('WebSocket Open');
      websocket.onclose   = () => setTimeout(initWebSocket, 2000);
      websocket.onmessage = (event) => updateRelay(event.data);
    }

    function toggleRelay() {
      websocket.send("toggle");
    }

    function updateRelay(state) {
      const statusEl = document.getElementById('relayStatus');
      const panel = document.getElementById('panel');
      const btn = document.getElementById('toggleBtn');

      statusEl.innerText = state;

      if (state === "on") {
        panel.classList.add("glow");
        btn.classList.remove("off");
        btn.classList.add("on");
        btn.innerText = "Turn OFF";
      } else {
        panel.classList.remove("glow");
        btn.classList.remove("on");
        btn.classList.add("off");
        btn.innerText = "Turn ON";
      }
    }
  </script>
</body>
</html>
)rawliteral";


// Broadcast state to all clients
void notifyClients() {
  ws.textAll(output5State);
}

// Handle WebSocket messages
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    String msg = (char*)data;

    if (msg == "toggle") {
      if (output5State == "off") {
        output5State = "on";
        digitalWrite(output5, LOW);
      } else {
        output5State = "off";
        digitalWrite(output5, HIGH);
      }
    }
    notifyClients();
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      client->text(output5State); // Send current state
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(output5, OUTPUT);
  digitalWrite(output5, HIGH); // OFF initially

  // Start Access Point (open, max 8 clients)
  WiFi.softAP(ssid, nullptr, 1, 0, 8);
  WiFi.setSleep(false);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // DNS for captive portal
  dnsServer.start(DNS_PORT, "*", IP);

  // WebSocket
  ws.onEvent(onEvent);
  server.addHandler(&ws);

  // Captive portal redirects
  server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request){ request->redirect("/"); });
  server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request){ request->redirect("/"); });
  server.on("/fwlink", HTTP_GET, [](AsyncWebServerRequest *request){ request->redirect("/"); });
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ request->send_P(200, "text/html", index_html); });
  server.onNotFound([](AsyncWebServerRequest *request){ request->redirect("/"); });

  // Start server
  server.begin();
}

void loop() {
  dnsServer.processNextRequest();
}
