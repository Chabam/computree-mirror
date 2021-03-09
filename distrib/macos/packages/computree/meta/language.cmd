@echo off

cd "%~dp0"

:: To recreate files from scratch (be careful, you lose you previous translations !)
:: lupdate installscript.qs . -ts en.ts
:: lupdate installscript.qs . -ts fr.ts

lrelease en.ts
lrelease fr.ts

exit
