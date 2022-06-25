# Settings
.PHONY := all compile test run clean

# Source directories
SRC_DIR := src/sources
HDR_DIR := src/headers
BIN_DIR := bin

# Needed files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
HEADERS := $(wildcard $(HDR_DIR)/*.hpp)
OBJECTS := $(patsubst src/sources/%.cpp, bin/%.o, $(SOURCES))

# To differentiate between the ordinary and the test main
MAINOBJ := $(filter-out bin/test.o, $(OBJECTS))
TESTOBJ := $(filter-out bin/main.o, $(OBJECTS))

# Dependencies
DEPFILES:= $(patsubst src/sources/%.cpp, bin/%.d, $(SOURCES))

# SLD2 flags
SDL2_LDFLAGS := -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# Everything needed for proper compiling
CXX      := g++
CXXFLAGS := -Wall -pedantic -std=c++17 -g
LDFLAGS  := $(SDL2_LDFLAGS)
INCLUDES := -I $(HDR_DIR)

# ----------------------------------------------------------------- #

all: neprater doc

compile: neprater

test: addTestingFlag neprater-test

run: neprater
	./neprater

# Valgrind is set to ignore SDL2 built in memory leaks
valgrind: $(MAINOBJ)
	$(CXX) $^ -o neprater  $(LDFLAGS) $(INCLUDES)
	valgrind --gen-suppressions=all --suppressions=./assets/linux_sdl_gl.sup --leak-check=full ./neprater

addTestingFlag:
	$(eval CXXFLAGS += -DTESTING)

neprater-test: $(TESTOBJ)
	$(CXX) $^ -o neprater $(LDFLAGS) $(INCLUDES) && ./neprater

neprater: $(MAINOBJ)
	$(CXX) $^ -o neprater $(LDFLAGS) $(INCLUDES)

bin/%.o: src/sources/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDES)

doc: Doxyfile $(HEADERS) ./assets/README.md
	-mkdir -p doc
	doxygen Doxyfile

clean:
	-rm -f $(BIN_DIR)/*
	-rm -f neprater
	-rm -fr doc/*

bin/%.d: src/sources/%.cpp $(HEADERS)
	-mkdir -p $(BIN_DIR)
	$(CXX) -MM $(INCLUDES) $< -MT $@ > $@

include $(DEPFILES)