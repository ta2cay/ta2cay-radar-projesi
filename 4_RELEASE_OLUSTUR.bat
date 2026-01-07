@echo off
color 0B
echo ===================================================
echo   GITHUB RELEASE (SURUM) OLUSTURMA - OTOMATIK
echo ===================================================
echo.
echo v2.0 Surumu GitHub'da Olusturuluyor...
echo.

:: v2.0 etiketini ve sürümünü oluştur
gh release create v2.0 --title "v2.0 - Yeni Gorsel Arayuz" --notes "OLED ikon tasarimlari, acilis animasyonu ve font iyilesirmeleri eklendi. Gorsel olarak tamamen yenilendi."

echo.
echo ===================================================
echo   ISLEM TAMAMLANDI!
echo   GitHub sayfasinda 'Releases' kismina bakin.
echo   v2.0 surumunun orada oldugunu goreceksiniz.
echo ===================================================
pause
