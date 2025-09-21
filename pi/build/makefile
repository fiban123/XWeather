CXX := g++
CXXFLAGS := -std=c++26 -I../include/ -I../../shared/include/ -MMD -MP -Wall

PI_MAIN := ../main.cpp
PI_SRC := $(wildcard ../src/*.cpp)
SHARED_SRC := $(wildcard ../../shared/src/*.cpp)

DEPS := xweatherpi.d

TARGET := xweatherpi

$(TARGET): $(SHARED_SRC) $(PI_SRC) $(PI_MAIN)
	$(CXX) $(CXXFLAGS) $(PI_SRC) $(SHARED_SRC) $(PI_MAIN) -o $(TARGET)

.PHONY: all build run build-run

all: build

build: $(TARGET)

run:
	./$(TARGET)

build-run: build run

clean:
	rm -f $(TARGET) $(DEPS)

-include $(DEPS)