@echo off
cd /d "%~dp0"

set SOLUTION_PATH=Game.sln
set MSBUILD_PATH="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"

set CONFIGURATION=Debug
set PLATFORM=x64

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

echo Copying Resources...
xcopy "%GAME_RESOURCES%\*" "%OUTPUT_DIR%\Resources\" /E /I /Y

echo.
echo Build completed.
echo Output: %OUTPUT_DIR%

pause