# Compiles the entirety of mitre

ROOT_DIR := .
SRC_DIR := $(ROOT_DIR)/src
OBJ_DIR := obj
BIN_DIR := bin

CXX := g++

CXX_FLAGS := -std=c++20 -O0 -Wall -Wextra
LD_FLAGS :=

CXX_SOURCES := \
	mitre.cpp \
	ServerCore.cpp \
	Configuration.cpp \
	ClientConnection.cpp \
	Request.cpp \
	Response.cpp \
	util.cpp \

OBJ = $(CXX_SOURCES:%.cpp=$(OBJ_DIR)/%.cpp.o)
DEP = $(OBJ:%.o=%.d)

.PHONY: all clean install

all: $(OBJ) Makefile
	mkdir -p $(BIN_DIR)
	$(CXX) $(LD_FLAGS) -o $(BIN_DIR)/mitre $(OBJ) -pthread

$(OBJ_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp Makefile
	mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) -MMD -c $< -o $@

install: all
	cp $(BIN_DIR)/mitre /usr/local/bin/

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
