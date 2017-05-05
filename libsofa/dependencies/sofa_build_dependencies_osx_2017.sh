#!/bin/bash

####################################################################
# @brief    This script build the dependencies libs for SOFA
#           namely this builds zlib, curl, hdf5, netcdf and netcdf-c++            
# @author   Thibaut Carpentier, Ircam-Centre Pompidou-CNRS
#
# @details  Tested on Mac OSX 10.10.6 with Xcode 8 Apple LLVM version 8.0.0 (clang-800.0.38)
#

# the folder where the build occurs
DEP_BUILD_FOLDER=$HOME/sofa_build

# the destination folder
DEP_INSTALLATION_FOLDER=$HOME/sofa_installation

# these files must be downloaded and unzip prior to running this script
# place all these files in the DEP_BUILD_FOLDER folder
ZLIB_LIB=zlib-1.2.11
#CURL_LIB=curl-7.50.3
# with curl higher than 7.45, the mmacosx-version-min=10.7 seems to not be respected.
# so, let's use a slightly older version of curl; should not be a problem
CURL_LIB=curl-7.45.0
HDF5_LIB=hdf5-1.10.0-patch1
NETCDF_LIB=netcdf-c-4.4.1.1
NETCDF_CXX_LIB=netcdf-cxx4-4.3.0

# standard library
STD_LIB=libc++

# Mac OS SDK
OSX_SDK=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk

# build options
BUILD_OPT="-stdlib=$STD_LIB -isysroot $OSX_SDK -mmacosx-version-min=10.7"

export MACOSX_DEPLOYMENT_TARGET="10.7" 

############################################################
# Building zlib with i386 and x86_64 arch
echo "Building: ${ZLIB_LIB}"
cd $DEP_BUILD_FOLDER
cd $ZLIB_LIB
CFLAGS="-arch i386 -arch x86_64 -Os $BUILD_OPT " ./configure --prefix=$DEP_INSTALLATION_FOLDER --static
make install

############################################################
# Building curl with i386
echo "Building: ${CURL_LIB}"
cd $DEP_BUILD_FOLDER
rm -Rf curl-universal
mkdir curl-universal
cp -R $CURL_LIB curl-universal/i386
cd curl-universal/i386
CFLAGS="-arch i386 -Os $BUILD_OPT " ./configure --prefix=$DEP_INSTALLATION_FOLDER --without-ssl --disable-ldap --with-zlib=$DEP_INSTALLATION_FOLDER --disable-shared --host=i386-apple-darwin 
make

# Building curl with x86_64 arch
cd $DEP_BUILD_FOLDER
cp -R $CURL_LIB curl-universal/x86_64
cd curl-universal/x86_64
CFLAGS="-arch x86_64 -Os $BUILD_OPT " ./configure --prefix=$DEP_INSTALLATION_FOLDER --without-ssl --disable-ldap --with-zlib=$DEP_INSTALLATION_FOLDER --disable-shared --host=x86_64-apple-darwin
make install

rm -f $DEP_INSTALLATION_FOLDER/lib/libcurl.*

# Assembling i386 and x86_64 arch for curl
cd $DEP_BUILD_FOLDER/curl-universal
lipo -create -output libcurl.a ./i386/lib/.libs/libcurl.a ./x86_64/lib/.libs/libcurl.a

# Copy the assembled lib to the destination folder
mv libcurl.a $DEP_INSTALLATION_FOLDER/lib

