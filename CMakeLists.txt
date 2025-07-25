cmake_minimum_required(VERSION 3.5.1)

# Project name
project(my-project CXX)
set(CMAKE_CXX_STANDARD 23)

# To enable smart features
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler
set(CMAKE_CXX_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_STANDARD_REQUIRED True)

# Finding OpenFHE
find_package(OpenFHE CONFIG REQUIRED)
if (OpenFHE_FOUND)
    message(STATUS "FOUND PACKAGE OpenFHE")
    message(STATUS "OpenFHE Version: ${BASE_OPENFHE_VERSION}")
    message(STATUS "OpenFHE installed as shared libraries: ${OpenFHE_SHARED}")
    message(STATUS "OpenFHE include files location: ${OpenFHE_INCLUDE}")
    message(STATUS "OpenFHE lib files location: ${OpenFHE_LIBDIR}")
    message(STATUS "OpenFHE Native Backend size: ${OpenFHE_NATIVE_SIZE}")
else()
    message(FATAL_ERROR "PACKAGE OpenFHE NOT FOUND")
endif ()

set(CMAKE_CXX_FLAGS ${OpenFHE_CXX_FLAGS})

# Compilation flags
set(C_COMPILE_FLAGS "-Wall -Werror")
set(CXX_COMPILE_FLAGS "-Wall -Werror")

# Include directories (OpenMP and OpenFHE)
include_directories( ${OPENMP_INCLUDES} )
include_directories( ${OpenFHE_INCLUDE} )
include_directories( ${OpenFHE_INCLUDE}/third-party/include )
include_directories( ${OpenFHE_INCLUDE}/core )
include_directories( ${OpenFHE_INCLUDE}/pke )
include_directories( ${OpenFHE_INCLUDE}/binfhe )

link_directories(${OpenFHE_LIBDIR})
link_directories(${OPENMP_LIBRARIES})
if(BUILD_STATIC)
    set( CMAKE_EXE_LINKER_FLAGS "${OpenFHE_EXE_LINKER_FLAGS} -static")
    link_libraries( ${OpenFHE_STATIC_LIBRARIES} )
else()
    set( CMAKE_EXE_LINKER_FLAGS ${OpenFHE_EXE_LINKER_FLAGS} )
    link_libraries( ${OpenFHE_SHARED_LIBRARIES} )
endif()

# Build your project
# Look for cpp files in src/lib
file(GLOB LIB_SOURCES "${PROJECT_SOURCE_DIR}/src/lib/*.cpp")

# Look for headers in src/include
include_directories(${PROJECT_SOURCE_DIR}/src/include)

# Compile project
add_executable(main src/main.cpp ${LIB_SOURCES})
