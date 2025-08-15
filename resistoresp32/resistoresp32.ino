#include <WiFi.h>

// Wi-Fi Access Point credentials
const char* ssid = "ESP32_Resistance";
const char* password = "12345678";

WiFiServer server(80);
String resistanceStr = "--.-- Ω"; // Default display until UNO sends data

void setup() {
  Serial.begin(115200);  // Debug output to PC
  Serial2.begin(115200, SERIAL_8N1, 16, 17); // RX=GPIO16, TX=GPIO17 for UNO link

  WiFi.softAP(ssid, password);
  server.begin();

  Serial.println("AP started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  // Continuously read from UNO
  readSerialData();

  // Handle web client requests
  WiFiClient client = server.available();
  if (!client) return;

  String request = client.readStringUntil('\r');
  client.flush();

  if (request.indexOf("GET / ") >= 0) {
    sendHTMLPage(client);
  } else if (request.indexOf("GET /resistance") >= 0) {
    sendResistance(client);
  } else {
    client.println("HTTP/1.1 404 Not Found");
    client.println("Content-type:text/plain");
    client.println();
    client.println("404 Not Found");
  }

  delay(1);
  client.stop();
}

void readSerialData() {
  static String inputString = "";
  while (Serial2.available()) {
    char c = Serial2.read();
    if (c == '\n') {
      inputString.trim();
      if (inputString.length() > 0) {
        resistanceStr = inputString; // Update displayed value
      }
      inputString = "";
    } else {
      inputString += c;
    }
  }
}

void sendHTMLPage(WiFiClient& client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  client.println(R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>Resistance Meter</title>
<style>
  body { background: black; color: #0f0; font-family: monospace; text-align: center; padding-top: 50px; }
  .resistance { font-size: 80px; letter-spacing: 15px; font-weight: bold; }
</style>
</head>
<body>
<h1>Resistance Meter</h1>
<div class="resistance" id="resistance">--.-- Ω</div>
<script>
  function fetchResistance() {
    fetch('/resistance').then(r => r.text()).then(t => {
      document.getElementById('resistance').innerText = t;
    });
  }
  setInterval(fetchResistance, 1000);
  fetchResistance();
</script>
</body>
</html>
)rawliteral");
}

void sendResistance(WiFiClient& client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/plain");
  client.println();
  client.println(resistanceStr);
}
