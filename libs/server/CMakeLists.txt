# Automatically detect all source files in the src directory
file(GLOB server_SOURCES src/*.cpp src/*.c)

add_library(server SHARED ${server_SOURCES})

target_include_directories(server PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Optionally, link other libraries if server depends on them
# target_link_libraries(server PUBLIC some_other_lib)

# Add tests
add_executable(test_server test/test_server.c)
target_link_libraries(test_server PRIVATE server)

# Add test to CTest
add_test(NAME test_server COMMAND test_server)
