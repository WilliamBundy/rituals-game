# [Rituals](rituals.handmade.network)
A Zelda-like survival/adventure game, 

## Notes:

It's hard to admit this project is largely discontinued, but I've not been motivated
to work on it for a long time. I don't think this source particularly represents 
my current coding style/ability, and, though I've recently waded through it again,
I don't think much of it is salvagable for new projects. 

This commit represents the last playable build before I started playing around with cleaning up in the dev branch.

## License
Rituals' source is licensed under the MIT license. 

## How to build (windows)
 1. Install Visual Studio (I'm using 2017)
 2. Change the msvcdir variable in make.bat, if necessary
 3. Download SDL2's development libraries
 4. Set LIBRARIES and INCLUDES in make.bat to point to those
 5. You probably want to delete the line "start python autogit.py" in make.bat too
 6. Run make_from_scratch.bat
 7. Run make.bat

## How to build (other OSes)

(It's entirely possible this doesn't work anymore)

 1. Install clang or gcc
 2. Install SDL2 development packages for your distro, or the SDL2 framework for OSX
 2. Make sure you have sdl2-config
 2. On macOS, add -framework opengl to the makefile
 3. Run make.

