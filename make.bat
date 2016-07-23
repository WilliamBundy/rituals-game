@echo off
SET msvcdir=C:\Program Files^ (x86)\Microsoft^ Visual^ Studio^ 14.0\VC\
if not defined DevEnvDir call "%msvcdir%vcvarsall.bat" amd64

SET MAINFILE=src\rituals_main.cpp
SET OUTPUT=bin\Rituals.exe
SET PDBOUT=bin\Rituals.pdb
SET LIBRARIES="c:\mslibs\lib"
SET INCLUDES=c:\mslibs\include\

SET DEBUG_DEF=DEBUG
SET RELEASE_DEF=RELEASE

SET DISABLED_WARNINGS=/wd4477 ^
	/wd4244 ^
	/wd4267 ^
	/wd4334 

SET LIBS=SDL2.lib ^
	SDL2main.lib ^
	SDL2_mixer.lib ^
	opengl32.lib ^
	Shlwapi.lib

REM start python autogit.py

ctime -begin rituals.ctm
if "%~1"=="" goto DEBUG_BUILD
if "%~1"=="release" goto RELEASE_BUILD
if "%~1"=="debug" goto DEBUG_BUILD
if "%~1"=="run" goto DEBUG_BUILD

:DEBUG_BUILD
echo Building a debug build with %DEBUG_DEF% defined
cl ^
	/I %INCLUDES% ^
	/Zi ^
	/MTd ^
	/W3 ^
	/Gd ^
	%DISABLED_WARNINGS% ^
	%MAINFILE% ^
	%LIBS% ^
	/EHsc ^
	/Fe%OUTPUT% ^
	/Fd%PDBOUT% ^
	/D%DEBUG_DEF% ^
	/link ^
	/ignore:4075 ^
	/LIBPATH:%LIBRARIES% ^
	/SUBSYSTEM:CONSOLE ^
	/NOLOGO ^
	/INCREMENTAL:NO
pushd bin
GOTO END

:RELEASE_BUILD
echo Building a release build with %RELEASE_DEF% defined
cl ^
	/I %INCLUDES% ^
	/MT ^
	%DISABLED_WARNINGS% ^
	/Gd ^
	/O2 ^
	%MAINFILE% ^
	%LIBS% ^
	/Fe%OUTPUT% ^
	/Fd%PDBOUT% ^
	/D%RELEASE_DEF% ^
	/EHsc ^
	/link ^
	/LIBPATH:%LIBRARIES% ^
	/SUBSYSTEM:CONSOLE ^
	/NOLOGO ^
	/INCREMENTAL:NO
pushd bin
ReM del *.pdb
GOTO END

:END
popd
del *.obj
ctime -end rituals.ctm
echo Build Complete
if "%~1"=="run" goto RUN_APP
if "%~2"=="run" goto RUN_APP
GOTO END2

:RUN_APP
start %OUTPUT%
GOTO END2

:END2

