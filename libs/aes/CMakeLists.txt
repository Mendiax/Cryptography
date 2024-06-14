# Automatically detect all source files in the src directory
file(GLOB aes_SOURCES src/*.cpp src/*.c)

add_library(aes ${aes_SOURCES})

target_include_directories(aes PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Optionally, link other libraries if aes depends on them
# target_link_libraries(aes PUBLIC some_other_lib)

# Add tests
file(GLOB aes_TEST_SOURCES test/*.cpp test/*.c)
add_executable(test_aes ${aes_TEST_SOURCES})
target_link_libraries(test_aes PRIVATE aes)

# Add test to CTest
add_test(NAME test_aes COMMAND test_aes)
