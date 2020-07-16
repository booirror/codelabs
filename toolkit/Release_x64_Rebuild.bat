TITLE=火拼麻将 Release x64 Rebuild  -  CenterServer 1/12
"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" /t:rebuild /p:Configuration=Release /p:Platform=x64 "./CenterServer/CenterServer.sln"
IF %ERRORLEVEL% NEQ 0 pause
TITLE=火拼麻将 Release x64 Rebuild  -  DBServer 2/12
"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" /t:rebuild /p:Configuration=Release /p:Platform=x64 "./DBServer/前置机.sln"
IF %ERRORLEVEL% NEQ 0 pause
TITLE=火拼麻将 Release x64 Rebuild  -  mahjong 3/12
"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" /t:rebuild /p:Configuration=Release /p:Platform=x64 "./GameModule/mahjong/mahjong.sln"
IF %ERRORLEVEL% NEQ 0 pause
::TITLE=火拼麻将 Release x64 Rebuild  -  mahjong 3/12
::"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" /t:rebuild /p:Configuration=Release /p:Platform=x64 "./GameModule_ErRen/mahjong/mahjong.sln"
::IF %ERRORLEVEL% NEQ 0 pause
TITLE=火拼麻将 Release x64 Rebuild  -  GameServer 4/12
"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" /t:rebuild /p:Configuration=Release /p:Platform=x64 "./GameServer/GameServer.sln"
IF %ERRORLEVEL% NEQ 0 pause
TITLE=火拼麻将 Release x64 Rebuild  -  GatewayServer 5/12
"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" /t:rebuild /p:Configuration=Release /p:Platform=x64 "./GatewayServer/GatewayServer.sln"
IF %ERRORLEVEL% NEQ 0 pause
TITLE=火拼麻将 Release x64 Rebuild  -  GuardServer 6/12
"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" /t:rebuild /p:Configuration=Release /p:Platform=x64 "./GuardServer/守护进程.sln"
IF %ERRORLEVEL% NEQ 0 pause
TITLE=火拼麻将 Release x64 Rebuild  -  Kernel 7/12
"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" /t:rebuild /p:Configuration=Release /p:Platform=x64 "./Kernel/核心引擎.sln"
IF %ERRORLEVEL% NEQ 0 pause
TITLE=火拼麻将 Release x64 Rebuild  -  LogDBServerCopy 8/12
"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" /t:rebuild /p:Configuration=Release /p:Platform=x64 "./LogDBServerCopy/日志前置机.sln"
IF %ERRORLEVEL% NEQ 0 pause
TITLE=火拼麻将 Release x64 Rebuild  -  LoginServer 9/12
"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" /t:rebuild /p:Configuration=Release /p:Platform=x64 "./LoginServer/LoginServer.sln"
IF %ERRORLEVEL% NEQ 0 pause
TITLE=火拼麻将 Release x64 Rebuild  -  LogServer 10/12
"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" /t:rebuild /p:Configuration=Release /p:Platform=x64 "./LogServer/日志进程.sln"
IF %ERRORLEVEL% NEQ 0 pause
TITLE=火拼麻将 Release x64 Rebuild  -  PlazaServer 11/12
"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" /t:rebuild /p:Configuration=Release /p:Platform=x64 "./PlazaServer/PlazaServer.sln"
IF %ERRORLEVEL% NEQ 0 pause
TITLE=火拼麻将 Release x64 Rebuild  -  WebServer 12/12
"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" /t:rebuild /p:Configuration=Release /p:Platform=x64 "./WebServer/WEB服务器.sln"
IF %ERRORLEVEL% NEQ 0 pause
TITLE=火拼麻将 Release x64 Rebuild  -  Finished  All 12

pause
