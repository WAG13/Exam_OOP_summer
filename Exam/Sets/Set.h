#ifndef SET_H
#define SET_H

#include <vector>
#include <memory>
#include <utility>
#include "../Iterator.h"


template<typename T>
class Set
{
public:
	virtual void insert(const T& element) = 0;
	virtual void remove(const T& element) = 0;
	virtual bool contains(const T& element) = 0;

	//TODO: set operatrions (merge, diff, ...)

	virtual ForwardIterator<T> begin() = 0;

};


#endif
