# Automatically detect all source files in the src directory
file(GLOB dh_SOURCES src/*.cpp src/*.c)

add_library(dh SHARED ${dh_SOURCES})

target_include_directories(dh PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Optionally, link other libraries if dh depends on them
target_link_libraries(dh PUBLIC ${OPENSSL_LIBRARIES})

# Add tests
add_executable(test_dh test/test_dh.c)
target_link_libraries(test_dh PRIVATE dh ${OPENSSL_LIBRARIES})

# Add test to CTest
add_test(NAME test_dh COMMAND test_dh)

# Add tests
add_executable(test_ec test/test_ec.c)
target_link_libraries(test_ec PRIVATE dh ${OPENSSL_LIBRARIES})

# Add test to CTest
add_test(NAME test_ec COMMAND test_ec)


# Add tests
add_executable(test_ecpp test/test_dh.cpp)
target_link_libraries(test_ecpp PRIVATE dh ${OPENSSL_LIBRARIES})

# Add test to CTest
add_test(NAME test_ecpp COMMAND test_ecpp)
