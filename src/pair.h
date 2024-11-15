#pragma once

namespace homework {

/**
 * @brief Class template to hold two values of any types
 * @tparam T1 The type of the first value
 * @tparam T2 The type of the second value
 */

template <typename T1, typename T2>
class Pair {    
public:
    T1 first;
    T2 second;

    // Constructor to initialize the pair
    Pair(const T1& first_val, const T2& second_val) : first(first_val), second(second_val) {}
};

} // namespace homework
