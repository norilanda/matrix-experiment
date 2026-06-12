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
