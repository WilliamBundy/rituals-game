CC=clang
NAME=Rituals.bin
MAIN=src/rituals_main.cpp
CFLAGS=-xc++ -std=c++11 -DRITUALS_LINUX=1 -Wall `sdl2-config --cflags`
DISABLED_WARNINGS=-Wno-writable-strings \
				  -Wno-missing-braces \
				  -Wno-null-arithmetic \
				  -Wno-unused-variable \
				  -Wno-tautological-constant-out-of-range-compare \
				  -Wno-dangling-else \
				  -Wno-format \
				  -Wno-parentheses-equality \
				  -Wno-unused-function
LFLAGS=-lm `sdl2-config --libs` -lGL -o bin/$(NAME)
# on osx, add -framework OpenGL 

all: build

run: build
	bin/$(NAME)
	
build: 
	$(CC) $(CFLAGS) $(DISABLED_WARNINGS) $(MAIN) $(LFLAGS)
