#ifndef TREESET_H
#define TREESET_H

#include "Set.h"
#include <vector>
#include <utility>

template<typename T>
class TreeSet : public Set<T>
{
public:
	void insert(const T& element) override;
	void remove(const T& element) override;
	bool contains(const T& element) override;

	Iterator begin() override;

	//TODO: set operatrions (merge, diff, ...)

private:
	class TreeSetIteratorImpl : public IteratorImpl
	{};
};


#endif
