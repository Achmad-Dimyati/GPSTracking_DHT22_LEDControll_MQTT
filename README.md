# GPSTracking_DHT22_LEDControll_MQTT
Proyek ini menggunakan ESP32 untuk membaca data dari sensor suhu DHT22 dan modul GPS, serta menampilkan informasi tersebut bersama dengan status koneksi WiFi, status koneksi MQTT, dan status LED pada layar OLED 0,96 inci. Proyek ini juga memanfaatkan WiFiManager untuk manajemen koneksi WiFi dan MQTT untuk komunikasi data.
## Komponen Hardware:
- Board ESP32
- Sensor Suhu dan Kelembapan DHT22
- Modul GPS
- Layar OLED 0,96
- LED
- Resistor dan kabel penghubung
## Komponen Software & Liblary:
- Arduino IDE
- Library WiFiManager
- Library MQTT
- Library TinyGPS++
- Library NusabotSimpleTimer
- Library DHTesp
- Library Adafruit GFX dan SSD1306 untuk OLED
## Fitur:
- Koneksi ke WiFi menggunakan WiFiManager
- Pengiriman data ke MQTT
- Membaca dan menampilkan data lokasi GPS
- Membaca dan menampilkan suhu dan kelembapan dari sensor DHT22
- Mengontrol LED berdasarkan perintah MQTT
- Menampilkan informasi di layar OLED
## Cara Kerja:
- Koneksi WiFi: ESP32 terhubung ke jaringan WiFi menggunakan WiFiManager.
- Koneksi MQTT: ESP32 terhubung ke broker MQTT (broker.emqx.io) dan berlangganan ke topik yang relevan untuk menerima perintah dan menerbitkan data sensor.
## Pembacaan Sensor:
- Modul GPS: Membaca data GPS secara terus-menerus dari modul GPS.
- Sensor DHT22: Membaca suhu dan kelembapan secara berkala.
- Tampilan Data: Memperbarui layar OLED dengan data GPS, suhu, kelembapan, dan status LED terbaru.
- Kontrol LED: Menerima pesan MQTT untuk menyalakan atau mematikan LED berdasarkan perintah yang diterima.
## Tampilan Aplikasi Android:
![1720455666922](https://github.com/Achmad-Dimyati/GPSTracking_DHT22_LEDControll_MQTT/assets/167073850/ffe0e5a4-63e0-4083-a656-4b3b1999a90e)
