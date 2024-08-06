Pengukur Suhu ruangan menggunakan sensor DHT11 dengan papan Wemos D1 Mini(ESP8266) dan display OLED LCD(SSD1306)
Fitur:
1. pengukuran suhu dan kelembaban real-time
2. antar muka display oled lcd dan web browser(jaringan lokal)

Skema :

![image](https://github.com/user-attachments/assets/97127c73-1682-45c3-9772-f03dcda2ea59)

ESP8266 to OLED LCD :

GPIO 5 (D1): SCL

GPIO 4 (D2): SDA



ESP8266 to DHT11 :

GPIO 0 (D3): Data



Sebelum upload code pastikan untuk install terlebih dahulu papan ESP8266 pada Arduino IDE kalian, berikut adalah tautan cara pasangnya:
https://www.mahirelektro.com/2021/04/cara-install-board-esp8266-nodemcu-di-Arduino-IDE.html

install juga pustaka-pustaka berikut :
1.  Adafruit SSD1306 https://github.com/adafruit/Adafruit_SSD1306
2.  Adafruit GFX Library https://github.com/adafruit/Adafruit-GFX-Library
3.  DHT Sensor Library https://github.com/adafruit/DHT-sensor-library
4.  Adafruit Unified Sensor https://github.com/adafruit/Adafruit_Sensor
5.  ESPAsyncWebServer library https://github.com/me-no-dev/ESPAsyncWebServer/archive/master.zip

cara install library arduino :
https://www.mahirelektro.com/2021/04/cara-install-library-arduino.html#:~:text=Bagaimana%20Cara%20Install%20Library%20Arduino%3F%201%20Menggunakan%20Library,secara%20manual%20%20ke%20dalam%20direktori%20libraries.%20

gunakan board :

![image](https://github.com/user-attachments/assets/29c4b6d0-d627-412f-a2da-d03f20b59a8c)

pada bagian ini ganti dengan ssid dan password kalian

![image](https://github.com/user-attachments/assets/2c04e031-9e4c-45de-ba31-282452aa503c)

lalu klik upload pada Arduino IDE

setelah selesai board akan restart sendiri atau perlu di tekan tombol restart pada board. 
diawal board akan menghubungkan ke WiFi setelah tersambung beberapa detik LCD akan menampilkan IP Address dari board yang nantinya dapat digunakan untuk menampilkan data sensor dari web browser pada komputer yang terhubung pada jaringan yang sama dengan board 

![image](https://github.com/user-attachments/assets/ac4033fc-b9e1-446f-9581-cb4b32e517a2)


credits :

https://randomnerdtutorials.com/esp32-esp8266-dht-temperature-and-humidity-oled-display/
https://randomnerdtutorials.com/esp8266-dht11dht22-temperature-and-humidity-web-server-with-arduino-ide/
