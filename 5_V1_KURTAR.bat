@echo off
color 0D
echo ===================================================
echo   ESKI SURUMU (v1.0) KURTARMA VE YAYINLAMA
echo ===================================================
echo.
echo Hatami telafi ediyorum...
echo 1. Mevcut v2.0 kodlari yedekleniyor...
echo 2. Eski v1.0 kodlari geri getiriliyor...
echo 3. GitHub'a v1.0 olarak etiketleniyor...
echo.

:: 1. Mevcut (v2.0) dosyaları geçici isimlendir
move radar.ino radar_v2.ino
move radar_helpers.ino radar_helpers_v2.ino

:: 2. Eski (v1.0) dosyaları asıl isimlerine koy
copy radar_old.ino radar.ino
copy radar_helpers_old.ino radar_helpers.ino

:: 3. Git işlemleri - v1.0 Release oluştur
git add .
git commit -m "Recovered v1.0 Original Code"
git tag v1.0
git push origin v1.0
gh release create v1.0 --title "v1.0 - Orijinal Surum" --notes "Bu surum, gorsel iyilestirmeler yapilmadan onceki orijinal, calisan kodlardir."

echo.
echo v1.0 Basariyla Yayilandiktan Sonra...
echo v2.0 Kodlari Geri Yukleniyor...
echo.

:: 4. Dosyaları geri eski haline (v2.0) getir
del radar.ino
del radar_helpers.ino
move radar_v2.ino radar.ino
move radar_helpers_v2.ino radar_helpers.ino

:: 5. Git işlemleri - v2.0'a geri dön
git add .
git commit -m "Restored v2.0 (Latest)"
git push

echo.
echo ===================================================
echo   KURTARMA ISLEMI TAMAMLANDI!
echo   GitHub sayfasinda artik hem v1.0 hem v2.0 var.
echo ===================================================
pause
