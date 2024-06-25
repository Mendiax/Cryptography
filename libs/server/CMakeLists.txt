# Automatically detect all source files in the src directory
file(GLOB server_SOURCES src/*.cpp src/*.c)

add_library(server SHARED ${server_SOURCES})

target_include_directories(server PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Optionally, link other libraries if server depends on them
target_link_libraries(server PUBLIC aes dh)

# Add tests
add_executable(test_server test/test_server.c)
target_link_libraries(test_server PRIVATE server)
add_test(NAME test_server COMMAND test_server)

add_executable(test_serialize test/test_serialize.c)
target_link_libraries(test_serialize PRIVATE server)
add_test(NAME test_serialize COMMAND test_serialize)