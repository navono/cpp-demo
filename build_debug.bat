@ECHO ON

set BASEDIR=%~dp0
PUSHD %BASEDIR%

RMDIR /Q /S build
mkdir build

cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Debug
