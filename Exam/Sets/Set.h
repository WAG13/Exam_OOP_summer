#ifndef SET_H
#define SET_H

#include <vector>
#include <utility>

template<typename T>
class Set
{
public:
	virtual void insert(const T& element) = 0;
	virtual void remove(const T& element) = 0;
	virtual bool contains(const T& element) const;

	//TODO: set operatrions (merge, diff, ...)
}


#endif
