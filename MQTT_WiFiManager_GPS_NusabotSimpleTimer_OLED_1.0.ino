#include <WiFiManager.h>
#include <MQTT.h>
#include <TinyGPS++.h>
#include <NusabotSimpleTimer.h>
#include <DHTesp.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // Reset pin tidak digunakan dengan SSD1306
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

WiFiClient net;
MQTTClient client;
TinyGPSPlus gps;
NusabotSimpleTimer timer;
DHTesp dhtSensor;

const int DHT_PIN = 4;
const int pinLED = 33;

void connectWiFi() {
  Serial.println("\nConnecting to WiFi...");

  WiFiManager wm;
  if (!wm.autoConnect("ESP32", "bangjago")) {
    Serial.println("Failed to connect to WiFi. Retrying...");
    delay(1000);
  }
  Serial.println("Connected to WiFi.");
}

void connectMQTT() {
  Serial.println("Connecting to MQTT...");

  while (!client.connect("dimyati_device1")) {
    Serial.print(".");
    delay(1000);
  }
  client.subscribe("valMQTT/#", 1);
  Serial.println("\nConnected to MQTT.");
}

void subscribe(String &topic, String &data) {
  if (topic == "valMQTT/Relay") {
    if (data == "1") {
      digitalWrite(pinLED, HIGH);
    } else {
      digitalWrite(pinLED, LOW);
    }
  }
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(pinLED, OUTPUT);
  WiFi.mode(WIFI_STA);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Alamat I2C untuk layar OLED biasanya 0x3C
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  connectWiFi();
  timer.setInterval(3000, dataGPS);
  timer.setInterval(1000, dataDHT);
  client.begin("broker.emqx.io", net);
  client.onMessage(subscribe);
  connectMQTT();
}

void loop() {
  client.loop();
  timer.run();

  if (!client.connected()) {
    connectMQTT();
  }

  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  // Membaca data GPS saat tersedia
  while (Serial2.available() > 0) {
    if (gps.encode(Serial2.read())) {
      // Jika data GPS berhasil diuraikan, cetak data ke Serial Monitor
      if (gps.location.isValid()) {
        Serial.print("Latitude: ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("Longitude: ");
        Serial.println(gps.location.lng(), 6);
      } else {
        Serial.println("Data GPS Tidak Valid");
      }
    }
  }

  updateDisplay();  // Memanggil fungsi untuk memperbarui tampilan pada layar OLED
  delay(100);       // Memberikan sedikit delay untuk mengurangi beban prosesor
}

void updateDisplay() {
  display.clearDisplay();               // Membersihkan tampilan sebelum menulis informasi baru
  display.setTextSize(1);               // Ukuran teks
  display.setTextColor(SSD1306_WHITE);  // Warna teks

  // Memeriksa apakah data GPS valid
  display.setCursor(0, 0);
  display.print("GPS: ");
  if (gps.location.isValid()) {
    display.println("Data ready");
  } else {
    display.println("No data");
  }

  // Membaca dan menampilkan data suhu dari sensor DHT22
  display.setCursor(0, 12);
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  display.print("Temperature: ");
  display.print(data.temperature, 2);

  // Teks untuk status LED
  display.setCursor(0, 24);
  display.print("LED Status: ");
  if (digitalRead(pinLED) == HIGH) {
    display.println("ON");
  } else {
    display.println("OFF");
  }

  display.display();  // Menampilkan semua teks secara bersamaan
}


void dataDHT() {
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  client.publish("valMQTT/Themp", String(data.temperature, 2));
  Serial.println("Temperature: " + String(data.temperature, 2));
}

void dataGPS() {
  // Memeriksa apakah data GPS valid
  if (gps.location.isValid()) {
    char latString[15];
    char lngString[15];
    dtostrf(gps.location.lat(), 10, 6, latString);
    dtostrf(gps.location.lng(), 10, 6, lngString);
    client.publish("valMQTT/Lat", latString);
    client.publish("valMQTT/Long", lngString);
  } else {
    Serial.println("Data GPS Tidak Valid");
  }
}
