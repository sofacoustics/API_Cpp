[S]patially [O]riented [F]ormat for [A]coustics, C++ library.
==============================

Library for reading and writing the standartized .sofa file format (AES69-2015).

Changes to libSOFA-master:
------------

Added cmake build file and included static windows libraries (compiled with minGW on a x86 machine for both 32 and 64 bit targets). That means it should be possible to build libSOFA with this fork on windows 32/64Bit (running on x86 architecture) on-the-fly. Cmake and the minGW-w64 (both available for i686 [32Bit] and x64 [64Bit]) compiler is required.

Instructions to build dependencies from source:
------------

Ref.: This is a build instruction based on MSYS2 with the minGW compiler on windows. It can help you on other systems too, but there you  wouldn't work with the MSYS2 console and the minGW compiler. There, you have to find the suitable command line instructions. Additionally, This is no general solution to serve all possible system situations. I'd like to see it more like a hint to get things work properly. We need curl, zlib, hdf5 and netcdf (c and c++).

Ok, here we go:

- Install MSYS2 from https://sourceforge.net/projects/msys2/ . Afterwards, an update is recommended. Open the MSYS2 console and type:

``pacman-syu`` >>>restart MSYS2 console<<< ``pacman -Su``

- We need a compiler. ``pacman -S mingw-w64-i686`` for 32 bit or ``pacman -S mingw-w64-x86_64`` for 64 bit minGW. 

- Close the MSYS2 console and start the convenient mingw32 or mingw64 console (bash scripts in MSYS2 directory), pending on the compiler installation before (32 or 64 bit).

- You have to install several libraries and linux-like library wrappers: make, dlfcn, hdf5, netcdf, curl. ``pacman -Ss [fill on your own]`` plus a name hint will inform you about avilable up-to-date packages for each compiler, try ``pacman -Ss dlfcn`` and you'll see what I mean. Zlib is shipped with MinGW, so you don't need to install it.

- Unfortunalety, netcdf-c++4 is not available yet with pacman (maybe it is at the time when you read this? Try it with the command above). We need to build it from source. Download the netcdf_c++ unix-style source code (google will help you to find it) and navigate inside the mingw shell to the directory and configure the build with ``./configure --host=x86_64-w64-mingw32 --build=x86_64-w64-mingw32`` with the 64 bit compiler version, or ``./configure --host=i686-w64-mingw32 --build=i686-w64-mingw32`` with the 32Bit compiler version. then ``make`` and ``make install`` will compile static libraries and place them in the mingw lib folder.

Congratulations, now you have all the required libraries in your MSYS/minGW library folder. Link to them when building libSOFA (easiest and dirtiest is to copy them into your source directory) and everything should work fine.
