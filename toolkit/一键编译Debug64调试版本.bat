@echo off
set project_name=
set project_path=.%project_name%
set vs2013path="C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE"

set Path=%Path%;%vs2013path%

@echo Debug64版本
devenv "%project_path%/Kernel/Kernel.sln" /rebuild "Debug|x64" && echo ok || msg %username% "Kernel 编译失败"
devenv "%project_path%/DBServer/前置机.sln" /rebuild "Debug|x64" && echo ok || msg %username% "前置机 编译失败"
devenv "%project_path%/WebServer/WEB服务器.sln" /rebuild "Debug|x64" && echo ok || msg %username% "WEB服务器 编译失败"
devenv "%project_path%/GatewayServer/GatewayServer.sln" /rebuild "Debug|x64" && echo ok || msg %username% "代理前端 编译失败"
devenv "%project_path%/CenterServer/CenterServer.sln" /rebuild "Debug|x64" && echo ok || msg %username% "中心服务器 编译失败"
devenv "%project_path%/LoginServer/LoginServer.sln" /rebuild "Debug|x64" && echo ok || msg %username% "登录服务器 编译失败"
devenv "%project_path%/PlazaServer/PlazaServer.sln" /rebuild "Debug|x64" && echo ok || msg %username% "广场服务器 编译失败"
devenv "%project_path%/GameServer/GameServer.sln" /rebuild "Debug|x64" && echo ok || msg %username% "游戏服务器 编译失败"
devenv "%project_path%/GuardServer/守护进程.sln" /rebuild "Debug|x64" && echo ok || msg %username% "守护进程 编译失败"
devenv "%project_path%/LogDBServerCopy/日志前置机.sln" /rebuild "Debug|x64" && echo ok || msg %username% "日志前置机 编译失败"
devenv "%project_path%/LogServer/日志进程.sln" /rebuild "Debug|x64" && echo ok || msg %username% "日志进程 编译失败"

devenv "%project_path%/GameModule/mahjong/mahjong.sln" /rebuild "Debug|x64" && echo ok || msg %username% "游戏模块 mahjong 编译失败"

@echo 编译完成
