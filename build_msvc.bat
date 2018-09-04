@echo off

if not exist %~dp0build mkdir %~dp0build
pushd %~dp0build

set PROJECT=ptimer
set COMPILER_OPTIONS=-nologo -EHsc -Z7 -W4 -WX -permissive- -std:c++17
set COMPILER_DEFINES=-D%PROJECT%_INTERNAL
set LINKER_OPTIONS=-out:ptimer.exe
set LINKER_LIBRARIES=

rem Build the project executable
cl %COMPILER_OPTIONS% %COMPILER_DEFINES% %~dp0src/win32_%PROJECT%.cpp /link %LINKER_OPTIONS%

popd
