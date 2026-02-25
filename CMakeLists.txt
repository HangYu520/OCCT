########################################################################################################################
# CMake options
########################################################################################################################
cmake_minimum_required(VERSION 3.10)

project(OCCT)

option(DEFAULT_DEBUG "Set Debug as default build type" ON) # Debug
if(NOT CMAKE_BUILD_TYPE AND DEFAULT_DEBUG)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(OpenCASCADE_DIR "/Users/hangyu/Documents/Mycode/OCCT/install/release/lib/cmake/opencascade")

########################################################################################################################
# Include 3rdParty
########################################################################################################################
find_package(OpenCASCADE REQUIRED)
message(STATUS "Found OpenCASCADE ${OpenCASCADE_VERSION}")

########################################################################################################################
# Add subdirectories
########################################################################################################################
#add_subdirectory(deps)
#add_subdirectory(src)
add_subdirectory(app)