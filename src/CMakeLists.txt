macro(use_c99)
  if(CMAKE_VERSION VERSION_LESS "3.1")
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
      set(CMAKE_C_FLAGS "--std=gnu99 -pedantic ${CMAKE_C_FLAGS}")
    endif()
  else()
    set(CMAKE_C_STANDARD 99)
  endif()
endmacro(use_c99)

use_c99()

configure_file(config.h.in ${PROJECT_SOURCE_DIR}/src/config.h)

if(NOT MSVC)
  if(NOT WIN32)
    find_library(MATH m)
  else()
    set(MATH "")
  endif()
  include(FindZLIB)
else()
  set(MATH "")
  find_program(NUGET nuget)
  if(NUGET)
    execute_process(COMMAND ${NUGET} install zlib)
  endif()
  include_directories(
    ${PROJECT_SOURCE_DIR}/windows/third-party/zlib-1.2.11/include/)
endif()

# include(FindZLIB) include_directories(${ZLIB_INCLUDE_DIRS}) MESSAGE(STATUS
# "ZLIB libs" ${ZLIB_LIBRARIES})

if(NOT MSVC)
  if(NOT WIN32)
    find_library(MATH m)
  else()
    set(MATH "")
  endif()
  include(FindZLIB)
  include_directories(${ZLIB_INCLUDE_DIRS})
  set(PKG_CONFIG_PRIVATELIBS "-lm ${PKG_CONFIG_PRIVATELIBS}")
  set(PKG_CONFIG_PRIVATELIBS "-lz ${PKG_CONFIG_PRIVATELIBS}")
else()
  set(MATH "")
  find_program(NUGET nuget)
  if(NOT NUGET)
    message(
      FATAL
      "Cannot find nuget command line tool.\nInstall it with e.g. choco install nuget.commandline"
    )
  else()
    execute_process(COMMAND ${NUGET} install zlib)
  endif()
  include_directories(
    ${PROJECT_SOURCE_DIR}/windows/third-party/zlib-1.2.11/include/)
endif()

# set(libsrc hrtf/reader.c hdf/superblock.c hdf/dataobject.c hdf/btree.c
# hdf/fractalhead.c hdf/gunzip.c hdf/gcol.c hrtf/check.c hrtf/spherical.c
# hrtf/lookup.c hrtf/tools.c hrtf/kdtree.c hrtf/neighbors.c hrtf/interpolate.c
# hrtf/resample.c hrtf/loudness.c hrtf/minphase.c hrtf/easy.c hrtf/cache.c
# resampler/speex_resampler.c) add_library(mysofa-static STATIC ${libsrc})
# target_link_libraries (mysofa-static ${MATH} ${ZLIB_LIBRARIES})
# SET_TARGET_PROPERTIES(mysofa-static PROPERTIES OUTPUT_NAME mysofa
# CLEAN_DIRECT_OUTPUT 1 POSITION_INDEPENDENT_CODE ON) install(TARGETS mysofa-
# static ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(BUILD_SHARED_LIBS)
  # add_library(mysofa-shared SHARED ${libsrc}) target_link_libraries (mysofa-
  # shared ${MATH} ${ZLIB_LIBRARIES}) SET_TARGET_PROPERTIES(mysofa-shared
  # PROPERTIES OUTPUT_NAME mysofa CLEAN_DIRECT_OUTPUT 1) set_property(TARGET
  # mysofa-shared PROPERTY VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKA
  # GE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}") set_property(TARGET
  # mysofa-shared PROPERTY SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR} )
  # set_property(TARGET mysofa-shared PROPERTY C_VISIBILITY_PRESET hidden)
  # GENERATE_EXPORT_HEADER(mysofa-shared BASE_NAME mysofa EXPORT_FILE_NAME
  # ${PROJECT_SOURCE_DIR}/src/hrtf/mysofa_export.h) install(TARGETS mysofa-
  # shared RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} LIBRARY DESTINATION
  # ${CMAKE_INSTALL_LIBDIR})
else()
  # GENERATE_EXPORT_HEADER(mysofa-static BASE_NAME mysofa EXPORT_FILE_NAME
  # ${PROJECT_SOURCE_DIR}/src/hrtf/mysofa_export.h)
endif()

# install(FILES hrtf/mysofa.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

if(BUILD_TESTS)
  # add_executable(mysofa2json tests/sofa2json.c tests/json.c)
  # target_link_libraries (mysofa2json mysofa-static)

  # add_executable(external tests/external.c tests/check.c tests/lookup.c
  # tests/neighbors.c tests/interpolate.c tests/resample.c tests/loudness.c
  # tests/minphase.c tests/easy.c tests/cache.c tests/json.c
  # tests/user_defined_variable.c) target_link_libraries (external mysofa-shared
  # ${CUNIT_LIBRARIES}) add_test(NAME external WORKING_DIRECTORY
  # ${PROJECT_SOURCE_DIR} COMMAND external)

  # add_executable(internal tests/internal.c tests/tools.c hrtf/tools.c)
  # target_link_libraries (internal m ${CUNIT_LIBRARIES}) add_test(NAME internal
  # WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMAND internal)

  # add_executable(multithread tests/multithread.c) target_link_libraries
  # (multithread mysofa-shared pthread) add_test(NAME multithread
  # WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMAND multithread)

  # install(TARGETS mysofa2json RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  # ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} LIBRARY DESTINATION
  # ${CMAKE_INSTALL_LIBDIR})
