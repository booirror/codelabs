@echo off
echo ----��ʼ������ѹ����Դ----
pushd webpTool

if exist ccbResources rmdir /s /q ccbResources
:: ������webpTool
xcopy /s/y ..\Resources\res\ccbResources\*.* ccbResources\

:: ת��webTool
for /f "usebackq tokens=*" %%d in (`dir /s /b ccbResources\*.png ccbResources\*.jpg`) do (

bin\cwebp.exe -quiet -q  80 "%%d" -o "%%d"

@echo on
set currtime=%time%

echo %%d

@echo off
)
popd

echo ----��ʼ���ܴ���----
pushd luacompile
call lua_encrypt.bat < nul
popd

echo ----������asset----
rmdir /s /q androidstudio\app\src\main\assets\src
xcopy /s/y luacompile\src\*.* androidstudio\app\src\main\assets\src\

rmdir /s /q androidstudio\app\src\main\assets\res
xcopy /s/y Resources\res\*.* androidstudio\app\src\main\assets\res\
xcopy /s/y webpTool\ccbResources\*.* androidstudio\app\src\main\assets\res\ccbResources\

echo ----���----
pause
