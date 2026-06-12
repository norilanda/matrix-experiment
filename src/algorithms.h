#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>

// Naive row-by-column transposition
void transpose_naive(std::vector<int>& matrix, int N);

// Cache-aware blocked transposition
void transpose_cache_aware(std::vector<int>& matrix, int N, int B);

// Cache-oblivious recursive transposition
void transpose_cache_oblivious(std::vector<int>& matrix, int N);

#endif // ALGORITHMS_H