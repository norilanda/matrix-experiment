CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17

# Output executable name for Windows
TARGET = matrix_benchmark.exe

# Source files
SRCS = src/main.cpp src/algorithms.cpp src/utils.cpp

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	del $(TARGET)