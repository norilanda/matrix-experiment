#include "utils.h"
#include <random>

std::vector<int> generate_random_matrix(int N) {
    std::vector<int> matrix(N * N);
    
    int seed = 42;
    std::mt19937 gen(seed); 
    std::uniform_int_distribution<int> dist(1, 100);

    for (int i = 0; i < N * N; ++i) {
        matrix[i] = dist(gen);
    }
    
    return matrix;
}

bool verify_transpose(const std::vector<int>& expected, const std::vector<int>& actual, int N) {
    if (expected.size() != actual.size()) {
        return false;
    }
        
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            int index = i * N + j;

            if (expected[index] != actual[index]) {
                return false;
            }
        }
    }
    
    return true;
}