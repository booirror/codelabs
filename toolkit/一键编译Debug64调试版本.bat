@echo off
set project_name=
set project_path=.%project_name%
set vs2013path="C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE"

set Path=%Path%;%vs2013path%

@echo Debug64�汾
devenv "%project_path%/Kernel/Kernel.sln" /rebuild "Debug|x64" && echo ok || msg %username% "Kernel ����ʧ��"
devenv "%project_path%/DBServer/ǰ�û�.sln" /rebuild "Debug|x64" && echo ok || msg %username% "ǰ�û� ����ʧ��"
devenv "%project_path%/WebServer/WEB������.sln" /rebuild "Debug|x64" && echo ok || msg %username% "WEB������ ����ʧ��"
devenv "%project_path%/GatewayServer/GatewayServer.sln" /rebuild "Debug|x64" && echo ok || msg %username% "����ǰ�� ����ʧ��"
devenv "%project_path%/CenterServer/CenterServer.sln" /rebuild "Debug|x64" && echo ok || msg %username% "���ķ����� ����ʧ��"
devenv "%project_path%/LoginServer/LoginServer.sln" /rebuild "Debug|x64" && echo ok || msg %username% "��¼������ ����ʧ��"
devenv "%project_path%/PlazaServer/PlazaServer.sln" /rebuild "Debug|x64" && echo ok || msg %username% "�㳡������ ����ʧ��"
devenv "%project_path%/GameServer/GameServer.sln" /rebuild "Debug|x64" && echo ok || msg %username% "��Ϸ������ ����ʧ��"
devenv "%project_path%/GuardServer/�ػ�����.sln" /rebuild "Debug|x64" && echo ok || msg %username% "�ػ����� ����ʧ��"
devenv "%project_path%/LogDBServerCopy/��־ǰ�û�.sln" /rebuild "Debug|x64" && echo ok || msg %username% "��־ǰ�û� ����ʧ��"
devenv "%project_path%/LogServer/��־����.sln" /rebuild "Debug|x64" && echo ok || msg %username% "��־���� ����ʧ��"

devenv "%project_path%/GameModule/mahjong/mahjong.sln" /rebuild "Debug|x64" && echo ok || msg %username% "��Ϸģ�� mahjong ����ʧ��"

@echo �������
