#!/bin/bash

####################################################################
# @brief    This script build the dependencies libs for SOFA
#           namely this builds zlib, curl, hdf5, netcdf and netcdf-c++            
# @author   Thibaut Carpentier, Ircam-Centre Pompidou-CNRS
#
# @details  Tested on Linux gcc (GCC) 4.1.2 20080704 (Red Hat 4.1.2-54)
#

# the folder where the build occurs
DEP_BUILD_FOLDER=$HOME/src/sofa_netcdf/build

# the destination folder
DEP_INSTALLATION_FOLDER=$HOME/src/sofa_netcdf/destination

# 
ZLIB_LIB=zlib-1.2.8
CURL_LIB=curl-7.37.1
HDF5_LIB=hdf5-1.8.13
NETCDF_LIB=netcdf-4.3.2
NETCDF_CXX_LIB=netcdf-cxx4-4.2

# standard library
#STD_LIB=libstdc++

# build options
BUILD_OPT="-fPIC "

############################################################
# Building zlib with i386 and x86_64 arch
echo "Building: ${ZLIB_LIB}"
cd $DEP_BUILD_FOLDER
tar zxf $ZLIB_LIB.tar.gz
cd $ZLIB_LIB
CFLAGS="-O3 $BUILD_OPT " ./configure --prefix=$DEP_INSTALLATION_FOLDER --static 
make install

############################################################
# echo "Building: ${CURL_LIB}"
cd $DEP_BUILD_FOLDER
rm -Rf curl-universal
mkdir curl-universal
#mkdir curl-universal/x86_64

# Building curl with x86_64 arch
cd $DEP_BUILD_FOLDER
tar zxf $CURL_LIB.tar.gz
mv $CURL_LIB curl-universal/x86_64
cd curl-universal/x86_64
CFLAGS="-O3 $BUILD_OPT " ./configure --prefix=$DEP_INSTALLATION_FOLDER --without-ssl --disable-ldap --with-zlib=$DEP_INSTALLATION_FOLDER --disable-shared --with-pic
make install

mv $DEP_INSTALLATION_FOLDER/include/curl/* $DEP_INSTALLATION_FOLDER/include

############################################################
cd $DEP_BUILD_FOLDER
rm -Rf hdf5-universal
mkdir hdf5-universal
mkdir hdf5-universal/x86_64

# Building hdf5 with x86_64 arch
cd $DEP_BUILD_FOLDER
tar zxf $HDF5_LIB.tar.gz 
mv $HDF5_LIB hdf5-universal/x86_64
cd hdf5-universal/x86_64/$HDF5_LIB
CFLAGS="$BUILD_OPT " CPPFLAGS="$BUILD_OPT " ./configure --with-zlib=$DEP_INSTALLATION_FOLDER --prefix=$DEP_INSTALLATION_FOLDER --disable-shared --with-pic --disable-debug --enable-static-exec
make install


############################################################
NETCDF_OPTS="--enable-netcdf-4 --disable-shared --enable-dap --with-pic --disable-examples"

cd $DEP_BUILD_FOLDER
rm -Rf netcdf-universal
mkdir netcdf-universal

# Building netcdf with x86_64 arch
cd $DEP_BUILD_FOLDER
tar zxf $NETCDF_LIB.tar.gz
mv $NETCDF_LIB netcdf-universal/x86_64
cd netcdf-universal/x86_64
./configure CPPFLAGS="-I$DEP_INSTALLATION_FOLDER/include $BUILD_OPT" LDFLAGS="-L$DEP_INSTALLATION_FOLDER/lib" LIBS="-lz -lhdf5 -lhdf5_hl -lcurl -ldl" --prefix=$DEP_INSTALLATION_FOLDER $NETCDF_OPTS 
make install

############################################################
#NETCDF_CXX_OPTS="--disable-shared --disable-dap"
NETCDF_CXX_OPTS="--disable-shared --with-pic"

# Building netcdf-cxx with i386 arch
cd $DEP_BUILD_FOLDER
rm -Rf netcdf-cxx-universal
mkdir netcdf-cxx-universal


cd $DEP_BUILD_FOLDER
tar zxf $NETCDF_CXX_LIB.tar.gz
mv $NETCDF_CXX_LIB netcdf-cxx-universal/x86_64
cd netcdf-cxx-universal/x86_64

CPPFLAGS="$BUILD_OPT -I$DEP_INSTALLATION_FOLDER/include" LDFLAGS=" -L$DEP_INSTALLATION_FOLDER/lib" LIBS="-lz -lhdf5 -lhdf5_hl -lnetcdf -lcurl -ldl" ./configure --prefix=$DEP_INSTALLATION_FOLDER $NETCDF_CXX_OPTS
make install

