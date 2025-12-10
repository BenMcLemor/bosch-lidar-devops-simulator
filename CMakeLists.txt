cmake_minimum_required(VERSION 3.10)
project(bosch_lidar_calibration VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags für verschiedene Build-Typen
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG -g -O0)
    message(STATUS "Building in DEBUG mode")
else()
    add_definitions(-DNDEBUG -O2)
    message(STATUS "Building in RELEASE mode")
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party)

# Haupt-Executable
add_executable(lidar_calibration_main 
    src/cpp/main.cpp
    src/cpp/lidar_calibration.cpp
)

# Setze Compiler-Flags
target_compile_options(lidar_calibration_main PRIVATE
    -Wall
    -Wextra
    -Werror
    -Wno-deprecated
)

# Link OpenMP für Parallelisierung
find_package(OpenMP REQUIRED)
target_link_libraries(lidar_calibration_main OpenMP::OpenMP_CXX)

# Installation
install(TARGETS lidar_calibration_main
    RUNTIME DESTINATION bin
)

# Test-Target (wenn Tests vorhanden sind)
enable_testing()
#add_subdirectory(test/cpp)