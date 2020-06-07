#pragma once

#include <random>
#include <vector>
#include "DateTime.h"

using namespace std;

namespace Random {
    template<typename T>
    T getRandomInt(T min, T max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<T> dis(min, max);
        return dis(gen);
    }

    DateTime getRandomDate(DateTime min, DateTime max) {
        if (min>max) std::swap(min, max);
        unsigned int year = getRandomInt(min.getYear(), max.getYear());
        unsigned int month = getRandomInt(1, 12);
        unsigned int day = getRandomInt(1, 31);
        unsigned int hour = getRandomInt(0, 23);
        unsigned int minute = getRandomInt(0, 59);
        unsigned int sec = getRandomInt(0, 59);
        DateTime result(year, month, day, hour, minute, sec);
        if ((result < min) || (result > max)) result = getRandomDate(min, max);
        return result;
    }

    template<typename T>
    vector<T> getRandomVector(T min, T max, size_t size) {
        vector<T> result;
        for (size_t i = 0; i < size; i++) {
            result.push_back(getRandomInt(min, max));
        }
        return result;
    }

    template<>
    vector<DateTime> getRandomVector(DateTime min, DateTime max, size_t size) {
        vector<DateTime> result;
        for (size_t i = 0; i < size; i++) {
            result.push_back(getRandomDate(min, max));
        }
        return result;
    }
}

