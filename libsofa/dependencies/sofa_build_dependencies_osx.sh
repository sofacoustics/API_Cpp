#!/bin/bash

####################################################################
# @brief    This script build the dependencies libs for SOFA
#           namely this builds zlib, curl, hdf5, netcdf and netcdf-c++            
# @author   Thibaut Carpentier, Ircam-Centre Pompidou-CNRS
#
# @details  Tested on Mac OSX 10.10.1 with Xcode 6.1.1 Apple LLVM version 6.0 (clang-600.0.56) (based on LLVM 3.5svn)
#

# the folder where the build occurs
DEP_BUILD_FOLDER=$HOME/sofa_build

# the destination folder
DEP_INSTALLATION_FOLDER=$HOME/sofa_installation

# 
ZLIB_LIB=zlib-1.2.8
CURL_LIB=curl-7.37.1
HDF5_LIB=hdf5-1.8.13
NETCDF_LIB=netcdf-4.3.2
NETCDF_CXX_LIB=netcdf-cxx4-4.2

# standard library
STD_LIB=libc++

# Mac OS SDK
OSX_SDK=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.10.sdk

# build options
BUILD_OPT="-stdlib=$STD_LIB -isysroot $OSX_SDK -mmacosx-version-min=10.7"

############################################################
# Building zlib with i386 and x86_64 arch
echo "Building: ${ZLIB_LIB}"
cd $DEP_BUILD_FOLDER
tar zxf $ZLIB_LIB.tar.gz
cd $ZLIB_LIB
CFLAGS="-arch i386 -arch x86_64 -O3 $BUILD_OPT " ./configure --prefix=$DEP_INSTALLATION_FOLDER --static
make install

############################################################
# Building curl with i386
echo "Building: ${CURL_LIB}"
cd $DEP_BUILD_FOLDER
mkdir curl-universal
tar zxf $CURL_LIB.tar.gz
mv $CURL_LIB curl-universal/i386
cd curl-universal/i386
CFLAGS="-arch i386 -O3 $BUILD_OPT " ./configure --prefix=$DEP_INSTALLATION_FOLDER --without-ssl --disable-ldap --with-zlib=$DEP_INSTALLATION_FOLDER --disable-shared --host=i386-apple-darwin 
make

# Building curl with x86_64 arch
cd $DEP_BUILD_FOLDER
tar zxf $CURL_LIB.tar.gz
mv $CURL_LIB curl-universal/x86_64
cd curl-universal/x86_64
CFLAGS="-arch x86_64 -O3 $BUILD_OPT " ./configure --prefix=$DEP_INSTALLATION_FOLDER --without-ssl --disable-ldap --with-zlib=$DEP_INSTALLATION_FOLDER --disable-shared --host=x86_64-apple-darwin
make install

rm -f $DEP_INSTALLATION_FOLDER/lib/libcurl.*

# Assembling i386 and x86_64 arch for hdf5
cd $DEP_BUILD_FOLDER/curl-universal
lipo -create -output libcurl.a ./i386/lib/.libs/libcurl.a ./x86_64/lib/.libs/libcurl.a

# Copy the assembled lib to the destination folder
mv libcurl.a $DEP_INSTALLATION_FOLDER/lib

mv $DEP_INSTALLATION_FOLDER/include/curl/* $DEP_INSTALLATION_FOLDER/include

############################################################
# Building hdf5 with i386 arch
cd $DEP_BUILD_FOLDER
mkdir hdf5-universal
tar zxf $HDF5_LIB.tar.gz 
mv $HDF5_LIB hdf5-universal/i386
cd hdf5-universal/i386
CFLAGS="-arch i386 $BUILD_OPT " ./configure --with-zlib=$DEP_INSTALLATION_FOLDER --prefix=$DEP_INSTALLATION_FOLDER --disable-shared --host=i386-apple-darwin
make

# Building hdf5 with x86_64 arch
cd $DEP_BUILD_FOLDER
tar zxf $HDF5_LIB.tar.gz 
mv $HDF5_LIB hdf5-universal/x86_64
cd hdf5-universal/x86_64
CFLAGS="-arch x86_64 $BUILD_OPT " ./configure --with-zlib=$DEP_INSTALLATION_FOLDER --prefix=$DEP_INSTALLATION_FOLDER --disable-shared --host=x86_64-apple-darwin
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
tar zxf $NETCDF_LIB.tar.gz
mkdir netcdf-universal
mv $NETCDF_LIB netcdf-universal/i386
cd netcdf-universal/i386

CPPFLAGS="-arch i386 $BUILD_OPT -I$DEP_INSTALLATION_FOLDER/include" LDFLAGS="-arch i386 -L$DEP_INSTALLATION_FOLDER/lib" LIBS="-lz -lhdf5 -lhdf5_hl -lcurl" ./configure --host=i386-apple-darwin --prefix=$DEP_INSTALLATION_FOLDER $NETCDF_OPTS
make

# Building netcdf with x86_64 arch
cd $DEP_BUILD_FOLDER
tar zxf $NETCDF_LIB.tar.gz
mkdir netcdf-universal
mv $NETCDF_LIB netcdf-universal/x86_64
cd netcdf-universal/x86_64
CPPFLAGS="-arch x86_64 $BUILD_OPT -I$DEP_INSTALLATION_FOLDER/include" LDFLAGS="-arch x86_64 -L$DEP_INSTALLATION_FOLDER/lib" LIBS="-lz -lhdf5 -lhdf5_hl -lcurl" ./configure --host=x86_64-apple-darwin --prefix=$DEP_INSTALLATION_FOLDER $NETCDF_OPTS
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
tar zxf $NETCDF_CXX_LIB.tar.gz
mkdir netcdf-cxx-universal
mv $NETCDF_CXX_LIB netcdf-cxx-universal/i386
cd netcdf-cxx-universal/i386

CPPFLAGS="-arch i386 $BUILD_OPT -I$DEP_INSTALLATION_FOLDER/include" LDFLAGS="-arch i386 -L$DEP_INSTALLATION_FOLDER/lib" LIBS="-lz -lhdf5 -lhdf5_hl -lnetcdf -lcurl" ./configure --host=i386-apple-darwin --prefix=$DEP_INSTALLATION_FOLDER $NETCDF_CXX_OPTS
make


cd $DEP_BUILD_FOLDER
tar zxf $NETCDF_CXX_LIB.tar.gz
mkdir netcdf-cxx-universal
mv $NETCDF_CXX_LIB netcdf-cxx-universal/x86_64
cd netcdf-cxx-universal/x86_64

CPPFLAGS="-arch x86_64 $BUILD_OPT -I$DEP_INSTALLATION_FOLDER/include" LDFLAGS="-arch x86_64 -L$DEP_INSTALLATION_FOLDER/lib" LIBS="-lz -lhdf5 -lhdf5_hl -lnetcdf -lcurl" ./configure --host=x86_64-apple-darwin --prefix=$DEP_INSTALLATION_FOLDER $NETCDF_CXX_OPTS
make install

rm -f $DEP_INSTALLATION_FOLDER/lib/libnetcdf_c++4*

# Assembling i386 and x86_64 arch for netcdf
cd $DEP_BUILD_FOLDER/netcdf-cxx-universal
lipo -create -output libnetcdf_c++4.a ./i386/cxx4/.libs/libnetcdf_c++4.a ./x86_64/cxx4/.libs/libnetcdf_c++4.a

mv libnetcdf_c++4.a $DEP_INSTALLATION_FOLDER/lib
