# fps6
This is a game which is based on the fps tutorial from GameMaker, specifically the 6th part, which is basically a Wolfenstein 3D clone with a single level. This version however was extended by cheats, mouse support, music, an indicator of when you have finished the level and a random level mode, where new levels are generated on the fly. It was originally written in the spring of 2017.

## Building
This project uses cmake for building and has dependencies on [SDL](https://www.libsdl.org/) and [SDL mixer](https://www.libsdl.org/projects/SDL_mixer/). I recommend using the prebuilt binaries, because I'm not sure if it would even work otherwise. Don't forget to add SDL2main to the `SDL_LIBRARY` variable.

## Running
For running the game the binaries from SDL and SDL mixer are needed. The archive `data.img` containing the assets is also needed. It can be obtained by running [fps6-builder](https://github.com/falki147/fps6-assets)
