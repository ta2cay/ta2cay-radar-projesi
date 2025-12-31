# TA2CAY Radar Projesi

ESP32, HC-SR04 Ultrasonik Sensör, Servo Motor ve SH1106 OLED ekran kullanan gelişmiş radar sistemi.

![Radar Görseli](radar.jpg)

## Özellikler

- **Görsel Efektler**
  - Tarama izi (fade trail) efekti
  - Engel geçmişi takibi (10 saniye hafıza)
  - Animasyonlu tehlike sembolleri (!, !!, !!!)
  
- **Anlık Veriler**
  - Mesafe ölçümü (0-50 cm)
  - Tarama açısı (0-180°)
  - Ortam sıcaklığı (DHT11)
  - Tespit sayacı ve tarama istatistikleri

## Donanım Gereksinimleri

| Bileşen | Model | Adet |
|---------|-------|------|
| Geliştirme Kartı | ESP32 DevKit | 1 |
| Ekran | SH1106 OLED 1.3" (I2C) | 1 |
| Mesafe Sensörü | HC-SR04 | 1 |
| Servo Motor | SG90 (veya eşdeğeri) | 1 |
| Sıcaklık Sensörü | DHT11 | 1 |
| LED | Standart 5mm | 1 |
| Buzzer | Pasif Buzzer | 1 |

## Pin Bağlantıları

### Servo Motor (SG90)
- **Sinyal (Turuncu/Sarı)** → ESP32 GPIO 12
- **+5V (Kırmızı)** → ESP32 VIN veya Harici 5V
- **GND (Kahverengi)** → ESP32 GND

### Ultrasonik Sensör (HC-SR04)
- **VCC** → ESP32 5V (VIN)
- **Trig** → ESP32 GPIO 27
- **Echo** → ESP32 GPIO 26
- **GND** → ESP32 GND

### OLED Ekran (SH1106 I2C)
- **VCC** → ESP32 3.3V
- **GND** → ESP32 GND
- **SDA** → ESP32 GPIO 21
- **SCL** → ESP32 GPIO 22

### Sıcaklık Sensörü (DHT11)
- **VCC** → ESP32 3.3V
- **Data** → ESP32 GPIO 25
- **GND** → ESP32 GND

### LED
- **Anot (+)** → ESP32 GPIO 13 (220Ω direnç ile)
- **Katot (-)** → ESP32 GND

### Buzzer
- **Pozitif (+)** → ESP32 GPIO 14
- **Negatif (-)** → ESP32 GND

> **Not:** Servo motor yüksek akım çekebilir. Güç sorunları yaşarsanız servoya harici 5V güç kaynağı bağlayın (GND'leri ortak yapın).

## Kurulum

### 1. Kütüphaneleri Yükleyin

Arduino IDE → Sketch → Include Library → Manage Libraries

- `U8g2` (OLED ekran)
- `ESP32Servo` (Servo motor kontrolü)
- `DHT sensor library` (Sıcaklık sensörü)

### 2. Kodu Yükleyin

1. `radar.ino` ve `radar_helpers.ino` dosyalarını indirin
2. Arduino IDE ile `radar.ino` dosyasını açın
3. ESP32 kartınızı seçin (Tools → Board → ESP32 Dev Module)
4. Doğru portu seçin
5. Upload butonuna basın

## Kod Yapısı

### `radar.ino` (Ana Dosya)
- Donanım başlatma ve pin tanımlamaları
- Ana döngü (setup ve loop)
- Sensör okuma ve servo kontrolü
- Ekran çizim fonksiyonları

### `radar_helpers.ino` (Yardımcı Fonksiyonlar)
- `addObstacle()` - Engel hafıza yönetimi
- `drawFadeEffect()` - Tarama izi çizimi
- `drawObstacleHistory()` - Geçmiş engelleri görselleştirme
- `getDangerSymbol()` - Tehlike seviyesi belirleme

## Lisans

MIT License - Detaylar için [LICENSE](LICENSE) dosyasına bakın.

## Geliştirici

**TA2CAY** - Eğitim ve hobi amaçlı geliştirilmiştir.
