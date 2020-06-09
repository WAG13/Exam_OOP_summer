#pragma once

#include "Set.h"
#include <vector>
#include <utility>
#include <memory>

#include "../Lists/DoublyCircularLinkedList.h"
#include "../Lists/DoublyLinkedList.h"
#include "../Lists/ListNode.h"
#include "../Lists/ListTypes.h"

//////////////////////////
//Strategy pattern
//////////////////////////

template<typename T>
using SetListType = ListType<T, T>;


template<typename T>
using SetListTypeDouble = ListTypeDoubly<T, T>;


template<typename T>
using SetListTypeDoubleCircular = ListTypeDoublyCircular<T, T>;



template<typename T>
class ListSet : public Set<T>
{
public:
	ListSet(SetListType<T>* type);

	void insert(const T& element) override;
	void remove(const T& element) override;
	bool contains(const T& element) override;

	ForwardIterator<T> begin() override;
	ForwardIterator<T> end() override;

	//TODO: set operatrions (merge, diff, ...)

private:
	std::unique_ptr<DoublyLinkedListBase<T, T>> list;

	static T getKey(T kvp);
};

template<typename T>
ListSet<T>::ListSet(SetListType<T>* type)
	: list(type->createEmptyList(&getKey))
{}

template<typename T>
T ListSet<T>::getKey(T kvp)
{
	return kvp;
}

template<typename T>
ForwardIterator<T> ListSet<T>::begin()
{
	return list->begin();
}

template<typename T>
ForwardIterator<T> ListSet<T>::end()
{
	return list->end();
}

template<typename T>
void ListSet<T>::insert(const T& value)
{
	list->insertNewNode(value);
}

template<typename T>
void ListSet<T>::remove(const T& key)
{
	list->deleteByKey(key);
}

template<typename T>
bool ListSet<T>::contains(const T& key) 
{
	return list->searchByKey(key);
}

