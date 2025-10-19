@echo off
set "SCRIPT_DIR=%~dp0"
if "%SCRIPT_DIR:~-1%"=="\" set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"
set "EXE_PATH=%SCRIPT_DIR%\out\exeobj_cmake\trabalho1.exe"
"%EXE_PATH%" %*
