#include <iostream>
#include <vector>
#include <chrono>
#include "utils.h"
#include "algorithms.h"

void verify_and_print_result(const std::vector<int>& expected, const std::vector<int>& actual, int N, const std::string& test_name) {
    std::cout << "Verifying " << test_name << "..." << std::endl;

    if (verify_transpose(expected, actual, N)) {
        std::cout << test_name << " verification PASSED." << std::endl;
    } else {
        std::cout << test_name << " verification FAILED." << std::endl;
    }
}

int main() {
    bool verify = true;

    // 1024x1024 matrix = ~4MB of data (fits in L3 cache, exceeds L1/L2)
    int N = 1024; 
    
    std::cout << "Generating " << N << "x" << N << " matrix..." << std::endl;
    std::vector<int> src = generate_random_matrix(N);

    // --- NAIVE ---
    std::vector<int> naive_matrix(src);
    std::cout << "Running Naive Transpose..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    
    transpose_naive(naive_matrix, N);
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration_ms = end - start;

    std::cout << "Time elapsed: " << duration_ms.count() << " ms" << std::endl;

    // -- CACHE-AWARE ---
    std::vector<int> cache_aware_matrix(src);
    std::cout << "Running Cache-Aware Transpose..." << std::endl;
    start = std::chrono::high_resolution_clock::now();

    int B = 64; // Block size (tuned for cache)
    transpose_cache_aware(cache_aware_matrix, N, 64);

    end = std::chrono::high_resolution_clock::now();
    duration_ms = end - start;
    std::cout << "Time elapsed: " << duration_ms.count() << " ms" << std::endl;
    
    if (verify) {
        verify_and_print_result(naive_matrix, cache_aware_matrix, N, "Cache-Aware Transpose");
    }

    // -- CACHE-OBVIOUS ---
    std::vector<int> cache_oblivious_matrix(src);
    std::cout << "Running Cache-Oblivious Transpose..." << std::endl;
    start = std::chrono::high_resolution_clock::now();

    transpose_cache_oblivious(cache_oblivious_matrix, N);

    end = std::chrono::high_resolution_clock::now();
    duration_ms = end - start;
    std::cout << "Time elapsed: " << duration_ms.count() << " ms" << std::endl;

    if (verify) {
        verify_and_print_result(naive_matrix, cache_oblivious_matrix, N, "Cache-Oblivious Transpose");
    }

    return 0;
}