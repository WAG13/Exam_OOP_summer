#pragma once

#include "Random.h"
#include "DateTime.h"
#include <vector>
#include <algorithm>

using namespace std;

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