mv $DEP_INSTALLATION_FOLDER/include/curl/* $DEP_INSTALLATION_FOLDER/include

############################################################
# Building hdf5 with i386 arch
cd $DEP_BUILD_FOLDER
rm -Rf hdf5-universal
mkdir hdf5-universal
cp -R $HDF5_LIB hdf5-universal/i386
cd hdf5-universal/i386
CFLAGS="-arch i386 -Os $BUILD_OPT " ./configure --with-zlib=$DEP_INSTALLATION_FOLDER --prefix=$DEP_INSTALLATION_FOLDER --disable-shared --host=i386-apple-darwin
make

# Building hdf5 with x86_64 arch
cd $DEP_BUILD_FOLDER
cp -R $HDF5_LIB hdf5-universal/x86_64
cd hdf5-universal/x86_64
CFLAGS="-arch x86_64 -Os $BUILD_OPT " ./configure --with-zlib=$DEP_INSTALLATION_FOLDER --prefix=$DEP_INSTALLATION_FOLDER --disable-shared --host=x86_64-apple-darwin
make install

rm -f $DEP_INSTALLATION_FOLDER/lib/libhdf5*

# Assembling i386 and x86_64 arch for hdf5
cd $DEP_BUILD_FOLDER/hdf5-universal
lipo -create -output libhdf5.a ./i386/src/.libs/libhdf5.a ./x86_64/src/.libs/libhdf5.a
lipo -create -output libhdf5_hl.a ./i386/hl/src/.libs/libhdf5_hl.a ./x86_64/hl/src/.libs/libhdf5_hl.a

# Copy the assembled lib to the destination folder
mv libhdf5.a $DEP_INSTALLATION_FOLDER/lib
mv libhdf5_hl.a $DEP_INSTALLATION_FOLDER/lib


############################################################
# Building netcdf with i386 arch

NETCDF_OPTS="--enable-netcdf-4 --disable-shared --enable-dap"

cd $DEP_BUILD_FOLDER
rm -Rf netcdf-universal
mkdir netcdf-universal
cp -R $NETCDF_LIB netcdf-universal/i386
cd netcdf-universal/i386

CPPFLAGS="-arch i386 -Os $BUILD_OPT -I$DEP_INSTALLATION_FOLDER/include" LDFLAGS="-arch i386 -L$DEP_INSTALLATION_FOLDER/lib" LIBS="-lz -lhdf5 -lhdf5_hl -lcurl" ./configure --host=i386-apple-darwin --prefix=$DEP_INSTALLATION_FOLDER $NETCDF_OPTS
make

# Building netcdf with x86_64 arch
cd $DEP_BUILD_FOLDER
cp -R  $NETCDF_LIB netcdf-universal/x86_64
cd netcdf-universal/x86_64
CPPFLAGS="-arch x86_64 -Os $BUILD_OPT -I$DEP_INSTALLATION_FOLDER/include" LDFLAGS="-arch x86_64 -L$DEP_INSTALLATION_FOLDER/lib" LIBS="-lz -lhdf5 -lhdf5_hl -lcurl" ./configure --host=x86_64-apple-darwin --prefix=$DEP_INSTALLATION_FOLDER $NETCDF_OPTS
make install

rm -f $DEP_INSTALLATION_FOLDER/lib/libnetcdf*

# Assembling i386 and x86_64 arch for netcdf
cd $DEP_BUILD_FOLDER/netcdf-universal
lipo -create -output libnetcdf.a ./i386/liblib/.libs/libnetcdf.a ./x86_64/liblib/.libs/libnetcdf.a

mv libnetcdf.a $DEP_INSTALLATION_FOLDER/lib

############################################################

#NETCDF_CXX_OPTS="--disable-shared --disable-dap"
NETCDF_CXX_OPTS="--disable-shared"

# Building netcdf-cxx with i386 arch
cd $DEP_BUILD_FOLDER
rm -Rf netcdf-cxx-universal
mkdir netcdf-cxx-universal
cp -R $NETCDF_CXX_LIB netcdf-cxx-universal/i386
cd netcdf-cxx-universal/i386

CPPFLAGS="-arch i386 -Os $BUILD_OPT -I$DEP_INSTALLATION_FOLDER/include" LDFLAGS="-arch i386 -L$DEP_INSTALLATION_FOLDER/lib" LIBS="-lz -lhdf5 -lhdf5_hl -lnetcdf -lcurl" ./configure --host=i386-apple-darwin --prefix=$DEP_INSTALLATION_FOLDER $NETCDF_CXX_OPTS
make


cd $DEP_BUILD_FOLDER
cp -R $NETCDF_CXX_LIB netcdf-cxx-universal/x86_64
cd netcdf-cxx-universal/x86_64

CPPFLAGS="-arch x86_64 -Os $BUILD_OPT -I$DEP_INSTALLATION_FOLDER/include" LDFLAGS="-arch x86_64 -L$DEP_INSTALLATION_FOLDER/lib" LIBS="-lz -lhdf5 -lhdf5_hl -lnetcdf -lcurl" ./configure --host=x86_64-apple-darwin --prefix=$DEP_INSTALLATION_FOLDER $NETCDF_CXX_OPTS
make install

rm -f $DEP_INSTALLATION_FOLDER/lib/libnetcdf_c++4*

# Assembling i386 and x86_64 arch for netcdf
cd $DEP_BUILD_FOLDER/netcdf-cxx-universal
lipo -create -output libnetcdf_c++4.a ./i386/cxx4/.libs/libnetcdf_c++4.a ./x86_64/cxx4/.libs/libnetcdf_c++4.a

mv libnetcdf_c++4.a $DEP_INSTALLATION_FOLDER/lib
