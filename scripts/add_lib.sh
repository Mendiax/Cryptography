#!/bin/bash

# Check if a library name is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <library_name>"
  exit 1
fi

# Library name
LIB_NAME=$1

# Convert library name to uppercase for include guards
LIB_NAME_UPPER=$(echo "$LIB_NAME" | tr '[:lower:]' '[:upper:]')

# Create directories
mkdir -p libs/$LIB_NAME/include/$LIB_NAME
mkdir -p libs/$LIB_NAME/src
mkdir -p libs/$LIB_NAME/test

# Create include header file
bash_path=$(dirname $0)
cwd=$(pwd)
cd libs/$LIB_NAME/
echo "${cwd}/${bash_path}/add_file_c.sh"
${cwd}/${bash_path}/add_file_c.sh ${LIB_NAME}
cd ${cwd}


# Create CMakeLists.txt for the new library
cat <<EOL > libs/$LIB_NAME/CMakeLists.txt
# Automatically detect all source files in the src directory
file(GLOB ${LIB_NAME}_SOURCES src/*.cpp src/*.c)

add_library($LIB_NAME SHARED \${${LIB_NAME}_SOURCES})

target_include_directories($LIB_NAME PUBLIC
    \$<BUILD_INTERFACE:\${CMAKE_CURRENT_SOURCE_DIR}/include>
    \$<INSTALL_INTERFACE:include>
)

# Optionally, link other libraries if $LIB_NAME depends on them
# target_link_libraries($LIB_NAME PUBLIC some_other_lib)

# Add tests
add_executable(test_${LIB_NAME} test/test_${LIB_NAME}.c)
target_link_libraries(test_${LIB_NAME} PRIVATE $LIB_NAME)

# Add test to CTest
add_test(NAME test_${LIB_NAME} COMMAND test_${LIB_NAME})
EOL

# Add library to the top-level CMakeLists.txt
echo "add_subdirectory(libs/$LIB_NAME)" >> CMakeLists.txt

echo "Library $LIB_NAME has been created and added to CMakeLists.txt."
