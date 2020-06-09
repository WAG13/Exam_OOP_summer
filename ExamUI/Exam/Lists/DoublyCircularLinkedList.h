#pragma once

#include <iostream>
#include <string>
#include <functional>
#include "../Trees/SearchTree.h"
#include "DoublyLinkedListBase.h"
#include "ListNode.h"

template <typename ValueT, typename KeyT>
class DoublyCircularLinkedList : public DoublyLinkedListBase<ValueT, KeyT> //linked list of DoublyListNode objects
{
public:
	DoublyCircularLinkedList(KeyT(*myKeyGen)(ValueT));
	~DoublyCircularLinkedList() override;
private:
	void prepend(ValueT) override;										//inserts new node before the first node in the list
	void prepend(ValueT, DoublyListNode<ValueT, KeyT>*) override;		//inserts new node before given node in the list
	void append(ValueT) override;										//inserts new node after the last node in the list
	void append(ValueT, DoublyListNode<ValueT, KeyT>*) override;		//inserts new node after given node in the list
	void deleteNode(DoublyListNode<ValueT, KeyT>*) override;			//inserts new node after given node in the list
};

template<typename ValueT>
class DoublyCircularLinkedListSimple : public DoublyCircularLinkedList<ValueT, ValueT>
{
	DoublyCircularLinkedListSimple()
		: DoublyCircularLinkedList(lists::detail::getValueAsKey<ValueT>)
	{}
};

template <typename ValueT, typename KeyT>
DoublyCircularLinkedList<ValueT, KeyT>::DoublyCircularLinkedList(KeyT(*myKeyGen)(ValueT)) : DoublyLinkedListBase(myKeyGen)
{
	list_head = nullptr;
	list_tail = nullptr;
}

template <typename ValueT, typename KeyT>
DoublyCircularLinkedList<ValueT, KeyT>::~DoublyCircularLinkedList()
{
	if (!list_head)
	{
		DoublyListNode<ValueT, KeyT>* currentPtr = list_head;
		DoublyListNode<ValueT, KeyT>* tempPtr;

		while (currentPtr)
		{
			tempPtr = currentPtr;
			delete tempPtr;
			currentPtr = currentPtr->next;
		}
	}
}

/* ADD */
template <typename ValueT, typename KeyT>
void DoublyCircularLinkedList<ValueT, KeyT>::prepend(ValueT dataIn)
{
	DoublyListNode<ValueT, KeyT>* new_node = new DoublyListNode<ValueT, KeyT>(dataIn);

	if (!list_head) {
		new_node->next = new_node->prev = new_node;
		list_head = new_node;
	}
	else {
		new_node->next = list_head;
		new_node->prev = list_head->prev;
		list_tail->next = list_head->prev = new_node;
		list_head = new_node;
	}

	list_tail = list_head->prev;
}

template <typename ValueT, typename KeyT>
void DoublyCircularLinkedList<ValueT, KeyT>::prepend(ValueT dataIn, DoublyListNode<ValueT, KeyT>* node)
{
	if (!node) return;
	(node == list_head) ? prepend(dataIn) : append(dataIn, node->prev);
}

template <typename ValueT, typename KeyT>
void DoublyCircularLinkedList<ValueT, KeyT>::append(ValueT dataIn)
{
	DoublyListNode<ValueT, KeyT>* new_node = new DoublyListNode<ValueT, KeyT>(dataIn);

	if (!list_head) {
		new_node->next = new_node->prev = new_node;
		list_head = new_node;
	}
	else {
		new_node->next = list_head;
		list_head->prev = new_node;
		new_node->prev = list_tail;
		list_tail->next = new_node;
	}
	list_tail = list_head->prev;
}

template <typename ValueT, typename KeyT>
void DoublyCircularLinkedList<ValueT, KeyT>::append(ValueT dataIn, DoublyListNode<ValueT, KeyT>* node)
{
	if (!node) return;
	if (node == list_tail) append(dataIn);
	else {
		DoublyListNode<ValueT, KeyT>* new_node = new DoublyListNode<ValueT, KeyT>(dataIn);
		DoublyListNode<ValueT, KeyT>* next_node = node->next;

		node->next = new_node;
		new_node->prev = node;
		new_node->next = next_node;
		next_node->prev = new_node;
	}
	list_tail = list_head->prev;
}

/* DELETE */
template <typename ValueT, typename KeyT>
void DoublyCircularLinkedList<ValueT, KeyT>::deleteNode(DoublyListNode<ValueT, KeyT>* node) {
	if (node && list_head) {
		DoublyListNode<ValueT, KeyT>* prev_node = node->prev;

		if (node->next == node) {
			list_head = nullptr;
			list_tail = nullptr;
			return;
		}
		else if (node == list_head) {
			prev_node = list_head->prev;
			list_head = list_head->next;
			prev_node->next = list_head;
			list_head->prev = prev_node;
		}
		else if (node->next == list_head) {
			prev_node->next = list_head;
			list_head->prev = prev_node;
		}
		else {
			DoublyListNode<ValueT, KeyT>* temp = node->next;
			prev_node->next = temp;
			temp->prev = prev_node;
		}
		list_tail = list_head->prev;
		delete node;
	}
}



