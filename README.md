# Miata Engine

## Overview

Miata Engine is a modernized version of the [https://en.wikipedia.org/wiki/Id_Tech_2_(disambiguation)](id Tech 2) 
(retroactively [Quake](https://en.wikipedia.org/wiki/Quake_engine)/[Quake2](https://en.wikipedia.org/wiki/Quake_II_engine)) 
engine with merged NQ/QW/Q2 codebase (the way it actually should have always been)  
The engine is (will be) able to run both Quake and Quake 2 using either QC progs code or a game (dll) module. It provides all the features original engines does in addition to 
upgraded code which was (is currently being) ported to Modern C++ and OOP architecture. It might end up supporting some additional features but that's currently not in a high priority

The primary reason of modernizing the codebase is for fun. Another reason is to let future generations to use the engine in their learning process. The original code is more than 20 years
old at this stage (2019) and is a subject to [software rot](https://en.wikipedia.org/wiki/Software_rot) which makes it more and more harder to maintain over time

## Getting Started

The codebase will require a compiler with C++17(+) standard support so make sure that your the compiler type you're using does support it. 
Additionaly, you might need a Quake/Quake 2 game distribution in order to (play)test the engine. You can aquire them either from Steam or download their shareware versions.

## Contributing

Feel free to contribute to the project if you like. It definitely will be appreciated. Make sure to use a branch with latest available changes (dev or master) and base your work above it 
by creating your own branch that reflects the type or your changes ("feature", "bugfix", "hotfix", etc)

## License

GNU GPLv3