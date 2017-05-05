This is a readme file for the SOFA C++ API.

Contact : Thibaut Carpentier
first name dot last name at ircam dot fr


The SOFA API depends on the following libraries:
- zlib
- curl
- hdf5
- netcdf
- netcdf C++

The 'dependencies' folder contains pre-compiled static libraries for OSX and linux.
I used the following versions:
zlib-1.2.8
curl-7.37.1
hdf5-1.8.13
netcdf-4.3.2
netcdf-cxx4-4.2

The OSX pre-compiled libraries are built for i386 and x86_64 architectures,
for OSX 10.6 or higher.
They were built and tested on OSX 10.9.4 with Xcode 5.1.1, using libstdc++ as the standard library.

The Linux pre-compiled libraries are built for x86_64 architecture.
They were built and tested on Red Hat 4.1.2-54 (kernel version 2.6.18-371.1.2.el5).
gcc version 4.1.2

Besides the main SOFA library, there is also 'sofainfo' which is a command line utility
that prints informations about sofa files.
The 'sofainfo.cpp' is thus a basic example on how to use the API, especially for 
a SimpleFreeFieldHRIR file.


The repository also includes additional contributions from Hagen Jaeger and Christian Hoene.
This includes:
- a sofa/json converter based on the json-c library
- cmake file for Win32 / 64 MinGW build





