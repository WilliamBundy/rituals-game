@echo off
SET msvcdir=C:\Program Files^ (x86)\Microsoft^ Visual^ Studio^ 14.0\VC\
if not defined DevEnvDir call "%msvcdir%vcvarsall.bat" amd64

set file_input=src\thirdparty\gl_core_3_3.c
set name=gl_core_3_3

cl -nologo -O2 -MT -TP -c %file_input% ^
	&& lib -nologo %name%.obj -out:%name%.lib ^
	&& del *.obj