endif(BUILD_TESTS)

set(PKG_CONFIG_PRIVATELIBS
    "${PKG_CONFIG_PRIVATELIBS}"
    PARENT_SCOPE)

# determine if working with 32 or 64 bit compiler
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
    message(STATUS "Target is 64 bits")
    set(SOFA_EXT_LIB_PATH
        "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/win64"
        CACHE FILEPATH "description")
  else("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
    message(STATUS "Target is 32 bits")
    set(SOFA_EXT_LIB_PATH
        "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/win32"
        CACHE FILEPATH "description")
  endif("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
  # additional shipped dependencies on windows
  find_library(SZ_LIB szip HINTS ${SOFA_EXT_LIB_PATH})
  find_library(M_LIB m ${SOFA_EXT_LIB_PATH})
  find_library(DL_LIB dl ${SOFA_EXT_LIB_PATH})
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  # set(SOFA_EXT_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/linux"
  # CACHE FILEPATH "description")
  # additional dependencies on linux, should be on system find_library(SZ_LIB
  # szip) #linux library is compiled without szlib support
  find_library(M_LIB m)
  find_library(DL_LIB dl)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(SOFA_EXT_LIB_PATH
      "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/macos"
      CACHE FILEPATH "description")
  # additional dependencies on linux, should be on system find_library(SZ_LIB
  # szip) #macos library is compiled without szlib support
  find_library(M_LIB m)
  find_library(DL_LIB dl)
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  message(
    SEND_ERROR
      "Unknown system name (does not equal Windows, linux or Darwin [Mac OS X])"
  )
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

# set (SOFA_EXT_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/include"
# CACHE FILEPATH "description")

# Shipped libraries find_library(HDF5_LIB hdf5 HINTS ${SOFA_EXT_LIB_PATH})
# find_library(HDF5_HL_LIB hdf5_hl HINTS ${SOFA_EXT_LIB_PATH})
# find_library(NETCDF_LIB netcdf HINTS ${SOFA_EXT_LIB_PATH})
find_library(NETCDF_CXX_LIB netcdf_c++4)
# find_library(Z_LIB z HINTS ${SOFA_EXT_LIB_PATH})

# include_directories(${SOFA_EXT_INCLUDE_PATH})

add_library(
  sofa STATIC
  SOFAAPI.cpp
  SOFAAPI.h
  SOFAAttributes.cpp
  SOFAAttributes.h
  SOFACoordinates.cpp
  SOFACoordinates.h
  SOFADate.cpp
  SOFADate.h
  SOFAEmitter.cpp
  SOFAEmitter.h
  SOFAExceptions.cpp
  SOFAExceptions.h
  SOFAFile.cpp
  SOFAFile.h
  SOFAGeneralFIR.cpp
  SOFAGeneralFIR.h
  SOFAGeneralFIRE.cpp
  SOFAGeneralFIRE.h
  SOFAGeneralTF.cpp
  SOFAGeneralTF.h
  SOFAHelper.cpp
  SOFAHelper.h
  SOFAListener.cpp
  SOFAListener.h
  SOFANcFile.cpp
  SOFANcFile.h
  SOFAMultiSpeakerBRIR.cpp
  SOFAMultiSpeakerBRIR.h
  SOFAPoint3.cpp
  SOFAPoint3.h
  SOFAPosition.cpp
  SOFAPosition.h
  SOFAReceiver.cpp
  SOFAReceiver.h
  SOFASimpleFreeFieldHRIR.cpp
  SOFASimpleFreeFieldHRIR.h
  SOFASimpleFreeFieldSOS.cpp
  SOFASimpleFreeFieldSOS.h
  SOFASimpleHeadphoneIR.cpp
  SOFASimpleHeadphoneIR.h
  SOFASingleRoomDRIR.cpp
  SOFASingleRoomDRIR.h
  SOFASource.cpp
  SOFASource.h
  SOFAString.cpp
  SOFAString.h
  SOFAUnits.cpp
  SOFAUnits.h
  SOFAVersion.h)
target_link_libraries(sofa ${NETCDF_CXX_LIB})

add_executable(sofainfo sofainfo.cpp)
target_link_libraries(sofainfo sofa)

add_executable(sofamisc sofamisc.cpp)
target_link_libraries(sofamisc sofa)

add_executable(sofa2json sofa2json.cpp)
target_link_libraries(sofa2json sofa json-c)

add_executable(json2sofa json2sofa.cpp)
target_link_libraries(json2sofa sofa json-c)


if(BUILD_TESTS)
  add_test(sofainfo ./sofainfo 
           ../../tests/MIT_KEMAR_large_pinna.sofa)
  add_test(sofamisc ./sofamisc
           ../../tests/MIT_KEMAR_large_pinna.sofa)
  add_test(sofa2json ./sofa2json 
           ../../tests/MIT_KEMAR_large_pinna.sofa)
  add_test(json2sofa ./json2sofa 
           ../../tests/MIT_KEMAR_large_pinna.json)

endif(BUILD_TESTS)
