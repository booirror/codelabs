@echo off




for /f "usebackq tokens=*" %%d in (`dir /s /b ccbResources\*.png ccbResources\*.jpg`) do (

bin\cwebp.exe -quiet -q  80 "%%d" -o "%%d"

@echo on
set currtime=%time%

echo %%d

@echo off
)

pause
