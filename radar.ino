#include <DHT.h>
#include <ESP32Servo.h>
#include <U8g2lib.h>
#include <Wire.h>

// Pin tanımlamaları
#define SERVO_PIN 12
#define TRIG_PIN 27
#define ECHO_PIN 26
#define PIN_LED 13
#define PIN_BUZZER 14
#define PIN_DHT 25

// DHT sensör
#define DHTTYPE DHT11
DHT dht(PIN_DHT, DHTTYPE);

// Sistem durumu
bool systemReady = false; // Sistem hazır oldu mu?

// OLED ekran (1.3" 128x64 I2C - U8g2)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
// U8g2 SH1106 128x64 I2C
U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

Servo servo;

// Değişkenler
float temperature = 0.0;
float humidity = 0.0;
unsigned long lastDHTRead = 0;
const unsigned long DHT_INTERVAL =
    5000; // 5 saniye - güç tasarrufu için daha seyrek

// Radar koordinatları - alt orta yarım daire
const int CENTER_X = 64;   // Ekranın ortası
const int CENTER_Y = 63;   // Alt kenar
const int MAX_RADIUS = 45; // Maksimum yarıçap (Biraz daha büyütüldü)
const int RINGS = 4;       // Mesafe halkaları

// Tarama çizgisi için
int lastAngle = -1;

// Engel geçmişi sistemi
#define MAX_OBSTACLES 20
struct Obstacle {
  int angle;
  long distance;
  unsigned long timestamp;
  bool active;
};
Obstacle obstacleHistory[MAX_OBSTACLES];
int obstacleCount = 0;

// İstatistikler
long closestDistance = 999;
int totalDetections = 0;
long totalDistance = 0;
unsigned long scanCount = 0;

// Animasyon
unsigned long lastBlink = 0;
bool blinkState = false;

