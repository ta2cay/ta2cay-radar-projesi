## 🌟TA2CAY RADAR PROJESİ: 

Bu proje, Arduino/ESP32 tabanlı, OLED ekranlı ve servo motorlu bir radar sistemidir. Çevresindeki nesneleri ultrasonik sensör ile tarar ve mesafelerini görsel olarak ekrana yansıtır.

![Radar Projesi Görseli](radar.jpg)

## 🌟 Özellikler

*   **Gelişmiş Görsel Arayüz:** 
    *   Tarama izi efekti (fade trail)
    *   Animasyonlu tehlike sembolleri (!, !!, !!!)
    *   Engel geçmişi takibi
*   **Bilgi Paneli:**
    *   Anlık Açı ve Mesafe
    *   Ortam Sıcaklığı (DHT11 ile)
    *   Tespit Edilen Nesne Sayısı
    *   Toplam Tarama Sayısı
*   **Güç Tasarrufu:**
    *   Optimize edilmiş servo hareketleri
    *   Güç dalgalanmalarını önleyen kademeli başlangıç

## 🛠️ Donanım Listesi

*   ESP32 Geliştirme Kartı
*   HC-SR04 Ultrasonik Mesafe Sensörü
*   SG90 Servo Motor
*   SH1106 OLED Ekran (1.3 inç)
*   DHT11 Sıcaklık ve Nem Sensörü
*   LED ve Buzzer
*   Breadboard ve Jumper Kablolar

## 📚 Kütüphaneler

Bu proje aşağıdaki Arduino kütüphanelerini kullanır:
*   `U8g2` (Ekran için)
*   `ESP32Servo` (Motor için)
*   `DHT sensor library` (Sıcaklık için)

## � Kod Yapısı

Proje kodların daha düzenli olması için iki dosyaya bölünmüştür:

1.  **`radar.ino` (Ana Dosya):**
    *   Setup ve Loop döngüleri
    *   Sensör okuma ve servo kontrolü
    *   Ekran çizim ana fonksiyonları
    *   Pin tanımlamaları

2.  **`radar_helpers.ino` (Yardımcı Dosya):**
    *   **Engel Hafızası:** Eski engelleri ekranda 10 saniye tutan kodlar.
    *   **Fade Trail:** Tarama çizgisinin arkasındaki iz efekti.
    *   **Tehlike Sembolleri:** Mesafeye göre (!, !!, !!!) işaretlerini belirleyen mantık.

## �🚀 Kurulum

1.  `radar.ino` ve `radar_helpers.ino` dosyalarını aynı klasöre koyun.
2.  Arduino IDE ile `radar.ino` dosyasını açın.
3.  Gerekli kütüphaneleri Yöneticiden yükleyin.
4.  ESP32 kartınıza yükleyin.

## 📝 Pin Bağlantıları

| Bileşen | Pin |
|---------|-----|
| Servo   | 12  |
| Trig    | 27  |
| Echo    | 26  |
| LED     | 13  |
| Buzzer  | 14  |
| DHT11   | 25  |

---
**Geliştirici:** TA2CAY
