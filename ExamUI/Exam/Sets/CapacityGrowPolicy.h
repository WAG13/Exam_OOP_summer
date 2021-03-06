#pragma once

#include <math.h>

class CapacityGrowPolicy {
public:
	virtual size_t grow() = 0;
};

class ExponentialGrowPolicy : public CapacityGrowPolicy {
private:
	size_t power = 1;

public:
	size_t grow() override {
		power++;
		return static_cast<size_t>(pow(2, power) + 1);
	}
};