// Fade effect için son birkaç açı
#define FADE_LENGTH 8
int fadeAngles[FADE_LENGTH];
int fadeIndex = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("OLED Radar Başlatılıyor...");

  // Pin modları
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  // Fade array başlat
  for (int i = 0; i < FADE_LENGTH; i++) {
    fadeAngles[i] = -1;
  }

  // DHT başlat
  dht.begin();

  // I2C başlat
  Wire.begin();

  // U8g2 OLED başlat
  display.begin();
  Serial.println("U8g2 OLED başlatıldı!");

  // ========== AÇILIŞ ANIMASYONU - 3 AŞAMA ==========

  // AŞAMA 1: Animasyonlu Logo (Büyüyerek Giriş)
  for (int scale = 0; scale <= 10; scale++) {
    display.clearBuffer();

    // Dış çerçeve - ölçeklendirerek büyüt
    int frameSize = 40 + (scale * 4);
    int frameX = 64 - (frameSize / 2);
    int frameY = 32 - (frameSize / 2);
    display.drawRFrame(frameX, frameY, frameSize, frameSize, 3);

    // Başlık - fade in efekti (piksel atlayarak)
    if (scale > 3) {
      display.setFont(u8g2_font_ncenB10_tr);
      int textWidth = display.getStrWidth("TA2CAY");
      display.drawStr((128 - textWidth) / 2, 28, "TA2CAY");
    }

    // Alt başlık - daha sonra görünsün
    if (scale > 6) {
      display.setFont(u8g2_font_6x10_tr);
      int subWidth = display.getStrWidth("RADAR");
      display.drawStr((128 - subWidth) / 2, 42, "RADAR");
    }

    display.sendBuffer();
    delay(60);
  }
  delay(300);

  // Alt bilgi - küçük font
  display.setFont(u8g2_font_4x6_tr);
  int textWidth = display.getStrWidth("v2.0");
  display.drawStr((128 - textWidth) / 2, 52, "v2.0");

  display.sendBuffer();
  delay(2500);

  // AŞAMA 2: İlerleme Çubuğu (Radar Yükleniyor)
  display.clearBuffer();
  display.setFont(u8g2_font_ncenB08_tr);
  int msgWidth = display.getStrWidth("RADAR YUKLENIYOR...");
  display.drawStr((128 - msgWidth) / 2, 20, "RADAR YUKLENIYOR...");

  // İlerleme çubuğu çerçevesi
  display.drawRFrame(14, 28, 100, 12, 2);
  display.sendBuffer();
  delay(200);

  // İlerleme animasyonu
  for (int progress = 0; progress <= 96; progress += 4) {
    display.setDrawColor(1);
    display.drawBox(16, 30, progress, 8);
    display.sendBuffer();
    delay(40);
  }
  delay(300);

  // AŞAMA 3: Sonar Dalgası (Tarama Başlıyor)
  display.clearBuffer();
  display.setFont(u8g2_font_6x10_tr);
  int radarWidth = display.getStrWidth("TARAMA BASLIYOR!");
  display.drawStr((128 - radarWidth) / 2, 12, "TARAMA BASLIYOR!");

  // Merkez nokta
  int centerX = 64;
  int centerY = 42;

  // Genişleyen dalgalar
  for (int wave = 0; wave <= 30; wave += 3) {
    display.clearBuffer();

    // Başlık
    display.setFont(u8g2_font_6x10_tr);
    display.drawStr((128 - radarWidth) / 2, 12, "TARAMA BASLIYOR!");

    // Merkez noktası
    display.drawDisc(centerX, centerY, 2);

    // 3 dalga çemberi (büyüyerek kayboluyor)
    if (wave >= 3)
      display.drawCircle(centerX, centerY, wave);
    if (wave >= 6)
      display.drawCircle(centerX, centerY, wave - 6);
    if (wave >= 9)
      display.drawCircle(centerX, centerY, wave - 12);

    display.sendBuffer();
    delay(50);
  }

  // Son mesaj
  display.clearBuffer();
  display.setFont(u8g2_font_ncenB08_tr);
  int readyWidth = display.getStrWidth("BASLIYORUZ!");
  display.drawStr((128 - readyWidth) / 2, 35, "BASLIYORUZ!");
  display.sendBuffer();
  delay(800);

  // ========== AÇILIŞ ANIMASYONU BİTTİ ==========

  // Servo başlat - güç için ÇOK yavaş başlat
  servo.attach(SERVO_PIN);
  delay(200);
  servo.write(90); // Ortadan başlat
  delay(1000);     // Servo sabitlensin

  // Birkaç küçük hareket - adaptörü alıştır
  for (int i = 0; i < 3; i++) {
    servo.write(85);
    delay(300);
    servo.write(95);
    delay(300);
  }
  servo.write(90);
  delay(500);

  systemReady = true; // Artık buzzer aktif olabilir

  // İlk ekranı çiz
  display.clearBuffer();
  drawStaticRadar();
  display.sendBuffer();
}

void loop() {
  // DHT sensör oku
  if (millis() - lastDHTRead >= DHT_INTERVAL) {
    updateTemp();
    lastDHTRead = millis();
  }

  // Her taramada en yakın mesafeyi sıfırla
  closestDistance = 999;

  // 0'dan 180'e tarama
  for (int angle = 0; angle <= 180; angle += 5) {
    servo.write(angle);
    delay(150); // Güç stabilitesi için daha uzun bekleme

    long dist = getDistance();

    // Ekranı güncelle
    updateRadarDisplay(angle, dist);

    // LED ve buzzer kontrolü
    if (dist > 0 && dist < 50) {
      digitalWrite(PIN_LED, HIGH);
      // Buzzer - sadece sistem hazırsa (başlangıçta devre dışı)
      if (systemReady) {
        for (int i = 0; i < 10; i++) {
          digitalWrite(PIN_BUZZER, HIGH);
          delayMicroseconds(400);
          digitalWrite(PIN_BUZZER, LOW);
          delayMicroseconds(400);
        }
      }
    } else {
      digitalWrite(PIN_LED, LOW);
      digitalWrite(PIN_BUZZER, LOW);
    }
  }

  // 180'den 0'a tarama
  for (int angle = 180; angle >= 0; angle -= 5) {
    servo.write(angle);
    delay(150); // Güç stabilitesi için daha uzun bekleme

    long dist = getDistance();

    updateRadarDisplay(angle, dist);

    // LED ve buzzer kontrolü
    if (dist > 0 && dist < 50) {
      digitalWrite(PIN_LED, HIGH);
      // Buzzer - sadece sistem hazırsa (başlangıçta devre dışı)
      if (systemReady) {
        for (int i = 0; i < 10; i++) {
          digitalWrite(PIN_BUZZER, HIGH);
          delayMicroseconds(400);
          digitalWrite(PIN_BUZZER, LOW);
          delayMicroseconds(400);
        }
      }
    } else {
      digitalWrite(PIN_LED, LOW);
      digitalWrite(PIN_BUZZER, LOW);
    }
  }
}

