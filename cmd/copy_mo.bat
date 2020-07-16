@echo off
if not exist ..\Resources\res\i18n mkdir ..\Resources\res\i18n
for %%i in (*.mo) do (
	copy %%i ..\Resources\res\i18n\ /y
)