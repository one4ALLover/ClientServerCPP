@echo off
setlocal

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

if not exist "%VSWHERE%" (
    echo Visual Studio Installer was not found.
    exit /b 1
)

set "VSROOT="

for /f "usebackq delims=" %%I in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSROOT=%%I"

if not defined VSROOT (
    echo MSVC installation was not found.
    exit /b 1
)

call "%VSROOT%\Common7\Tools\VsDevCmd.bat" -arch=x64

if errorlevel 1 exit /b 1


REM Build server
cl /EHsc /Zi /std:c++17 server.cpp /Fe:server.exe /link Ws2_32.lib

if errorlevel 1 (
    echo Server build failed.
    exit /b 1
)

REM Build client
cl /EHsc /Zi /std:c++17 client.cpp /Fe:client.exe /link Ws2_32.lib

if errorlevel 1 (
    echo Client build failed.
    exit /b 1
)

echo Server and client built successfully.
exit /b 0

