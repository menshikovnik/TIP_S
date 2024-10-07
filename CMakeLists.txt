cmake_minimum_required(VERSION 3.29)
project(TIP_S)

include(ExternalProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS REQUIRED Widgets PrintSupport)
include_directories(${PROJECT_SOURCE_DIR})

MESSAGE(STATUS "Trying to install fftw...")

ExternalProject_Add(project_fftw
        #GIT_REPOSITORY  https://github.com/FFTW/fftw3
        URL http://www.fftw.org/fftw-3.3.2.tar.gz
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/fftw
        CONFIGURE_COMMAND
        ${CMAKE_CURRENT_BINARY_DIR}/fftw/src/project_fftw/configure
        --prefix=${CMAKE_CURRENT_BINARY_DIR}/fftw/install
        INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/fftw/install
)

add_library(fftw STATIC IMPORTED)

set(lib_fftw_name
        ${CMAKE_STATIC_LIBRARY_PREFIX}fftw3${CMAKE_STATIC_LIBRARY_SUFFIX})
set_target_properties(fftw PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_BINARY_DIR}/fftw/install/lib/${lib_fftw_name})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/fftw/install/include)
include_directories(${CMAKE_BINARY_DIR}/TIP_S_autogen/include)

add_executable(TIP_S main.cpp
        harmonic_vibration_and_meander.cpp
        harmonic_vibration_and_meander.h
        harmonic_vibration_and_meander.ui
        qcustomplot.cpp
        qcustomplot.h
)

add_dependencies(${PROJECT_NAME} project_fftw)

target_link_libraries(TIP_S PRIVATE Qt6::Widgets Qt6::PrintSupport fftw)