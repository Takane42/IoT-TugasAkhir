# Sistem Monitoring Suhu & Kelembaban IoT

Sistem monitoring IoT lengkap yang mengumpulkan data suhu dan kelembaban dari sensor ESP8266 dan menampilkannya dalam antarmuka web modern dengan pembaruan real-time dan kemampuan kontrol LED.

## Gambaran Umum Proyek

Proyek ini terdiri dari dua komponen utama:
- **ESP8266** - Memantau suhu dan kelembaban menggunakan sensor DHT11 dan memungkinkan kontrol jarak jauh LED
- **Antarmuka Web** - Menampilkan data sensor dalam grafik real-time dan menyediakan tombol kontrol LED

## Fitur

- 📊 **Visualisasi Data Real-time** - Grafik langsung untuk pembacaan suhu dan kelembaban
- 💡 **Kontrol LED** - Kendali jarak jauh tiga LED yang terhubung ke ESP8266
- 📱 **Desain Responsif** - Berfungsi di perangkat desktop dan mobile
- 🔔 **Sistem Peringatan** - Peringatan suara saat suhu atau kelembaban melebihi ambang batas
- 💾 **Pencatatan Data** - Menyimpan pembacaan sensor dalam database MySQL

## Struktur Proyek

```
uas-iot/
├── esp8266-program/
│   └── esp8266-program.ino   # Kode Arduino ESP8266
└── website/
    ├── index.html            # File HTML utama
    ├── assets/
    │   ├── css/
    │   │   └── style.css     # CSS yang diekstrak
    │   ├── js/
    │   │   ├── app.js        # Logika aplikasi utama (penanganan MQTT, grafik)
    │   │   └── custom.js     # Peningkatan dan penyesuaian UI kustom
    │   └── images/           # Untuk aset gambar di masa mendatang
    └── api/
        └── save_data.php     # API backend untuk menyimpan data ke database
```

## Kebutuhan Perangkat Keras

- NodeMCU ESP8266
- Sensor Suhu & Kelembaban DHT11
- 3 LED (terhubung ke GPIO12/D6, GPIO13/D7, GPIO15/D8)
- Kabel jumper
- Breadboard
- Catu daya 5V

## Kebutuhan Perangkat Lunak

- Arduino IDE dengan dukungan board ESP8266
- Web server dengan dukungan PHP dan MySQL
- Browser web modern (Chrome, Firefox, Edge, dll.)

## Petunjuk Pengaturan

### Pengaturan ESP8266

1. Hubungkan sensor DHT11 ke GPIO2 (D4) dari ESP8266
2. Hubungkan LED ke GPIO12 (D6), GPIO13 (D7), dan GPIO15 (D8) dengan resistor yang sesuai
3. Buka Arduino IDE dan instal pustaka yang diperlukan:
   - Pustaka sensor DHT
   - PubSubClient (MQTT)
   - ESP8266WiFi
   - ArduinoJSON
   - LiquidCrystal_I2C (jika menggunakan layar LCD)

4. Perbarui kredensial WiFi dan MQTT di file `esp8266-program.ino`:
   ```cpp
   const char* ssid = "SSID_WIFI_ANDA";
   const char* password = "PASSWORD_WIFI_ANDA";
   ```

5. Unggah kode ke board ESP8266 Anda

### Pengaturan Antarmuka Web

1. Siapkan web server dengan dukungan PHP dan MySQL
2. Buat database MySQL bernama `iot_database` dengan struktur berikut:
   ```sql
   CREATE TABLE `data` (
     `id` int(11) NOT NULL AUTO_INCREMENT,
     `suhu` float NOT NULL,
     `kelembapan` float NOT NULL,
     `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
     PRIMARY KEY (`id`)
   );
   ```

3. Perbarui detail koneksi database di `api/save_data.php`:
   ```php
   $dbHost = "localhost";
   $dbUser = "username_database_anda";
   $dbPass = "password_database_anda";
   $dbName = "iot_database";
   ```

4. Perbarui informasi broker MQTT di `assets/js/app.js` jika diperlukan:
   ```javascript
   const brokerHost = "mqtt.revolusi-it.com";
   const brokerPort = 9001;
   const nim = "NIM_ANDA";
   ```

5. Unggah semua file ke web server Anda

## Cara Kerja

1. **Pengumpulan Data**: ESP8266 membaca data suhu dan kelembaban dari sensor DHT11
2. **Transmisi Data**: Data dikirim ke broker MQTT dalam format JSON
3. **Antarmuka Web**: Antarmuka web berlangganan topik MQTT dan menampilkan data real-time
4. **Kontrol LED**: Antarmuka web mengirimkan pesan MQTT untuk mengendalikan LED yang terhubung ke ESP8266
5. **Penyimpanan Data**: Data disimpan dalam database MySQL melalui panggilan AJAX ke save_data.php

## Topik MQTT

- **Topik Berlangganan**: `iot/G.231.22.0154` - Untuk menerima data sensor
- **Topik Publikasi**: `iot/G.231.22.0154/sub` - Untuk mengirim perintah kontrol LED

## Perintah Kontrol LED

- `LED1_ON` / `LED1_OFF` - Mengontrol LED yang terhubung ke GPIO12 (D6)
- `LED2_ON` / `LED2_OFF` - Mengontrol LED yang terhubung ke GPIO13 (D7)
- `LED3_ON` / `LED3_OFF` - Mengontrol LED yang terhubung ke GPIO15 (D8)

## Aturan Peringatan

- **Suhu**:
  - Bunyi bip 3x saat suhu > 31°C
  - Bunyi bip 2x saat suhu antara 30°C dan 31°C
  - Bunyi bip 1x saat suhu antara 29°C dan 30°C

- **Kelembaban**:
  - Bunyi bip 3x saat kelembaban ≥ 70%
  - Bunyi bip 1x saat kelembaban antara 60% dan 70%

## Kredit

- Dibuat oleh Alif Naufal Ilyas (G.231.22.0154)
- Broker MQTT disediakan oleh Revolusi IT
- Library: Chart.js, MQTT.js, Bootstrap, dan lainnya

## Lisensi

Proyek ini dilisensikan di bawah Lisensi MIT - lihat file LICENSE untuk detailnya.
