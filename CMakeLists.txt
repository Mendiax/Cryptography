cmake_minimum_required(VERSION 3.10)

project(Crypto)

# Set C standards
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

option(USE_AVX "Enable AVX optimizations" OFF)
if(USE_AVX)
    # add_compile_definitions(USE_AVX)
    add_compile_options(-mavx2)
endif()

add_compile_options(-Wfatal-errors -Werror=return-type -Wall -Wextra -Wformat-truncation)
# Add main app
add_executable(client_server app/main.c)
target_link_libraries( client_server
            server
    )

# Enable testing
enable_testing()

# Add subdirectories for libraries
add_subdirectory(libs/aes)
add_subdirectory(libs/server)

