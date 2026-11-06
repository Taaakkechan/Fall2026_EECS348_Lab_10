CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = matrix

# Windows has no rm, so use cmd.exe and del there
ifeq ($(OS),Windows_NT)
SHELL = cmd.exe
CLEAN = if exist $(TARGET).exe del /Q $(TARGET).exe
else
CLEAN = rm -f $(TARGET)
endif

all: $(TARGET)

$(TARGET): matrix.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) matrix.cpp

clean:
	$(CLEAN)

.PHONY: all clean
