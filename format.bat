@echo off
setlocal

REM Path to clang-format.
set CF=clang-format

echo Formatting Engine files...

for /r Engine %%f in (*.cpp *.h *.hpp) do (
    echo Formatting %%f
    %CF% -i -style=file "%%f"
)

echo.
echo Formatting Roguelike files...

for /r Roguelike %%f in (*.cpp *.h *.hpp) do (
    echo Formatting %%f
    %CF% -i -style=file "%%f"
)

echo.
echo Formatting completed!
pause