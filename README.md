## 🌟TA2CAY RADAR PROJESİ: 

Bu proje, Arduino/ESP32 tabanlı, OLED ekranlı ve servo motorlu bir radar sistemidir. Çevresindeki nesneleri ultrasonik sensör ile tarar ve mesafelerini görsel olarak ekrana yansıtır.

![Radar Projesi Görseli](radar.jpg)

## � Proje Hakkında

Bu proje, klasik ultrasonik radar projelerini bir adım öteye taşıyor. Sadece mesafe ölçmekle kalmaz, topladığı verileri  kullanıcıya sunar.

**Öne Çıkan Farklar:**
*   **Akıllı Görselleştirme:** Tarama yapan ışının arkasında bıraktığı "Fade Trail" (iz) efekti.
*   **Hafıza Sistemi:** Tespit edilen engeller ekrandan hemen silinmez, 10 saniye boyunca "hayalet iz" olarak kalır.
*   **Dinamik Uyarılar:** Cisim yaklaştıkça beliren ve yanıp sönen animasyonlu tehlike sembolleri (`!`, `!!`, `!!!`).
*   **Detaylı Analiz:** Mesafe, Açı, Ortam Sıcaklığı ve Tespit Sayacı gibi verileri anlık gösterir.

## 🛠️ Teknik Özellikler ve Donanım

| Bileşen | Model | Görevi |
|---------|-------|--------|
| **İşlemci** | ESP32 DevKit V1 | Sistemin beyni, yüksek performanslı kontrol. |
| **Göz** | HC-SR04 | Ultrasonik ses dalgaları ile hassas mesafe ölçümü. |
| **Hareket** | SG90 Servo | 0-180 derece hassas tarama hareketi. |
| **Ekran** | SH1106 OLED (1.3") | Yüksek kontrastlı grafik arayüz (I2C). |
| **Sensör** | DHT11 | Ortam sıcaklık ve nem takibi. |

## 📂 Kod Mimarisi

Proje, yönetilebilir ve temiz bir kod yapısı için modüler hale getirilmiştir:

### 1. 🧠 `radar.ino` (Ana Kontrolcü)
Sistemin çekirdeğidir. Donanım başlatma, sensör okuma döngüleri ve ana ekran çizimlerini yönetir.

### 2. 🎨 `radar_helpers.ino` (Görsel Motor)
Projenin "makyaj" kısmıdır.
*   **`drawFadeEffect()`:** Radar tarama izini çizer.
*   **`addObstacle()`:** Tespit edilen engelleri hafızaya alır ve yönetir.
*   **`getDangerSymbol()`:** Tehlike seviyesini analiz eder.

## 🚀 Kurulum ve Kullanım

1.  Bu repoyu indirin veya klonlayın.
2.  `radar.ino` dosyasını Arduino IDE ile açın (Helper dosyası otomatik açılacaktır).
3.  Gerekli kütüphaneleri (`U8g2`, `ESP32Servo`, `DHT`) kütüphane yöneticisinden yükleyin.
4.  Bağlantı şemasını kontrol edip kodu ESP32'ye yükleyin.
5.  **Keyfini çıkarın!** Radarınız açılış animasyonu ile başlayacaktır.


## 📝 Pin Bağlantıları

```
ESP32  <-->  Bileşen
--------------------
D12    <-->  Servo Motor (Sarı/Turuncu Kablo)
D27    <-->  HC-SR04 (Trig)
D26    <-->  HC-SR04 (Echo)
D13    <-->  LED (Opsiyonel)
D14    <-->  Buzzer (Opsiyonel)
D25    <-->  DHT11 (Data)
```

---
**Geliştirici:** TA2CAY
*Bu proje Eğitim ve Hobi amaçlı geliştirilmiştir.*

