@ECHO ON

set BASEDIR=%~dp0
PUSHD %BASEDIR%

RMDIR /Q /S build
mkdir build

REM for connan
conan install . -if=./build --build=missing -s build_type=Debug --profile=debug

cd build
cmake ..
cmake --build . --config Debug