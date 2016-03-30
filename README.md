# libWRP-key

C++ cross platform key-code library
 with corss platform virtual keyboard write(send) key library.

## Support

- (Supported)
    - GNU/Linux
    - Mac/OSX
    - Microsoft/Windows

## Library

The library is in the "./include" directory.

### Install

#### for GNU/Linux or OSX

to system: install to /usr/local

    sudo ./install.sh

to the other:

    PREFIX=~/opt ./install.sh

#### Windows

You should be set appropriately for your environment.

## Example

[example/key.cxx](example/key.cxx)

    cd example
    mkdir build
    cd build
    cmake -G Ninja ..
    ninja
    ./key

[example/writer.cxx](example/writer.cxx)

GNU/Linux: * need root *

    sudo ./writer

OSX or Windows:

    ./writer

### Note for Windows

you must need copy or symlink two .dll files from your development environment lib directory, maybe:

1. libgcc_s_sjlj-1.dll
2. libstdc++-6.dll

## Requirement

* C++11
    * g++ >= 4.6 (Recommend >= 4.7)
    * clang++ >= 3.3

## Licence

* [libWRP-key is licensed under The Happy Bunny License and MIT License](LICENCE.md)

## Author

(C)2014 Usagi Ito<usagi@WonderRabbitProject.net> / [Wonder Rabbit Project](http://www.WonderRabbitProject.net/).
