cmake_minimum_required(VERSION 3.10)

project(Crypto LANGUAGES C)

# Set C standards
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

option(USE_AVX "Enable AVX optimizations" OFF)
if(USE_AVX)
    # add_compile_definitions(USE_AVX)
    add_compile_options(-mavx2)
endif()

option(ENABLE_ASAN "Enable AddressSanitizer" OFF)

if (ENABLE_ASAN)
    message(STATUS "AddressSanitizer enabled")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
endif()

find_package(OpenSSL REQUIRED)
message("OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
message("OpenSSL libraries: ${OPENSSL_LIBRARIES}")

add_compile_options(-Wfatal-errors -Werror=return-type -Wall -Wextra -Wformat-truncation)
# Add main app
add_executable(client_server app/main.c)
target_link_libraries( client_server
            server
            aes
    )

# Enable testing
enable_testing()

# Add subdirectories for libraries
add_subdirectory(libs/aes)
add_subdirectory(libs/server)
add_subdirectory(libs/dh)
