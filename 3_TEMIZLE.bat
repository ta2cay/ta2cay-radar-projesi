@echo off
color 0C
echo ===================================================
echo   GITHUB TEMIZLIK ISLEMI
echo ===================================================
echo.
echo Gereksiz dosyalar (bat, txt) GitHub'dan siliniyor...
echo.

:: Dosyaları git'ten sil (bu komut yerel dosyayı da siler)
git rm -f 1_GIRIS_YAP.bat
git rm -f 2_YUKLE.bat
git rm -f OKU_BENI_KURULUM.txt

:: Değişiklikleri onayla ve gönder
git commit -m "Kurulum dosyalari temizlendi"
git push

echo.
echo ===================================================
echo   TEMIZLIK TAMAMLANDI!
echo   Gereksiz dosyalar GitHub'dan kaldirildi.
echo ===================================================
pause
