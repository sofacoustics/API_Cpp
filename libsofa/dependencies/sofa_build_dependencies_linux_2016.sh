#!/bin/bash

####################################################################
# @brief    This script build the dependencies libs for SOFA
#           namely this builds zlib, curl, hdf5, netcdf and netcdf-c++            
# @author   Thibaut Carpentier, Ircam-Centre Pompidou-CNRS
#
# @details  Tested on Linux gcc (Debian 4.9.2-10) 4.9.2
#

# the folder where the build occurs
DEP_BUILD_FOLDER=$HOME/forge/sofa_netcdf/build

# the destination folder
DEP_INSTALLATION_FOLDER=$HOME/forge/sofa_netcdf/destination

# these files must be downloaded and unzip prior to running this script
# place all these files in the DEP_BUILD_FOLDER folder

ZLIB_LIB=zlib-1.2.11
CURL_LIB=curl-7.52.1
HDF5_LIB=hdf5-1.10.0-patch1
NETCDF_LIB=netcdf-c-4.4.1.1
NETCDF_CXX_LIB=netcdf-cxx4-4.2

#ZLIB_LIB=zlib-1.2.8
#CURL_LIB=curl-7.50.3
#HDF5_LIB=hdf5-1.10.0-patch1
#NETCDF_LIB=netcdf-c-4.4.1
#NETCDF_CXX_LIB=netcdf-cxx4-4.3.0

# standard library
#STD_LIB=libstdc++

# build options
BUILD_OPT="-fPIC "

############################################################
# Building zlib with i386 and x86_64 arch
echo "Building: ${ZLIB_LIB}"
cd $DEP_BUILD_FOLDER
cd $ZLIB_LIB
make clean
chmod +x configure
CFLAGS="-O3 $BUILD_OPT " ./configure --prefix=$DEP_INSTALLATION_FOLDER --static 
make install

############################################################
# echo "Building: ${CURL_LIB}"
# Building curl with x86_64 arch
cd $DEP_BUILD_FOLDER
cd $CURL_LIB
chmod +x configure
make clean
CFLAGS="-O3 $BUILD_OPT " ./configure --prefix=$DEP_INSTALLATION_FOLDER --without-ssl --disable-ldap --with-zlib=$DEP_INSTALLATION_FOLDER --disable-shared --with-pic
make install

mv $DEP_INSTALLATION_FOLDER/include/curl/* $DEP_INSTALLATION_FOLDER/include

############################################################
# Building hdf5 with x86_64 arch
cd $DEP_BUILD_FOLDER
cd $HDF5_LIB
chmod +x configure
make clean
CFLAGS="$BUILD_OPT " CPPFLAGS="$BUILD_OPT " ./configure --with-zlib=$DEP_INSTALLATION_FOLDER --prefix=$DEP_INSTALLATION_FOLDER --disable-shared --with-pic --enable-static-exec --enable-build-mode=production
make install


############################################################
NETCDF_OPTS="--enable-netcdf-4 --disable-shared --enable-dap --with-pic --disable-examples"

# Building netcdf with x86_64 arch
cd $DEP_BUILD_FOLDER
cd $NETCDF_LIB
chmod +x configure
make clean
./configure CFLAGS="-I$DEP_INSTALLATION_FOLDER/include $BUILD_OPT" LDFLAGS="-L$DEP_INSTALLATION_FOLDER/lib" LIBS="-lz -lhdf5 -lhdf5_hl -lcurl -ldl" --prefix=$DEP_INSTALLATION_FOLDER $NETCDF_OPTS 
make install

############################################################
NETCDF_CXX_OPTS="--disable-shared --with-pic"

cd $DEP_BUILD_FOLDER
cd $NETCDF_CXX_LIB
chmod +x configure
make clean
CPPFLAGS="$BUILD_OPT -I$DEP_INSTALLATION_FOLDER/include" LDFLAGS=" -L$DEP_INSTALLATION_FOLDER/lib" LIBS="-lz -lhdf5 -lhdf5_hl -lnetcdf -lcurl -ldl" ./configure --prefix=$DEP_INSTALLATION_FOLDER $NETCDF_CXX_OPTS
make install

