# Compiles the entirety of msite

MAJOR_VERSION := 0
MINOR_VERSION := 1
PATCH_VERSION := 0

ROOT_DIR := ..
SRC_DIR := $(ROOT_DIR)/src
OBJ_DIR := obj
BIN_DIR := bin

CXX := g++
#LD := ld

CXX_FLAGS := -std=c++17 -O3 -Wall -Wextra -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION)-D -DPATCH_VERSION=$(PATCH_VERSION)
LD_FLAGS :=

CXX_SOURCES := \
	msite.cpp

OBJ = $(CXX_SOURCES:%.cpp=$(OBJ_DIR)/%.cpp.o)
DEP = $(OBJ:%.o=%.d)

.PHONY: all clean install

all: $(OBJ)
	mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/msite $(OBJ)

$(OBJ_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

install:
	@echo "To be made lol"

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)