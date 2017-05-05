project(libsofa)
cmake_minimum_required(VERSION 2.8)

#determine if working with 32 or 64 bit compiler
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        message(STATUS "Target is 64 bits")
        set(SOFA_EXT_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/win64" CACHE FILEPATH "description")
    else("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        message(STATUS "Target is 32 bits")
        set(SOFA_EXT_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/win32" CACHE FILEPATH "description")
    endif("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
    #additional shipped dependencies on windows
    find_library(SZ_LIB szip HINTS ${SOFA_EXT_LIB_PATH})
    find_library(M_LIB m ${SOFA_EXT_LIB_PATH})
    find_library(DL_LIB dl ${SOFA_EXT_LIB_PATH})
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(SOFA_EXT_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/linux" CACHE FILEPATH "description")
    #additional dependencies on linux, should be on system
    #find_library(SZ_LIB szip) #linux library is compiled without szlib support
    find_library(M_LIB m)
    find_library(DL_LIB dl)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(SOFA_EXT_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/macos" CACHE FILEPATH "description")
    #additional dependencies on linux, should be on system
    #find_library(SZ_LIB szip) #macos library is compiled without szlib support
    find_library(M_LIB m)
    find_library(DL_LIB dl)
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message(SEND_ERROR "Unknown system name (does not equal Windows, linux or Darwin [Mac OS X])")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

set (SOFA_EXT_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/include" CACHE FILEPATH "description")

#Shipped libraries
find_library(HDF5_LIB hdf5 HINTS ${SOFA_EXT_LIB_PATH})
find_library(HDF5_HL_LIB hdf5_hl HINTS ${SOFA_EXT_LIB_PATH})
find_library(NETCDF_LIB netcdf HINTS ${SOFA_EXT_LIB_PATH})
find_library(NETCDF_CXX_LIB netcdf_c++4 HINTS ${SOFA_EXT_LIB_PATH})
find_library(CURL_LIB curl HINTS ${SOFA_EXT_LIB_PATH})
find_library(Z_LIB z HINTS ${SOFA_EXT_LIB_PATH})

include_directories(${SOFA_EXT_INCLUDE_PATH})

add_library(sofa STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAAPI.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAAPI.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAAttributes.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAAttributes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFACoordinates.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFACoordinates.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFADate.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFADate.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAEmitter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAEmitter.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAExceptions.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAExceptions.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAFile.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAFile.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAGeneralFIR.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAGeneralFIR.h"    
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAGeneralFIRE.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAGeneralFIRE.h"    
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAGeneralTF.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAGeneralTF.h"        
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAHelper.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAHelper.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAListener.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAListener.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFANcFile.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFANcFile.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAMultiSpeakerBRIR.cpp"    
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAMultiSpeakerBRIR.h"        
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAPoint3.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAPoint3.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAPosition.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAPosition.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAReceiver.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAReceiver.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFASimpleFreeFieldHRIR.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFASimpleFreeFieldHRIR.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFASimpleFreeFieldSOS.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFASimpleFreeFieldSOS.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFASimpleHeadphoneIR.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFASimpleHeadphoneIR.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFASingleRoomDRIR.cpp"    
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFASingleRoomDRIR.h"        
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFASource.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFASource.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAString.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAString.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAUnits.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAUnits.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SOFAVersion.h")

add_executable(sofainfo "${CMAKE_CURRENT_SOURCE_DIR}/src/sofainfo.cpp")
target_link_libraries(sofainfo sofa
	${NETCDF_CXX_LIB} ${NETCDF_LIB} 
	${HDF5_HL_LIB} ${HDF5_LIB} 
	${SZ_LIB} ${Z_LIB} 
	${CURL_LIB} ${M_LIB} ${DL_LIB})

add_executable(sofamisc "${CMAKE_CURRENT_SOURCE_DIR}/src/sofamisc.cpp")
target_link_libraries(sofamisc sofa
	${NETCDF_CXX_LIB} ${NETCDF_LIB} 
	${HDF5_HL_LIB} ${HDF5_LIB} 
	${SZ_LIB} ${Z_LIB} 
	${CURL_LIB} ${M_LIB} ${DL_LIB})
