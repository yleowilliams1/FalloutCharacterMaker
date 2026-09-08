@echo off
setlocal enabledelayedexpansion

:: Compile all .c files found in a src directory into one binary (Windows/MinGW version)
:: Usage: cpil.bat [path_to_src] [output_binary]
::
:: Requires: MinGW-w64 gcc on PATH, and raylib installed where gcc can find
:: its headers/libs (either in your MinGW include/lib dirs, or pass extra
:: -I/-L paths below if you installed raylib somewhere custom).

set "SRC_DIR=%~1"
if "%SRC_DIR%"=="" set "SRC_DIR=src"

set "OUT_BIN=%~2"
if "%OUT_BIN%"=="" set "OUT_BIN=app.exe"

set "CC=gcc"

if not exist "%SRC_DIR%\" (
    echo Error: source directory '%SRC_DIR%' not found. >&2
    exit /b 1
)

:: Collect all .c files recursively
set "FILES="
set /a COUNT=0
for /r "%SRC_DIR%" %%F in (*.c) do (
    set "FILES=!FILES! "%%F""
    set /a COUNT+=1
)

if !COUNT! equ 0 (
    echo No .c files found in '%SRC_DIR%'. >&2
    exit /b 1
)

echo Found !COUNT! source file(s):
for /r "%SRC_DIR%" %%F in (*.c) do echo   %%F

echo Compiling into: %OUT_BIN%

:: raylib on Windows needs opengl32/gdi32/winmm in addition to raylib/m.
:: If you installed raylib to a custom folder, add e.g.:
::   -I"C:\raylib\include" -L"C:\raylib\lib"
:: before -lraylib below.
%CC% !FILES! -o "%OUT_BIN%" -lraylib -lopengl32 -lgdi32 -lwinmm -lm

if errorlevel 1 (
    echo ---------------------------------------- >&2
    echo Build failed. >&2
    exit /b 1
)

echo ----------------------------------------
echo Build succeeded: %OUT_BIN%
