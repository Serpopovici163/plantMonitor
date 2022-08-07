#include <ESP8266WiFi.h>
#include <DHT.h>

#define ENV_DHT 2

//https://esp32io.com/tutorials/esp32-soil-moisture-sensor

const char* ssid     = "stuff";
const char* password = "yeanoperoo";

WiFiServer server(80);
DHT dht(ENV_DHT, DHT11);

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
  dht.begin();
}

//snapshot link http://192.168.23.6:88/cgi-bin/CGIProxy.fcgi?cmd=snapPicture2&usr=esp12e&pwd=password1!&

void loop() {  
  WiFiClient client = server.available();

  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            client.println(String(dht.readTemperature()));
            client.println(String("sunlight"));
            client.println(String(dht.readHumidity()));
            client.println(String(analogRead(A0)));
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
  }
}
