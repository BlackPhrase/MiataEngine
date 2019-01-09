# Miata Engine

## Overview

Miata Engine is a modernized version of the [id Tech 2](https://en.wikipedia.org/wiki/Id_Tech#id_Tech_2) 
(formerly [Quake](https://en.wikipedia.org/wiki/Quake_engine)/[Quake2](https://en.wikipedia.org/wiki/Quake_II_engine)) 
engine with merged NQ/QW/Q2 codebase (the way it actually should have always been)  
The engine is (will be) able to run both [Quake](https://en.wikipedia.org/wiki/Quake_(video_game)) and [Quake 2](https://en.wikipedia.org/wiki/Quake_II) using either QC progs code or a game (dll) module. It provides all the features original engines does in addition to 
upgraded code which was (is currently being) ported to Modern C++ and [OOP](https://en.wikipedia.org/wiki/Object-oriented_programming) architecture (along with tools code). 
It might end up supporting some additional features but that's currently not in a high priority

The primary reason of modernizing the codebase is mostly for fun. Another reason is to let future generations use the engine in their learning process. 
The original code is more than 20 years old at this stage (2019) and is a subject to [software rot](https://en.wikipedia.org/wiki/Software_rot) 
which makes it more and more harder to maintain over time. There are lot of other forks out there that supports alot more features than this one, but the two distinct directions of 
this specific one are to upgrade the code to modern standards and keep its original features. There will be a bunch of notable changes though mostly related to modern games requirements 
and reduce of maintenance cost achieved by using benefits of software modularity

## Getting Started

The codebase will require a compiler with [C++17](https://en.wikipedia.org/wiki/C%2B%2B17) (or higher) standard support so make sure that the compiler type you're using does support it. 
Additionaly, you might need a Quake/Quake 2 game distribution in order to (play)test the engine. You can aquire them either from Steam or download their shareware versions

The primary building system of the repo is [CMake](https://en.wikipedia.org/wiki/CMake) so you'll feel youself comfortable if you familiar with it. It is advised to use its GUI application 
to configure/customize the build if you need to

The supported compilers are (will be) the latest versions of [Visual Studio (Build Tools)](https://en.wikipedia.org/wiki/Microsoft_Visual_C%2B%2B), [GCC](https://en.wikipedia.org/wiki/GNU_Compiler_Collection)/[MinGW](https://en.wikipedia.org/wiki/MinGW) and [Clang](https://en.wikipedia.org/wiki/Clang). Some older versions should work too but there's no guarantee of that and 
you should check for yourself

## Contributing

Feel free to contribute to the project if you like. It definitely will be appreciated. Make sure to use a branch with latest available changes (dev or master) and base your 
work above it by creating your own branch with a name that reflects the type of changes you're about to make ("feature", "bugfix", "hotfix", etc)

## License

[GNU GPLv3](LICENSE)