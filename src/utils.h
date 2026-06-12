#ifndef UTILS_H
#define UTILS_H

#include <vector>

std::vector<int> generate_random_matrix(int N);

bool verify_transpose(const std::vector<int>& expected, const std::vector<int>& actual, int N);

#endif // UTILS_H