#include "algorithms.h"
#include <algorithm>

constexpr int OBLIVIOUS_THRESHOLD = 16;

void transpose_naive(std::vector<int>& matrix, int N) {
    for (int i = 0; i < N; ++i)
    {
        for (int j = i + 1; j < N; ++j)
        {
            std::swap(matrix[i * N + j], matrix[j * N + i]);
        }
    }
}

void transpose_cache_aware(std::vector<int>& matrix, int N, int B) {
    for (int i = 0; i < N; i += B)
    {
        for (int j = i; j < N; j += B)
        {
            if (i == j) {
                // Diagonal block: Transpose internally
                int max_r = std::min(i + B, N);
                int max_c = std::min(j + B, N);
                for (int r = i; r < max_r; ++r)
                {
                    for (int c = r + 1; c < max_c; ++c) {
                        std::swap(matrix[r * N + c], matrix[c * N + r]);
                    }
                }
            }
            else
            {
                // Off-diagonal block: Swap block (i, j) with block (j, i)
                int max_r = std::min(i + B, N);
                int max_c = std::min(j + B, N);
                for (int r = i; r < max_r; ++r)
                {
                    for (int c = j; c < max_c; ++c) {
                        // The symmetric coordinate across the main diagonal
                        int sym_r = c; 
                        int sym_c = r;
                        std::swap(matrix[r * N + c], matrix[sym_r * N + sym_c]);
                    }
                }
            }
        }
    }
}

void swap_transpose_rec(std::vector<int>& matrix, int r1, int c1, int r2, int c2, int h, int w, int N) {
    // Base case: When the block is small enough to stop recursion overhead
    if (h <= OBLIVIOUS_THRESHOLD && w <= OBLIVIOUS_THRESHOLD) {
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                std::swap(matrix[(r1 + i) * N + (c1 + j)], matrix[(r2 + j) * N + (c2 + i)]);
            }
        }
    } else if (w >= h) {
        // Split along the width
        int half_w = w / 2;
        swap_transpose_rec(matrix, r1, c1, r2, c2, h, half_w, N);
        swap_transpose_rec(matrix, r1, c1 + half_w, r2 + half_w, c2, h, w - half_w, N);
    } else {
        // Split along the height
        int half_h = h / 2;
        swap_transpose_rec(matrix, r1, c1, r2, c2, half_h, w, N);
        swap_transpose_rec(matrix, r1 + half_h, c1, r2, c2 + half_h, h - half_h, w, N);
    }
}

void transpose_inplace_rec(std::vector<int>& matrix, int r, int c, int size, int N) {
    if (size <= OBLIVIOUS_THRESHOLD) {
        // Base case: Naive transpose for a small block
        for (int i = 0; i < size; ++i) {
            for (int j = i + 1; j < size; ++j) {
                std::swap(matrix[(r + i) * N + (c + j)], matrix[(r + j) * N + (c + i)]);
            }
        }
    } else {
        // Divide into 4 quadrants
        int half = size / 2;
        
        // 1. Transpose top-left quadrant (A11)
        transpose_inplace_rec(matrix, r, c, half, N);
        
        // 2. Transpose bottom-right quadrant (A22)
        transpose_inplace_rec(matrix, r + half, c + half, size - half, N);
        
        // 3. Swap and transpose the top-right (A12) and bottom-left (A21) quadrants
        swap_transpose_rec(matrix, r, c + half, r + half, c, half, size - half, N);
    }
}

void transpose_cache_oblivious(std::vector<int>& matrix, int N) {
    transpose_inplace_rec(matrix, 0, 0, N, N);
}