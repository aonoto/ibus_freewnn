# We choose 3.4 because we need pkg_get_variable()
# (If you write fallback, this file can run with older cmake.)
cmake_minimum_required(VERSION 3.4)
project(ibus-fwnn C)

set(IBUS_FWNN_ROOT_DIR "${CMAKE_INSTALL_PREFIX}/share/ibus-fwnn")
set(IBUS_FWNN_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib/ibus-fwnn")

include(FindIntl)

#select the release build type by default to get optimization flags
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
  message(STATUS "Build type not specified: defaulting to release.")
endif(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "")


configure_file(${PROJECT_SOURCE_DIR}/src/config.h.in ${PROJECT_SOURCE_DIR}/src/config.h)
configure_file(${PROJECT_SOURCE_DIR}/fwnn.xml.in ${PROJECT_SOURCE_DIR}/fwnn.xml)

# check ibus
find_package(PkgConfig REQUIRED)
pkg_check_modules(IBUS REQUIRED ibus-1.0)

# set install dir
pkg_get_variable(IBUS_PKGDATADIR ibus-1.0 pkgdatadir)
set(IBUS_COMPONENT_DIR "${IBUS_PKGDATADIR}/component/")

# FIXME: FreeWnn does not have pkgconf support
pkg_check_modules(WNN REQUIRED)

# check libiconv
# some parts are from FindIconv on https://cmake.org/Bug/view.php?id=13517
find_library(Iconv_LIBRARIES NAMES iconv libiconv libiconv-2 c)

add_subdirectory(src)
add_subdirectory(icons)

install(FILES ${PROJECT_SOURCE_DIR}/fwnn.xml DESTINATION ${IBUS_COMPONENT_DIR})
