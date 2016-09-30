# [Rituals](rituals.handmade.network)
A free, open source Zelda-like survival/adventure game

This repository is up early to satisfy the "Public Git Repository" tier on my [Patreon](http://patreon.com/williambundy).

(9/30) I've moved this repository to git.handmadedev.org! (but don't worry, I push to github too)

### by William Bundy
 - @William_Bundy on twitter

## Stream
I stream Rituals' development regularly at [my twitch channel](http://twitch.tv/williambundy). My stream schedule is "sometime before 2PM Pacific" every day except Monday. This usually means around 11AM

## License
Rituals' source is licensed under the MIT license. Feel free to learn as much as you can from it.
I'm planning to relicence the game under something without a "redistribute the LICENSE.txt" clause; expect an update in the future where I do so.

## How to build (windows)
 1. Install Visual Studio (I'm using 2015)
 2. Change the msvcdir variable in make.bat, if necessary
 3. Download SDL2's development libraries
 4. Set LIBRARIES and INCLUDES in make.bat to point to those
 5. You probably want to delete the line "start python autogit.py" in make.bat too
 6. Run make.bat

## How to build (other OSes)
 1. Install clang or gcc
 2. Install SDL2 development packages for your distro, or the SDL2 framework for OSX
 2. Make sure you have sdl2-config
 2. On macOS, add -framework opengl to the makefile
 3. Run make.

## How to play
The game is currently in heavy development ("pre-alpha"), so there is very little to actually play:
 - Use the WASD keys to move.
 - Shoot some kind of... shotgun type weapon with left click.
 - Press F to whack the ground. If a tile breaks, bare earth is revealed underneath.
 - Press and hold Space to grab an entity in the direction you are facing.

