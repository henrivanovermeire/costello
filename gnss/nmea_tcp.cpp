#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <WiFi.h> // Library for Wi-Fi functionality

// Replace with your network credentials
#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "Telenet3943314";
const char* password = "rnhzmmg6tkyVhefW";

// TCP Server settings
WiFiServer tcpServer(5000);  // Port 5000 for NMEA TCP access

// GPS UART settings
#define GPS_RX 16  // GPIO 16 = RX2
#define GPS_TX 17  // TX2 (not used here but needed for Serial2)
HardwareSerial GPS(2); // Use UART2

void setup() {
  Serial.begin(115200);  // For debug output
  delay(1000);

  // Start GPS Serial
  GPS.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);  // Most GPS modules use 9600 baud

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start TCP server
  tcpServer.begin();
  tcpServer.setNoDelay(true);
  Serial.println("TCP server started on port 5000");
}

void loop() {
  // Check for new client
  WiFiClient client = tcpServer.available();

  // If a client is connected
  if (client) {
    Serial.println("Client connected.");

    // While client is connected
    while (client.connected()) {
      // Read from GPS and send to TCP client
      while (GPS.available()) {
        char c = GPS.read();
        client.write(c);      // Forward GPS data to client
      }

      // Handle client disconnect
      if (!client.connected()) {
        Serial.println("Client disconnected.");
        client.stop();
        break;
      }
    }
  }
}
