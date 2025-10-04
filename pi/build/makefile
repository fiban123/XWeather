CXX := g++
CXXFLAGS := -std=c++26 -I../include/ -I../../shared/include/ -MMD -MP -Wall

DEBUGFLAGS := -ggdb -O0

# Source files
PI_MAIN := ../main.cpp
PI_SRC := $(wildcard ../src/*.cpp)
SHARED_SRC := $(wildcard ../../shared/src/*.cpp)
SRC := $(PI_MAIN) $(PI_SRC) $(SHARED_SRC)

# Object and dependency files inside ./obj
OBJDIR := ./obj
OBJ := $(patsubst %.cpp,$(OBJDIR)/%.o,$(notdir $(SRC)))
DEPS := $(OBJ:.o=.d)

TARGET := xweatherpi
DEBUG_TARGET := xweatherpi_debug

# Default target
all: build

# Link final executables
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(DEBUG_TARGET): CXXFLAGS += $(DEBUGFLAGS)
$(DEBUG_TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Ensure obj directory exists
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Pattern rules – compile to ./obj regardless of source location
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: ../src/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: ../../shared/src/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: ../%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all build run build-run debug debug-run clean

build: $(TARGET)
debug: $(DEBUG_TARGET)

run:
	./$(TARGET)

build-run: build
	./$(TARGET)

debug-run: debug
	gdb ./$(DEBUG_TARGET)

clean:
	rm -rf $(TARGET) $(DEBUG_TARGET) $(OBJDIR)

-include $(DEPS)