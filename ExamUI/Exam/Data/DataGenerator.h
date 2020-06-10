#pragma once

#include "Random.h"
#include "DateTime.h"
#include <vector>
#include <algorithm>

using namespace std;

DateTime getRandomDate(DateTime min, DateTime max) {
    if (min>max) std::swap(min, max);
    unsigned int year = Random::getRandomInt(min.getYear(), max.getYear());
    unsigned int month = Random::getRandomInt(1, 12);
    unsigned int day = Random::getRandomInt(1, 31);
    unsigned int hour = Random::getRandomInt(0, 23);
    unsigned int minute = Random::getRandomInt(0, 59);
    unsigned int sec = Random::getRandomInt(0, 59);
    DateTime result(year, month, day, hour, minute, sec);
    if ((result < min) || (result > max)) result = getRandomDate(min, max);
    return result;
}

template<>
vector<DateTime> Random::getRandomVector(DateTime min, DateTime max, size_t size) {
    vector<DateTime> result;
    for (size_t i = 0; i < size; i++) {
        result.push_back(getRandomDate(min, max));
    }
    return result;
}

template<typename T>
class DataGenerator
{
public:
    DataGenerator();
    virtual ~DataGenerator();

    virtual vector<T> generateVector(T min, T max, size_t count) = 0;
};

template<typename T>
class RandomDataGenerator : public DataGenerator<T>
{
public:
    ~RandomDataGenerator() override;

    vector<T> generateVector(T min, T max, size_t count) override;
};


template<typename T>
DataGenerator<T>::DataGenerator() {}

template<typename T>
DataGenerator<T>::~DataGenerator() {}


template<typename T>
RandomDataGenerator<T>::~RandomDataGenerator<T>() {}

template<typename T>
vector<T> RandomDataGenerator<T>::generateVector(T min, T max, size_t count) {
    return Random::getRandomVector(min, max, count);
}

