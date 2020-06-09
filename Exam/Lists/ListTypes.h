#pragma once

#include "DoublyLinkedListBase.h"
#include "DoublyLinkedList.h"
#include "DoublyCircularLinkedList.h"

template<typename ValueT, typename KeyT>
class ListType
{
public:
	virtual DoublyLinkedListBase<ValueT, KeyT>* createEmptyList(KeyT(*getKey)(ValueT)) = 0;
};


template<typename ValueT, typename KeyT>
class ListTypeDoubly : public ListType<ValueT, KeyT>
{
public:
	DoublyLinkedListBase<ValueT, KeyT>* createEmptyList(KeyT(*getKey)(ValueT val)) override
	{
		return new DoublyLinkedList<ValueT, KeyT>(getKey);
	}
};


template<typename ValueT, typename KeyT>
class ListTypeDoublyCircular : public ListType<ValueT, KeyT>
{
public:
	DoublyLinkedListBase<ValueT, KeyT>* createEmptyList(KeyT(*getKey)(ValueT val)) override
	{
		return new DoublyCircularLinkedList<ValueT, KeyT>(getKey);
	}
};