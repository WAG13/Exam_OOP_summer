#pragma once

#include <random>
#include <vector>

using namespace std;

namespace Random {
    template<typename T>
    T getRandomInt(T min, T max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<T> dis(min, max);
        return dis(gen);
    }

    template<typename T>
    vector<T> getRandomVector(T min, T max, size_t size) {
        vector<T> result;
        for (size_t i = 0; i < size; i++) {
            result.push_back(getRandomInt(min, max));
        }
        return result;
    }
}

