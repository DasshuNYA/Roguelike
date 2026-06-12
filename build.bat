@echo off
cd /d "%~dp0"

set SOLUTION_PATH=Game.sln
set MSBUILD_PATH="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"

set CONFIGURATION=Release
set PLATFORM=x64

REM Avoid duplicate Path/PATH entries from shells; MSBuild can fail on them.
set Path=

set OUTPUT_DIR=%CD%\bin\Roguelike\%CONFIGURATION%
set GAME_RESOURCES=%CD%\Roguelike\Resources

echo Building Roguelike...

if not exist %MSBUILD_PATH% (
    echo MSBuild not found.
    pause
    exit /b 1
)

%MSBUILD_PATH% %SOLUTION_PATH% /t:Roguelike /p:Configuration=%CONFIGURATION% /p:Platform=%PLATFORM%

if %ERRORLEVEL% neq 0 (
    echo Build failed.
    pause
    exit /b %ERRORLEVEL%
)

mkdir "%OUTPUT_DIR%" 2>nul

echo Copying openal32.dll...
copy "%CD%\Roguelike\openal32.dll" "%OUTPUT_DIR%\" /Y

echo Copying Visual C++ runtime DLLs...
"%SystemRoot%\System32\WindowsPowerShell\v1.0\powershell.exe" -NoProfile -ExecutionPolicy Bypass -Command "$root='C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Redist\MSVC'; $crt=Get-ChildItem $root -Directory -ErrorAction SilentlyContinue | Sort-Object Name -Descending | ForEach-Object { Join-Path $_.FullName 'x64\Microsoft.VC143.CRT' } | Where-Object { Test-Path $_ } | Select-Object -First 1; if ($crt) { Copy-Item (Join-Path $crt 'msvcp140.dll'),(Join-Path $crt 'vcruntime140.dll'),(Join-Path $crt 'vcruntime140_1.dll') '%OUTPUT_DIR%' -Force } else { Write-Warning 'VC++ runtime redist not found. Install Microsoft Visual C++ Redistributable on target PC.' }"

echo Copying Resources...
"%SystemRoot%\System32\xcopy.exe" "%GAME_RESOURCES%\*" "%OUTPUT_DIR%\Resources\" /E /I /Y

echo.
echo Build completed.
echo Output: %OUTPUT_DIR%

pause
