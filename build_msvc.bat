@echo off

set PROJECT=ptimer
set COMPILER_OPTIONS=-O2 -EHsc -nologo -W4 -WX -MD -permissive- -std:c++17
set COMPILER_DEFINES=-D%PROJECT%_INTERNAL
set LINKER_OPTIONS=-out:ptimer.exe -subsystem:console
set LINKER_LIBRARIES=

if not exist %~dp0build mkdir %~dp0build
pushd %~dp0build

ptime cl %COMPILER_OPTIONS% %COMPILER_DEFINES% %~dp0src\win32_%PROJECT%.cpp /link %LINKER_OPTIONS% %LINKER_LIBRARIES%

popd
