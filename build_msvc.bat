@echo off

set PROJECT=ptimer
set COMPILER_OPTIONS=-O2 -Oi -GS- -nologo -W4 -WX -permissive- -std:c++17
set COMPILER_DEFINES=-D%PROJECT%_INTERNAL -DWIN32_LEAN_AND_MEAN
set LINKER_OPTIONS=-out:ptimer.exe -subsystem:console -entry:main -nodefaultlib
set LINKER_LIBRARIES=Kernel32.lib User32.lib

if not exist "%~dp0build" mkdir "%~dp0build"
pushd "%~dp0build"

ptime cl %COMPILER_OPTIONS% %COMPILER_DEFINES% ^"%~dp0src\win32_%PROJECT%.cpp^" /link %LINKER_OPTIONS% %LINKER_LIBRARIES%

popd
