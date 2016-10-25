@echo off
SET msvcdir=C:\Program Files^ (x86)\Microsoft^ Visual^ Studio^ 14.0\VC\
if not defined DevEnvDir call "%msvcdir%vcvarsall.bat" amd64

SET MAINFILEBASE=rituals_main
SET MAINFILE=src\%MAINFILEBASE%.cpp
SET PPC_OUT=%MAINFILEBASE%.i
SET BASENAME=Rituals.exe
SET OUTPUT=bin\Rituals.exe
SET PDBOUT=bin\Rituals.pdb
SET LIBRARIES="msvc_libs\lib"
SET INCLUDES=msvc_libs\include\

SET DEBUG_DEF=DEBUG
SET RELEASE_DEF=RELEASE

SET DISABLED_WARNINGS=/wd4477 ^
	/wd4244 ^
	/wd4267 ^
	/wd4334 ^
	/wd4305

SET LIBS=SDL2.lib ^
	SDL2main.lib ^
	SDL2_mixer.lib ^
	opengl32.lib ^
	Shlwapi.lib

start python autogit.py


cl ^
	/P ^
	/I %INCLUDES% ^
	%MAINFILE% ^
	/DPREPROCESSOR

metaprogram -m %PPC_OUT% > src\rituals_reflection.cpp
metaprogram -t -p %PPC_OUT% > src\rituals_types.cpp

taskkill /IM %BASENAME% 
ctime -begin rituals.ctm
if "%~1"=="" goto DEBUG_BUILD
if "%~1"=="meta" goto META_BUILD
if "%~1"=="release" goto RELEASE_BUILD
if "%~1"=="debug" goto DEBUG_BUILD
if "%~1"=="run" goto DEBUG_BUILD

:META_BUILD
set META_MAIN=src\metaprogram\metaprogram_main.cpp
set META_OUT=metaprogram.exe
set META_PDB=metaprogram.pdb

cl ^
	/Zi ^
	/MTd ^
	/W3 ^
	/Gd ^
	%DISABLED_WARNINGS% ^
	%META_MAIN% ^
	/EHsc ^
	/Fe%META_OUT% ^
	/Fd%META_PDB% ^
	/D%DEBUG_DEF% ^
	/DRITUALS_WINDOWS#1 ^
	/link ^
	/SUBSYSTEM:CONSOLE ^
	/NOLOGO ^
	/INCREMENTAL:NO
goto END


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
	/DRITUALS_WINDOWS#1 ^
	/link ^
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
	/DRITUALS_WINDOWS#1 ^
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

