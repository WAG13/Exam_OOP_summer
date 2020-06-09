#pragma once

#include <Diagnostics/Random.h>
#include <vector>

using std::vector;

template <typename T>
class PivotingStrategy {
public:
	virtual T getPivot(vector<T> const &sample, int left, int right) = 0;

	virtual ~PivotingStrategy() {}
};

template <typename T>
class MiddlePivot : public PivotingStrategy<T> {
public:
	T getPivot(vector<T> const &sample, int left, int right) override {
		return sample[(left + right) / 2];
	}

	~MiddlePivot() override {}
};

template<typename T>
class MedianOfThreePivot : public PivotingStrategy<T> {
public:
	T getPivot(vector<T> const &sample, int left, int right) override {
		T a = sample.front(), b = sample[(left + right) / 2], c = sample.back();
		if (a < b) {
			if (b < c) return b;
			else if (a < c) return c;
			else return a;
		}
		else {
			if (c < b) return b;
			else if (a < c) return a;
			else return c;
		}
	}

	~MedianOfThreePivot() override {}
};

template <typename T>
class RandomPivot : public PivotingStrategy<T> {
	T getPivot(vector<T> const &sample, int left, int right) override {
        return sample[Random::getRandomInt<size_t>(left, right)];
	}

	~RandomPivot() {}
};
