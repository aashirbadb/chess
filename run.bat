@echo off

echo Compiling file...
g++ .\src\*.cpp -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -o .\build\Chess.exe

if %ERRORLEVEL% == 0 (
echo Compiled successfully. Running game...
.\build\Chess.exe
echo Game closed
)
