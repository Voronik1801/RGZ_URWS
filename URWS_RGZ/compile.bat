echo off
cls

set vcvarsscript="%programfiles(x86)%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"

call %vcvarsscript%

if exist *.exe del *.exe
if exist *.dll del *.dll
cl "./Main.cpp" kernel32.lib user32.lib gdi32.lib advapi32.lib 
cl /LD "./MonitorInfo.cpp" kernel32.lib user32.lib gdi32.lib advapi32.lib
cl /LD "./RDTSC.cpp" kernel32.lib user32.lib gdi32.lib advapi32.lib

pause
