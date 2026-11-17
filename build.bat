@echo off
setlocal EnableDelayedExpansion
chcp 65001 >nul

REM ============================================================
REM  DuiLib_Ultimate CMake Build Script
REM  Usage:
REM    build.bat              Default Release + VS2010 + Win32
REM    build.bat Debug        Build type: Debug/Release
REM    build.bat Release 2019 VS version: 2010/2015/2017/2019/2022
REM    build.bat Release 2022 x64  Arch: Win32/x64
REM    build.bat clean        Clean build dir
REM ============================================================

set "BUILD_TYPE=%~1"
set "VS_VER=%~2"
set "ARCH=%~3"

if "%BUILD_TYPE%"=="" set "BUILD_TYPE=Release"
if "%VS_VER%"==""   set "VS_VER=2022"
if "%ARCH%"==""     set "ARCH=Win32"

if /I "%BUILD_TYPE%"=="clean" (
    echo [INFO] Cleaning build directory...
    if exist build rmdir /s /q build
    if exist bin rmdir /s /q bin
    if exist lib rmdir /s /q lib
    echo [OK]   Clean done
    goto :eof
)

where cmake >nul 2>nul
if errorlevel 1 (
    echo [ERROR] cmake not found, please install CMake and add to PATH
    exit /b 1
)

set "GENERATOR="
set "TOOLSET="
if "%VS_VER%"=="2010" (
    set "GENERATOR=Visual Studio 10 2010"
    set "TOOLSET=v100"
) else if "%VS_VER%"=="2015" (
    set "GENERATOR=Visual Studio 14 2015"
    set "TOOLSET=v140"
) else if "%VS_VER%"=="2017" (
    set "GENERATOR=Visual Studio 15 2017"
    set "TOOLSET=v141"
) else if "%VS_VER%"=="2019" (
    set "GENERATOR=Visual Studio 16 2019"
    set "TOOLSET=v142"
) else if "%VS_VER%"=="2022" (
    set "GENERATOR=Visual Studio 17 2022"
    set "TOOLSET=v143"
) else (
    echo [ERROR] Unsupported VS version: %VS_VER%
    echo        Supported: 2010 / 2015 / 2017 / 2019 / 2022
    exit /b 1
)

if /I "%ARCH%"=="x64" (
    if "%VS_VER%"=="2010" (
        echo [ERROR] VS2010 does not support x64 generator, use Win32 or upgrade VS
        exit /b 1
    )
    set "GENERATOR=!GENERATOR! %ARCH%"
)

set "ROOT=%~dp0"
pushd "%ROOT%"

echo.
echo ============================================================
echo  Project:     DuiLib_Ultimate
echo  BuildType:   %BUILD_TYPE%
echo  VS Version:  %VS_VER% ^(!TOOLSET!^)
echo  Arch:        %ARCH%
echo  Generator:   !GENERATOR!
echo ============================================================
echo.

if not exist build mkdir build
cd build

echo [1/2] CMake configure...
cmake -G "!GENERATOR!" -T "!TOOLSET!" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ..
if errorlevel 1 (
    echo [ERROR] CMake configure failed
    popd
    exit /b 1
)

echo.
echo [2/2] Building...
cmake --build . --config %BUILD_TYPE% --parallel 4
if errorlevel 1 (
    echo [ERROR] Build failed
    popd
    exit /b 1
)

echo.
echo ============================================================
echo  [OK] Build succeeded
echo  Output:
echo    DLL/EXE:  %ROOT%build\bin\%BUILD_TYPE%
echo    LIB:      %ROOT%build\lib\%BUILD_TYPE%
echo ============================================================

popd
endlocal
