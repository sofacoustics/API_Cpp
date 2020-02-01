cmake_minimum_required(VERSION 2.8)
project(libsofa)

include(CheckCCompilerFlag)
include(GenerateExportHeader)
include(GNUInstallDirs)

option(BUILD_TESTS "Build test programs" ON)
option(BUILD_SHARED_LIBS "Build shared library" ON)

set(CPACK_PACKAGE_DESCRIPTION "SOFA library to read and write HRTFs")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "This library reads and writes AES SOFA files")
set(CPACK_PACKAGE_VENDOR "Symonics GmbH")
set(CPACK_PACKAGE_CONTACT "umlaeute@debian.org")
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "IOhannes m zmölnig")
set(CPACK_PACKAGE_VERSION_MAJOR "2")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "netcdf-c++4")

set(PKG_CONFIG_PRIVATELIBS "")

set(PROJECT_VERSION
    "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")

check_c_compiler_flag(-Wall C_HAS_WALL)
if(C_HAS_WALL)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
endif(C_HAS_WALL)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -DVDEBUG")

if(BUILD_TESTS)
  enable_testing()
endif(BUILD_TESTS)

add_subdirectory(src)

include(CPack)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/libsofa.pc.cmake"
               "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(doxyDoc COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_HOME_DIRECTORY}/doc/doxygen/sofa_api.doxyfile"
        WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/doc/doxygen
        COMMENT "Building user's documentation into doxyDoc build dir..."
    )
endif()

