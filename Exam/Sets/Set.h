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

	/////////////////////////////////////
	//TEMPLATE METHOD PATTERN
	/////////////////////////////////////

	//Standard implementations using standard operations (insert, remove, contains, ForwardIterator)

	///
	/// @brief this set will contain the elements from this and the elements from otherSet
	///
	void insertUnion(Set* s1, Set* s2);
	///
	/// @brief this set will contain the elements that belong to this, but not to otherSet
	///
	void insertComplement(Set* s1, Set* s2);
	///
	/// @brief this set will contain the elements that belong to both this and otherSet
	///
	void insertIntersection(Set* s1, Set* s2);
	///
	/// @return this set will contain the elements from this and the elements from otherSet, except those that belong to both.
	///
	void insertSymmetricDiff(Set* s1, Set* s2);

	virtual ForwardIterator<T> begin() = 0;

private:
};

template<typename T>
void Set<T>::insertUnion(Set* s1, Set* s2)
{
	auto it1 = s1->begin();
	auto it2 = s2->begin();

	while (!it1.isEnd())
	{
		insert(*it1);
		++it1;
	}
	while (!it2.isEnd())
	{
		if (!s1->contains(*it2))
			insert(*it2);
		++it2;
	}
}

template<typename T>
void Set<T>::insertIntersection(Set* s1, Set* s2)
{
	auto it1 = s1->begin();
	auto it2 = s2->begin();

	while (!it1.isEnd())
	{
		if (s2->contains(*it1))
			insert(*it1);
		++it1;
	}
}

template<typename T>
void Set<T>::insertComplement(Set* s1, Set* s2)
{
	auto it1 = s1->begin();
	auto it2 = s2->begin();

	while (!it1.isEnd())
	{
		if (!s2->contains(*it1))
			insert(*it1);
		++it1;
	}
}

template<typename T>
void Set<T>::insertSymmetricDiff(Set* s1, Set* s2)
{
	auto it1 = s1->begin();
	auto it2 = s2->begin();

	while (!it1.isEnd())
	{
		if (!s2->contains(*it1))
			insert(*it1);
		++it1;
	}

	while (!it2.isEnd())
	{
		if (!s1->contains(*it2))
			insert(*it2);
		++it2;
	}
}

#endif
