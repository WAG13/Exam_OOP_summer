#ifndef TREESET_H
#define TREESET_H

#include "Set.h"
#include <vector>
#include <utility>
#include <memory>

#include "../Trees/SearchTree.h"
#include "../Trees/TreeTypes.h"
#include "../Iterator.h"

//Strategy pattern


template<typename T>
using SetTreeType = TreeType<T, T>;


template<typename T>
using SetTreeTypeAVL = TreeTypeAVL<T, T>;


template<typename T>
using SetTreeTypeBPlus = TreeTypeBPlus<T, T>;




template<typename T>
class TreeSet : public Set<T>
{
public:
	TreeSet(SetTreeType<T>* type);

	void insert(const T& element) override;
	void remove(const T& element) override;
	bool contains(const T& element) override;

	ForwardIterator<T> begin() override;
	ForwardIterator<T> end() override;

	//TODO: set operatrions (merge, diff, ...)

private:
	std::unique_ptr<lists::SearchTreeSimple<T>> tree;

	struct TreeSetIteratorImpl : public ForwardIteratorImpl<T>
	{
		//extremely hacky but quick implementation
		std::vector<T> values;
		size_t currentIndex = 0;
		bool isNull = false;
		TreeSetIteratorImpl(lists::SearchTreeSimple<T>* tree)
		{
			if (!tree) {
				isNull = true;
			}
			else {
				tree->forEach([&](const T& value) {
					values.push_back(value);
				});
			}
		}

		void increment() override
		{
			currentIndex++;
		}

		bool isEnd() const override
		{
			return isNull || currentIndex >= values.size();
		}

		T& getRef() override
		{
			return values[currentIndex];
		}
		T* getPtr() override
		{
			return &values[currentIndex];
		}
		const T& getRef() const override
		{
			return values[currentIndex];
		}
		const T* getPtr() const override
		{
			return &values[currentIndex];
		}
	};
};

template<typename T>
TreeSet<T>::TreeSet(SetTreeType<T>* type)
	: tree(type->createEmptyTree(lists::detail::getValueAsKey<T>, lists::detail::lessThan<T>))
{}

template<typename T>
ForwardIterator<T> TreeSet<T>::begin()
{
	return ForwardIterator<T>(new TreeSet<T>::TreeSetIteratorImpl(tree.get()));
}

template<typename T>
ForwardIterator<T> TreeSet<T>::end()
{
	return ForwardIterator<T>(new TreeSet<T>::TreeSetIteratorImpl(nullptr));
}

template<typename T>
void TreeSet<T>::insert(const T& element)
{
	tree->add(element);
}

template<typename T>
void TreeSet<T>::remove(const T& element)
{
	tree->remove(element);
}

template<typename T>
bool TreeSet<T>::contains(const T& element)
{
	return tree->contains(element);
}

#endif
