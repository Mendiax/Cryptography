# Variables
BUILD_DIR = build
SRC_DIR = app
LIBS_DIR = libs

# Default target
all: build_main

# Check for USE_AVX flag
ifdef USE_AVX
    CMAKE_FLAGS = -DUSE_AVX=ON
else
    CMAKE_FLAGS =
endif

ifeq ($(D),1)
    BUILD_TYPE = Debug
else
    BUILD_TYPE = Release
endif

CMAKE_ARGS := -S ./ \
	-B $(BUILD_DIR) \
	-DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
	-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \

# Build the main application
build_main:
	mkdir -p $(BUILD_DIR)
	cmake $(CMAKE_FLAGS) $(CMAKE_ARGS)
	cmake --build ./build --parallel $(NPROCS)

# Build and run all tests
test: build_main
	cd $(BUILD_DIR) && make test

# Run a specific test
run_test: build_main
	# Usage: make run_test TEST_NAME=test_<lib_name>
	cd $(BUILD_DIR) && ctest -R $(TEST_NAME)

# Run valgrind on the main application
valgrind: build_main
	valgrind --leak-check=full ./$(BUILD_DIR)/Foo

# Clean build directory
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all build_main test run_test valgrind clean
