@echo off
color 0A
echo ===================================================
echo   GITHUB GIRIS ISLEMI - TA2CAY RADAR PROJESI
echo ===================================================
echo.
echo 1. Birazdan tarayiciniz acilacak ve bir kod goreceksiniz.
echo 2. Lutfen o kodu tarayiciya girin ve 'Authorize' deyin.
echo 3. 'Congratulations' yazisini gorunce buraya donun.
echo.
echo Devam etmek icin bir tusa basin...
pause >nul

echo.
echo Giris islemi baslatiliyor...
gh auth login --web -h github.com -p https

echo.
echo ISLEM TAMAMLANDI! Simdi 2_YUKLE.bat dosyasini calistirin.
pause
