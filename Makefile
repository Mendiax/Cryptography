APP_NAME := main

FOLDER_APP := ./app
FOLDER_SRC := ./src
FOLDER_INC := ./inc
FOLDER_BUILD := ./build

APP_CPP := $(wildcard $(FOLDER_APP)/*.cpp)
SRC_CPP := $(wildcard $(FOLDER_SRC)/*.cpp)
APP_C := $(wildcard $(FOLDER_APP)/*.c)
SRC_C := $(wildcard $(FOLDER_SRC)/*.c)

I_INC := $(foreach d, $(FOLDER_INC), -I$d)

OBJ_SRC_CPP := $(SRC_CPP:$(FOLDER_SRC)/%.cpp=$(FOLDER_BUILD)/%.o)
OBJ_APP_CPP := $(APP_CPP:$(FOLDER_APP)/%.cpp=$(FOLDER_BUILD)/%.o)
OBJ_SRC_C := $(SRC_C:$(FOLDER_SRC)/%.c=$(FOLDER_BUILD)/%.o)
OBJ_APP_C := $(APP_C:$(FOLDER_APP)/%.c=$(FOLDER_BUILD)/%.o)

# Set the C++ compiler
CXX ?= g++
CC ?= gcc

CPP_STD   := -std=c++17
CPP_OPT   := -O3
CPP_FLAGS :=
CPP_WARNS :=

C_STD   := -std=c11
C_OPT   := -O3
C_FLAGS :=
C_WARNS :=

ifeq ("$(origin DEBUG)", "command line")
    GGDB := -ggdb3
    GDB := -ggdb3
    CPP_OPT := -O0
    C_OPT := -O0
else
    GGDB :=
    GDB :=
endif

ifeq ($(CXX),clang)
    CPP_WARNS +=  -Weverything -Wno-padded -Wno-gnu-zero-variadic-macro-arguments -Wno-newline-eof -Wno-reserved-id-macro \
                -Wno-missing-variable-declarations -Wno-declaration-after-statement
else ifneq (, $(filter $(CXX), cc gcc))
    CPP_WARNS += -Wall -Wextra -pedantic -Wcast-align \
               -Winit-self -Wlogical-op -Wmissing-include-dirs \
               -Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wundef  \
               -Wwrite-strings -Wpointer-arith -Wmissing-declarations \
               -Wuninitialized -Wno-old-style-definition -Wno-old-style-declaration -Wstrict-prototypes \
               -Wmissing-prototypes -Wswitch-default -Wbad-function-cast \
               -Wnested-externs -Wconversion -Wunreachable-code
endif

C_WARNS += -Wall -Wextra -pedantic

CPP_FLAGS += $(CPP_STD) $(CPP_OPT) $(GGDB) $(CPP_WARNS)
C_FLAGS += $(C_STD) $(C_OPT) $(GDB) $(C_WARNS)

$(FOLDER_BUILD)/%.o: $(FOLDER_SRC)/%.cpp
	$(CXX) $(CPP_FLAGS) $(I_INC) -c $< -o $@

$(FOLDER_BUILD)/%.o: $(FOLDER_APP)/%.cpp
	$(CXX) $(CPP_FLAGS) $(I_INC) -c $< -o $@

$(FOLDER_BUILD)/%.o: $(FOLDER_SRC)/%.c
	$(CC) $(C_FLAGS) $(I_INC) -c $< -o $@

$(FOLDER_BUILD)/%.o: $(FOLDER_APP)/%.c
	$(CC) $(C_FLAGS) $(I_INC) -c $< -o $@

all: app

.PHONY: app
app: $(OBJ_SRC_CPP) $(OBJ_APP_CPP) $(OBJ_SRC_C) $(OBJ_APP_C)
	$(CXX) $(CPP_FLAGS) $(I_INC) $(OBJ_SRC_CPP) $(OBJ_APP_CPP) $(OBJ_SRC_C) $(OBJ_APP_C) -o $(APP_NAME)

.PHONY: clean
clean:
	rm -f $(APP_NAME)
	rm -rf $(FOLDER_BUILD)
	mkdir -p $(FOLDER_BUILD)
	mkdir -p $(FOLDER_BUILD)/$(FOLDER_SRC)
	mkdir -p $(FOLDER_BUILD)/$(FOLDER_APP)

compile_commands:
	bear -- make