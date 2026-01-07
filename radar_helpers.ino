//. Yardımcı fonksiyonlar - Engel geçmişi ve istatistikler

void addObstacle(int angle, long distance) {
  // Eski engelleri kontrol et ve pasifleştir (10 saniyeden eski)
  unsigned long currentTime = millis();
  for (int i = 0; i < MAX_OBSTACLES; i++) {
    if (obstacleHistory[i].active &&
        (currentTime - obstacleHistory[i].timestamp > 10000)) {
      obstacleHistory[i].active = false;
    }
  }

  // Yeni engel ekle
  for (int i = 0; i < MAX_OBSTACLES; i++) {
    if (!obstacleHistory[i].active) {
      obstacleHistory[i].angle = angle;
      obstacleHistory[i].distance = distance;
      obstacleHistory[i].timestamp = currentTime;
      obstacleHistory[i].active = true;
      break;
    }
  }

  // İstatistikleri güncelle
  totalDetections++;
  totalDistance += distance;
  if (distance < closestDistance) {
    closestDistance = distance;
  }
}

void updateFadeTrail(int currentAngle) {
  fadeAngles[fadeIndex] = currentAngle;
  fadeIndex = (fadeIndex + 1) % FADE_LENGTH;
}

void drawFadeEffect() {
  // Son birkaç açıda soluk çizgiler çiz
  for (int i = 0; i < FADE_LENGTH; i++) {
    int idx = (fadeIndex - i - 1 + FADE_LENGTH) % FADE_LENGTH;
    int ang = fadeAngles[idx];

    if (ang >= 0 && ang <= 180) {
      // Sadece belirli aralıklarla çiz (daha az piksel)
      if (i % 2 == 0) {
        float rad = radians(ang);
        int lineX = CENTER_X + (MAX_RADIUS - i * 3) * cos(rad);
        int lineY = CENTER_Y - (MAX_RADIUS - i * 3) * sin(rad);
        display.drawLine(CENTER_X, CENTER_Y, lineX, lineY);
      }
    }
  }
}

void drawObstacleHistory() {
  // Geçmişteki engelleri çiz
  for (int i = 0; i < MAX_OBSTACLES; i++) {
    if (obstacleHistory[i].active) {
      float rad = radians(obstacleHistory[i].angle);
      int obstacleR = (obstacleHistory[i].distance * MAX_RADIUS) / 50;
      int obstacleX = CENTER_X + obstacleR * cos(rad);
      int obstacleY = CENTER_Y - obstacleR * sin(rad);

      // Küçük nokta (geçmiş engel)
      display.drawPixel(obstacleX, obstacleY);
      display.drawPixel(obstacleX + 1, obstacleY);
      display.drawPixel(obstacleX, obstacleY + 1);
      display.drawPixel(obstacleX - 1, obstacleY);
    }
  }
}

char *getDangerSymbol(long distance) {
  if (distance < 15)
    return "!!!";
  if (distance < 30)
    return "!!";
  if (distance < 50)
    return "!";
  return "";
}
