#ifndef VECTORSET_H
#define VECTORSET_H

#include "Set.h"
#include <vector>
#include <utility>
#include <algorithm>

#include "../Iterator.h"



template<typename T>
class VectorSet : public Set<T>
{
public:
	VectorSet() {};

	void insert(const T& element) override;
	void remove(const T& element) override;
	bool contains(const T& element) override;

	ForwardIterator<T> begin() override;
	ForwardIterator<T> end() override;

	//TODO: set operatrions (merge, diff, ...)

private:
	std::vector<T> vector;

	struct VectorSetIteratorImpl : public ForwardIteratorImpl<T>
	{
		std::vector<T>* vectorPtr;
		size_t currentIndex = 0;
		VectorSetIteratorImpl(std::vector<T>* vectorPtr)
			: vectorPtr(vectorPtr)
		{}

		void increment() override
		{
			currentIndex++;
		}

		bool isEnd() const override
		{
			return !vectorPtr || currentIndex >= vectorPtr->size();
		}

		T& getRef() override
		{
			return (*vectorPtr)[currentIndex];
		}

		T* getPtr() override
		{
			return &(*vectorPtr)[currentIndex];
		}

		const T& getRef() const override
		{
			return (*vectorPtr)[currentIndex];
		}

		const T* getPtr() const override
		{
			return &(*vectorPtr)[currentIndex];
		}
	};
};

template<typename T>
ForwardIterator<T> VectorSet<T>::begin()
{
	return ForwardIterator(new VectorSet<T>::VectorSetIteratorImpl(&vector));
}

template<typename T>
ForwardIterator<T> VectorSet<T>::end()
{
	return ForwardIterator(new VectorSet<T>::VectorSetIteratorImpl(nullptr));
}

template<typename T>
void VectorSet<T>::insert(const T& element)
{
	vector.push_back(element);
}

template<typename T>
void VectorSet<T>::remove(const T& element)
{
	vector.erase(std::remove(vector.begin(), vector.end(), element), vector.end());
}

template<typename T>
bool VectorSet<T>::contains(const T& element)
{
	return std::find(vector.begin(), vector.end(), element) != vector.end();
}

#endif
