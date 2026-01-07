@echo off
color 0E
echo ===================================================
echo   YUKLEME ISLEMI - TA2CAY RADAR PROJESI
echo ===================================================
echo.
echo Kodlar GitHub'a Yukleniyor... Lutfen bekleyin...
echo.

git init
git config user.email "ta2cay@example.com"
git config user.name "TA2CAY"
git add .
git commit -m "v2.0 Guncellemesi - Yeni Arayuz"
git branch -M main
git remote remove origin
git remote add origin https://github.com/ta2cay/ta2cay-radar-projesi.git
git push -u origin main --force

echo.
echo ===================================================
echo   ISLEM BASARIYLA TAMAMLANDI!
echo   Kodlariniz artik GitHub'da:
echo   https://github.com/ta2cay/ta2cay-radar-projesi
echo ===================================================
pause
