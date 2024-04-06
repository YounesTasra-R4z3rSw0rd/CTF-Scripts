@ECHO OFF

cl.exe /nologo /Ox /MT /W0 /GS- /DNDEBUG /Tp pov.cpp /link /OUT:pov.exe /MACHINE:x64

del *.obj
