@echo off
setlocal
cd /d "%~dp0"

set SOLUTION_PATH=Game.sln
set MSBUILD_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe
set CONFIGURATION=Release
set PLATFORM=x64

REM Avoid duplicate Path/PATH entries from shells; MSBuild can fail on them.
set Path=

set OUTPUT_DIR=%CD%\bin\Roguelike\%CONFIGURATION%
set PACKAGE_ROOT=%CD%\dist
set PACKAGE_DIR=%PACKAGE_ROOT%\Roguelike-%CONFIGURATION%-%PLATFORM%
set PACKAGE_ZIP=%PACKAGE_ROOT%\Roguelike-%CONFIGURATION%-%PLATFORM%.zip

echo Building %CONFIGURATION% %PLATFORM% package...

if not exist "%MSBUILD_PATH%" (
    echo MSBuild not found: "%MSBUILD_PATH%"
    pause
    exit /b 1
)

"%MSBUILD_PATH%" "%SOLUTION_PATH%" /t:Roguelike /p:Configuration=%CONFIGURATION% /p:Platform=%PLATFORM% /nologo

if %ERRORLEVEL% neq 0 (
    echo Build failed.
    pause
    exit /b %ERRORLEVEL%
)

if exist "%PACKAGE_DIR%" rmdir /s /q "%PACKAGE_DIR%"
if not exist "%PACKAGE_ROOT%" mkdir "%PACKAGE_ROOT%"
mkdir "%PACKAGE_DIR%"

copy "%OUTPUT_DIR%\Roguelike.exe" "%PACKAGE_DIR%\" /Y
copy "%CD%\Roguelike\openal32.dll" "%PACKAGE_DIR%\" /Y

REM Include the release Visual C++ runtime DLLs, so players do not need Visual Studio.
"%SystemRoot%\System32\WindowsPowerShell\v1.0\powershell.exe" -NoProfile -ExecutionPolicy Bypass -Command "$root='C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Redist\MSVC'; $crt=Get-ChildItem $root -Directory -ErrorAction SilentlyContinue | Sort-Object Name -Descending | ForEach-Object { Join-Path $_.FullName 'x64\Microsoft.VC143.CRT' } | Where-Object { Test-Path $_ } | Select-Object -First 1; if ($crt) { Copy-Item (Join-Path $crt 'msvcp140.dll'),(Join-Path $crt 'vcruntime140.dll'),(Join-Path $crt 'vcruntime140_1.dll') '%PACKAGE_DIR%' -Force } else { Write-Warning 'VC++ runtime redist not found. Install Microsoft Visual C++ Redistributable on target PC.' }"

"%SystemRoot%\System32\xcopy.exe" "%CD%\Roguelike\Resources\*" "%PACKAGE_DIR%\Resources\" /E /I /Y

if exist "%PACKAGE_ZIP%" del /q "%PACKAGE_ZIP%"
"%SystemRoot%\System32\WindowsPowerShell\v1.0\powershell.exe" -NoProfile -ExecutionPolicy Bypass -Command "Compress-Archive -Path '%PACKAGE_DIR%\*' -DestinationPath '%PACKAGE_ZIP%' -Force"

echo.
echo Package completed:
echo %PACKAGE_ZIP%
echo.
echo Send this zip, not the Debug build.

pause
