## Simple Makefile for GulfOfMexicoInterpreter and tests

# Compiler and flags
CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic

# On some older libstdc++ versions, filesystem needs an extra link flag.
# If you get link errors about std::filesystem, try:
#   make FS_LIBS=-lstdc++fs
FS_LIBS ?=

TARGET      := GulfOfMexicoInterpreter
TEST_RUNNER := tests/testRunner

SRC_DIR     := .
UTIL_DIR    := util

SRCS        := $(SRC_DIR)/main.cpp \
               $(UTIL_DIR)/files/FileUtils.cpp \
               $(UTIL_DIR)/transformations/Transformations.cpp

TEST_SRCS   := tests/testRunner.cpp \
               $(UTIL_DIR)/FileUtils.cpp

OBJS        := $(SRCS:.cpp=.o)
TEST_OBJS   := $(TEST_SRCS:.cpp=.o)

.PHONY: all project tests clean test

# Default: build everything
all: project tests

# Build only the main interpreter
project: $(TARGET)

# Build only the test runner (and its deps)
tests: $(TEST_RUNNER)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(FS_LIBS)

$(TEST_RUNNER): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(FS_LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: project tests
	./$(TEST_RUNNER)

clean:
	$(RM) $(OBJS) $(TEST_OBJS) $(TARGET) $(TEST_RUNNER)

