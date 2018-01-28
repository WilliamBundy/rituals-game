
ctime=usr\bin\ctime.exe
wirmpht=usr\bin\wirmpht.exe
disabled=/wd4477\
		 /wd4244\
		 /wd4334\
		 /wd4305\
		 /wd4101\
		 /D_CRT_SECURE_NO_WARNINGS
ccflags=/TP /nologo /Zi /MTd /W3 /Gd /EHsc /fp:fast $(disabled)

.SILENT:
all: start debug_game end


debug_game:
#	$(wirmpht) -p -s -t src/rituals_main.cpp > src/rituals_types.cpp
	cl $(ccflags) /I"msvc_libs/include/" src/rituals_main.cpp \
		/Fe"bin/Rituals.exe" /Fd"bin/Rituals.pdb" \
		/DRITUALS_WINDOWS \
		/link /NOLOGO /LIBPATH:"msvc_libs/lib" /INCREMENTAL:NO /SUBSYSTEM:CONSOLE\
		user32.lib gdi32.lib Shlwapi.lib SDL2.lib SDL2main.lib opengl32.lib
	
start: 
	$(ctime) -begin usr/bin/rituals.ctm

end: 
	$(ctime) -end usr/bin/rituals.ctm
