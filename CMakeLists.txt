cmake_minimum_required(VERSION 3.10)

# =======================================================
# 📘 DBEngine Build Instructions
#
# 🔧 Default build (Development mode):
#     cmake -B build
#     cmake --build build
#
# 🚀 Release build (optimized mode):
#     cmake -B build -DCMAKE_BUILD_TYPE=Release
#     cmake --build build --config Release
#
# You can clean the build folder between builds to avoid conflicts:
#     rm -rf build
# =======================================================

project(DBEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "src/*.cpp")

add_executable(DBEngine ${SOURCES})

# =======================================================
# 🧩 Build type configuration
# =======================================================

# Available build types
set(AVAILABLE_BUILD_TYPES "Development" "Release")
set(CMAKE_BUILD_TYPE "Development" CACHE STRING "Choose the build type: Development or Release" FORCE)
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${AVAILABLE_BUILD_TYPES})

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# =======================================================
# ⚙️ Compiler flags
# =======================================================

if(CMAKE_BUILD_TYPE STREQUAL "Development")
    message(STATUS "Using development flags (debug friendly)")
    target_compile_options(DBEngine PRIVATE
        -O0             # No optimization
        -g              # Debug symbols
        -Wall -Wextra   # Enable warnings
        -Wpedantic
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Using release optimization flags")
    target_compile_options(DBEngine PRIVATE
        -O3                # Full optimization
        -march=native      # Optimize for local CPU
        -flto              # Link Time Optimization
        -ffast-math        # Fast (non-IEEE strict) math
        -fomit-frame-pointer
        -funroll-loops
    )
    target_link_options(DBEngine PRIVATE -flto)
endif()