void drawStaticRadar() {
  // Yarım daire grid çiz (5 halka) ve mesafe etiketleri ekle
  display.setFont(u8g2_font_4x6_tr);
  for (int i = 1; i <= RINGS; i++) {
    int r = (MAX_RADIUS * i) / RINGS;
    for (int angle = 0; angle <= 180; angle += 2) {
      float rad = radians(angle);
      int x = CENTER_X + r * cos(rad);
      int y = CENTER_Y - r * sin(rad);
      if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        display.drawPixel(x, y);
      }
    }

    // Mesafe etiketlerini ekle (10, 20, 30, 40, 50)
    int distance = i * 10;
    char distLabel[4];
    sprintf(distLabel, "%d", distance);
    // 90 derece (üstte) etiket yerleştir
    int labelX = CENTER_X - 4;
    int labelY = CENTER_Y - r + 3;
    if (labelY >= 5 && labelY < SCREEN_HEIGHT - 5) {
      display.drawStr(labelX, labelY, distLabel);
    }
  }

  // Merkez noktası
  display.drawPixel(CENTER_X, CENTER_Y);

  // Açı çizgileri (0°, 45°, 90°, 135°, 180°)
  for (int angle = 0; angle <= 180; angle += 45) {
    float rad = radians(angle);
    int x = CENTER_X + MAX_RADIUS * cos(rad);
    int y = CENTER_Y - MAX_RADIUS * sin(rad);
    display.drawLine(CENTER_X, CENTER_Y, x, y);
  }
}

