@echo off 

set BUILD_DIR="Build"
set GENERATOR="Visual Studio 16 2019"

mkdir %BUILD_DIR%
cd %BUILD_DIR%

cmake -G %GENERATOR% -A Win32 ..