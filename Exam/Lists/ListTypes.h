#pragma once

#include "LinkedList.h"
#include "List.h"
#include "DoublyLinkedList.h"
#include "DoublyCircularLinkedList.h"

template<typename NodeT, typename ValueT, typename KeyT>
class ListType
{
public:
	virtual DoublyLinkedListBase<NodeT, ValueT, KeyT>* createEmptyList(KeyT(*)(ValueT) getKey) = 0;
};


template<typename ValueT, typename KeyT>
class ListTypeSingle : public ListType<ValueT, KeyT>
{
public:
	DoublyLinkedListBase<ListNode<ValueT, KeyT>, ValueT, KeyT>* createEmptyTree(KeyT(*)(ValueT) getKey) override
	{
		return new LinkedList<ValueT, KeyT>(getKey);
	}
};


template<typename ValueT, typename KeyT>
class ListTypeDoubly : public ListType<ValueT, KeyT>
{
public:
	DoublyLinkedListBase<DoublyListNode<ValueT, KeyT>, ValueT, KeyT>* createEmptyTree(KeyT(*)(ValueT) getKey) override
	{
		return new DoublyLinkedList<ValueT, KeyT>(getKey);
	}
};


template<typename ValueT, typename KeyT>
class ListTypeDoublyCircular : public ListType<ValueT, KeyT>
{
public:
	DoublyLinkedListBase<DoublyListNode<ValueT, KeyT>, ValueT, KeyT>* createEmptyTree(KeyT(*)(ValueT) getKey) override
	{
		return new DoublyCircularLinkedList<ValueT, KeyT>(getKey);
	}
};