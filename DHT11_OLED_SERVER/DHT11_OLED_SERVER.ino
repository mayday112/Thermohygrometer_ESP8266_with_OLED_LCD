#include<SPI.h>
#include<Wire.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>
#include "DHT.h"


// Replace with your network credentials
const char* ssid = "OPPO A5s";
const char* password = "22222221";

const char* APssid = "DHT11 server";

#define DHTPIN 0
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
float t = 0.0, h = 0.0;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
//interval 5 s
unsigned long interval = 5000, lastTime = 0;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
  html{
        font-family: Arial, Helvetica, sans-serif;
        font-size: 1.5rem;
        display: flex;
        flex-direction: column;
        justify-content: center;
        align-items: center;
        text-align: center;
    }
    .pre{
            font-family: Arial, Helvetica, sans-serif;
            font-size: 2rem;
            margin-top: -1rem;
        }
    h3{
            font-family: Arial, Helvetica, sans-serif;
        }
  </style>
  <title>DHT11 ESP8266 Web Server</title>
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-QWTKZyjpPEjISv5WaRU9OFeRpok6YctnYmDr5pNlyT2bRjXh0JMhjY6hW+ALEwIH" crossorigin="anonymous">
</head>
<body>
  <h2>ESP8266 DHT11 Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i>
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;</sup>C
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i>
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    %
  </p>

</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;
</script>
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-YvpcrYf0tY3lHB60NNkmXc5s9fDVZLESaAA55NDzOxhy9GkcIdslK1eN7N6jIeHz" crossorigin="anonymous"></script>

</html>)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var) {
  Serial.println(var);
  if (var == "TEMPERATURE") {
    return String(t);
  }
  else if (var == "HUMIDITY") {
    return String(h);
  }
  return String("");
}

void displayConfig() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed!"));
    for (;;);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(16, 25);
  display.print("DHT 11");
  display.display();
}

//config WiFi
void wifiConfig() {
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  display.clearDisplay();
  display.setCursor(1, 1);
  display.print("Connecting to WiFi...");
  display.display();
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  // Print ESP8266 Local IP Address
  IPAddress IP = WiFi.localIP();
  Serial.print("Local IP: ");
  Serial.println(IP);
  //display Local IP to OLED LCD
  display.clearDisplay();
  display.setCursor(1, 1);
  display.print("Local IP :");
  display.setCursor(1, 10);
  display.print(IP);
  display.display();
  delay(10000);
}

//config server
void serverConfig() {
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(h).c_str());
  });

  server.begin();
}

//read tempereature and humidity
void readTempHum() {
  float temp = dht.readTemperature();//read tempereature
  float humi = dht.readHumidity();//read humidity

  //ensure read temp and humi not error
  if (isnan(h) || isnan(t)) {
    Serial.println("Pembacaan gagal");
  } else {
    t = temp;//store temp to variabel t
    h = humi;//store humi to variabel h
  }
}

//Display temperature and humidity to OLED LCD
void displayOled() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  //display temperature
  display.setCursor(1, 1);
  display.print("Temp.:");
  display.setCursor(50, 1);
  display.print(t);
  display.setCursor(80, 1);
  display.print("°C");

  //display humidity
  display.setCursor(1, 20);
  display.print("Humi.:");
  display.setCursor(50, 20);
  display.print(h);
  display.setCursor(80, 20);
  display.print("%");

  display.display();

  //  print tempereature at Serial
  Serial.print("Tempereature : ");
  Serial.print(t);
  Serial.println(" C");
  // print humidity at serial
  Serial.print("Humidity : ");
  Serial.print(h);
  Serial.println("%");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();
  displayConfig();
  wifiConfig();
  serverConfig();

  delay(5000);
}


void loop() {
  // put your main code here, to run repeatedly:

  unsigned long currentMillis = millis();
  if (currentMillis - lastTime >= interval) {
    // save the last time you updated the DHT values
    lastTime = currentMillis;
    readTempHum();
    displayOled();
  }
}
