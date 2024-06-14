cmake_minimum_required(VERSION 3.10)

project(Crypto)

# Set C standards
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

option(USE_AVX "Enable AVX optimizations" OFF)
if(USE_AVX)
    add_compile_definitions(USE_AVX)
    add_compile_options(-mavx2)
endif()

# Add main app
add_executable(Crypto app/main.c)

# Enable testing
enable_testing()

# Add subdirectories for libraries
add_subdirectory(libs/aes)



