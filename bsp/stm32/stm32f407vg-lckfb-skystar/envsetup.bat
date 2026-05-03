@echo off
setlocal enabledelayedexpansion

if "%CD%"=="C:\Windows\system32" (
    cd /d %~dp0
)

set SDK_PRJ_TOP_DIR=%cd%
set ENV_ROOT=D:\BaiduNetdiskDownload\env-windows
set PythonPath=%ENV_ROOT%\tools\python-3.11.9-amd64
set GCCPath=%ENV_ROOT%\tools\gnu_gcc\arm_gcc\mingw\bin
set KeilPath=C:\Keil_v5\UV4

for %%i in ("%SDK_PRJ_TOP_DIR%\..\..\..") do set "RTT_ROOT=%%~fi"

powershell.exe -NoExit -ExecutionPolicy Bypass -Command "& { $env:SDK_PRJ_TOP_DIR='%SDK_PRJ_TOP_DIR%'; $env:ENV_ROOT='%ENV_ROOT%'; $env:PythonPath='%PythonPath%'; $env:GCCPath='%GCCPath%'; $env:KeilPath='%KeilPath%'; $env:PATH='%PythonPath%;%PythonPath%\Scripts;%GCCPath%;%KeilPath%;%ENV_ROOT%\tools\bin;%ENV_ROOT%\tools\scripts;' + $env:PATH; $env:PKGS_ROOT='%ENV_ROOT%\packages'; $env:RTT_ROOT='%RTT_ROOT%'; $env:RTT_EXEC_PATH='%GCCPath%'; . '%~dp0envsetup.ps1' }"
