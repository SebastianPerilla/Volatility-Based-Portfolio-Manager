#pragma once
#include <vector>
#include <numeric>

namespace homework {

/**
 * @brief Template function for summing the elements of a vector holding elements of any type
 * @param s The vector to sum
 * @param v The initial value of the sum
 * @tparam T The type of the elements in the vector
 * @return The sum of the elements in the vector, plus the initial value
 */
template<typename T>
T sum_elements(const std::vector<T>& s, T v = T()) {
    return std::accumulate(s.begin(), s.end(), v);
}

} // namespace homework
