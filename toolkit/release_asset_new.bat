@echo off
echo ----开始拷贝并压缩资源----
pushd webpTool

if exist ccbResources rmdir /s /q ccbResources
:: 拷贝到webpTool
xcopy /s/y ..\Resources\res\ccbResources\*.* ccbResources\

:: 转换webTool
for /f "usebackq tokens=*" %%d in (`dir /s /b ccbResources\*.png ccbResources\*.jpg`) do (

bin\cwebp.exe -quiet -q  80 "%%d" -o "%%d"

@echo on
set currtime=%time%

echo %%d

@echo off
)
popd

echo ----开始加密代码----
pushd luacompile
call lua_encrypt.bat < nul
popd

echo ----拷贝到asset----
rmdir /s /q androidstudio\app\src\main\assets\src
xcopy /s/y luacompile\src\*.* androidstudio\app\src\main\assets\src\

rmdir /s /q androidstudio\app\src\main\assets\res
xcopy /s/y Resources\res\*.* androidstudio\app\src\main\assets\res\
xcopy /s/y webpTool\ccbResources\*.* androidstudio\app\src\main\assets\res\ccbResources\

echo ----完成----
pause