void updateRadarDisplay(int angle, long distance) {
  display.clearBuffer();

  // *** MANUEL TERS MOD *** - Tüm ekranı beyaz yap (RADAR başlığı tekniği!)
  display.setDrawColor(1);
  display.drawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // Tüm ekran beyaz

  // Şimdi her şeyi SIYAH çizeceğiz
  display.setDrawColor(0);

  // Statik radar gridini çiz (siyah)
  drawStaticRadar();

  // Fade effect - tarama izi
  updateFadeTrail(angle);
  drawFadeEffect();

  // Geçmiş engelleri göster
  drawObstacleHistory();

  // Tarama çizgisini çiz (ana çizgi)
  float rad = radians(angle);
  int lineX = CENTER_X + MAX_RADIUS * cos(rad);
  int lineY = CENTER_Y - MAX_RADIUS * sin(rad);
  display.drawLine(CENTER_X, CENTER_Y, lineX, lineY);

  // Engel varsa göster ve kaydet
  if (distance > 0 && distance <= 50) {
    int obstacleR = (distance * MAX_RADIUS) / 50;
    int obstacleX = CENTER_X + obstacleR * cos(rad);
    int obstacleY = CENTER_Y - obstacleR * sin(rad);

    // Engel noktası (büyük ve belirgin)
    display.drawDisc(obstacleX, obstacleY, 3);
    display.drawCircle(obstacleX, obstacleY, 5);

    // Engeli kaydet
    addObstacle(angle, distance);
  }

  // Tarama sayacını artır
  scanCount++;

  // Yanıp sönme animasyonu
  if (millis() - lastBlink > 500) {
    blinkState = !blinkState;
    lastBlink = millis();
  }

  // === YENİ LAYOUT - SADELEŞTİRİLMİŞ ===

  char buffer[20];
  display.setFont(u8g2_font_5x7_tr);

  // --- SOL SÜTUN ---

  // 1. Açı (İKONLU!) - Pusula/açı ikonu
  display.setFont(u8g2_font_open_iconic_embedded_1x_t);
  display.drawGlyph(0, 8, 75); // 75 = dial/açı ikonu

  // Açı değerini daha büyük fontla yaz
  display.setFont(u8g2_font_7x13B_tr); // Daha büyük, kalın font
  sprintf(buffer, "%d%c", angle, 176);
  display.drawStr(10, 8, buffer);

  // 2. Sıcaklık (İKONLU!) - Termometre ikonu
  if (temperature > 0) {
    // Termometre ikonu çiz (Open Iconic Thing set - 1x boyut)
    display.setFont(u8g2_font_open_iconic_thing_1x_t);
    display.drawGlyph(0, 18, 64); // 64 = termometre ikonu kodu

    // Sıcaklık değerini ikonun yanına yaz
    display.setFont(u8g2_font_5x7_tr);
    sprintf(buffer, "%dC", (int)temperature);
    display.drawStr(10, 18, buffer); // İkonun yanına yazdır
  }

  // --- SAĞ SÜTUN ---

  // 1. Mesafe (İKONLU!) - Cetvel ikonu
  display.setFont(u8g2_font_open_iconic_thing_1x_t);
  display.drawGlyph(120, 8, 71); // 71 = ruler/cetvel ikonu

  // Mesafe değerini yaz (Daha dengeli font - kullanıcı isteği 8x8 tarzı)
  display.setFont(u8g2_font_7x13B_tr); // Açı ile aynı font, daha dengeli
  sprintf(buffer, "%ld", distance < 999 ? distance : 0);
  int w = display.getStrWidth(buffer);
  display.drawStr(115 - w, 8, buffer); // Icon ile aynı hizada

  // 2. Tespit (İKONLU!) - Hedef ikonu
  display.setFont(u8g2_font_open_iconic_thing_1x_t);
  display.drawGlyph(120, 18, 68); // 68 = target/hedef ikonu

  display.setFont(u8g2_font_5x7_tr);
  sprintf(buffer, "%d", totalDetections);
  w = display.getStrWidth(buffer);
  display.drawStr(118 - w, 18, buffer);

  // 3. Tarama (İKONLU!) - Radar/reload ikonu
  display.setFont(u8g2_font_open_iconic_arrow_1x_t);
  display.drawGlyph(120, 28, 79); // 79 = reload/circular arrow ikonu

  display.setFont(u8g2_font_5x7_tr);
  sprintf(buffer, "%lu", scanCount / 36);
  w = display.getStrWidth(buffer);
  display.drawStr(118 - w, 28, buffer);

  // BAŞLIK - Ortada "RADAR" (Siyah çerçeve + beyaz yazı - beyaz zemin için)
  // Siyah kutu çiz (beyaz zemin üstünde)
  display.setDrawColor(0);
  display.drawRBox(35, 0, 58, 14, 2); // Siyah yuvarlak köşe kutu

  // Beyaz yazı yaz (siyah kutunun içinde)
  display.setDrawColor(1);
  display.setFont(u8g2_font_ncenB10_tr); // Daha BÜYÜK font!
  int titleWidth = display.getStrWidth("RADAR");
  display.drawStr((128 - titleWidth) / 2, 11, "RADAR"); // Y pozisyonu ayarlandı

  // Rengi siyaha döndür (geri kalan çizimler için)
  display.setDrawColor(0);

  // TEHLİKE SEMBOLLERİ - Animasyonlu (Ortada, radarın içinde)
  if (closestDistance < 30 && blinkState) {
    display.setFont(u8g2_font_ncenB08_tr);
    char *symbol = getDangerSymbol(closestDistance);
    int symWidth = display.getStrWidth(symbol);
    display.drawStr((128 - symWidth) / 2, 45, symbol);
  }

  display.sendBuffer();
}

void updateTemp() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (!isnan(h) && !isnan(t)) {
    humidity = h;
    temperature = t;
    Serial.print("Sıcaklık: ");
    Serial.print(t);
    Serial.print("°C, Nem: ");
    Serial.print(h);
    Serial.println("%");
  }
}

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0)
    return 999;

  long distance = (duration / 2) / 29.1;

  // 0-200 cm arasında makul değerler
  if (distance < 0 || distance > 200)
    return 999;

  return distance;
}
