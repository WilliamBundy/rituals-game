@echo off
SET msvcdir=C:\Program Files^ (x86)\Microsoft^ Visual^ Studio^ 14.0\VC\
if not defined DevEnvDir call "%msvcdir%vcvarsall.bat" amd64


taskkill /IM Rituals.exe >nul 2>&1
nmake /nologo -f windows.mak

if "%~1"=="run" goto run
if "%~2"=="run" goto run
GOTO end

:run
start bin\Rituals.exe

:end

