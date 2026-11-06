@echo off
cd /d "%~dp0"
g++ -std=c++11 -O2 main.cpp students.cpp attendance.cpp history.cpp search.cpp grouping.cpp -o attendance.exe
if errorlevel 1 (
    echo Build failed. Make sure g++ is installed and available in PATH.
    pause
    exit /b 1
)
attendance.exe
pause
