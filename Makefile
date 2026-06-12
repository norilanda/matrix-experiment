CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17
LDFLAGS = -lpapi

# Output executable name for Linux
TARGET = matrix_benchmark

# Source files (assuming they are in a src/ folder based on your original Makefile)
SRCS = src/main.cpp src/algorithms.cpp src/utils.cpp src/papi_wrapper.cpp

$(TARGET): $(SRCS)
	# Automatically create the data folder so C++ doesn't fail silently
	mkdir -p data 
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)