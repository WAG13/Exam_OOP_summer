#pragma once

#include <iostream>
#include <string>
#include <functional>
#include "List.h"
#include "ListNode.h"

template <typename ValueT, typename KeyT>
class DoublyLinkedList : public List<DoublyListNode<ValueT, KeyT>, ValueT, KeyT> //linked list of DoublyListNode objects
{
public:
	DoublyLinkedList(KeyT(*myKeyGen)(ValueT) = NULL);
	~DoublyLinkedList() override;									
private:
	void prepend(ValueT) override;										//inserts new node before the first node in the list
	void prepend(ValueT, DoublyListNode<ValueT, KeyT>*) override;		//inserts new node before given node in the list
	void append(ValueT) override;										//inserts new node after the last node in the list
	void append(ValueT, DoublyListNode<ValueT, KeyT>*) override;		//inserts new node after given node in the list
	void deleteNode(DoublyListNode<ValueT, KeyT>*) override;			//inserts new node after given node in the list
};

template <typename ValueT, typename KeyT>
DoublyLinkedList<ValueT, KeyT>::DoublyLinkedList(KeyT(*myKeyGen)(ValueT)) : List(myKeyGen)
{
	list_head = nullptr;
	list_tail = nullptr;
}

template <typename ValueT, typename KeyT>
DoublyLinkedList<ValueT, KeyT>::~DoublyLinkedList()
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
void DoublyLinkedList<ValueT, KeyT>::prepend(ValueT dataIn)
{
	DoublyListNode<ValueT, KeyT>* new_node = new DoublyListNode<ValueT, KeyT>(dataIn);

	if (!list_head) {
		list_head = new_node;
		list_tail = new_node;
	}
	else {
		new_node->next = list_head;
	}
	list_head = new_node;
}

template <typename ValueT, typename KeyT>
void DoublyLinkedList<ValueT, KeyT>::prepend(ValueT dataIn, DoublyListNode<ValueT, KeyT>* node)
{
	if (!node) return;
	if (node == list_head) prepend(dataIn);
	else {
		DoublyListNode<ValueT, KeyT>* new_node = new DoublyListNode<ValueT, KeyT>(dataIn);
		
		new_node->prev = node->prev;
		node->prev = new_node;
		new_node->next = node;
		if (new_node->prev) 
			node->prev->next = new_node;
	}
}

template <typename ValueT, typename KeyT>
void DoublyLinkedList<ValueT, KeyT>::append(ValueT dataIn)
{
	DoublyListNode<ValueT, KeyT>* new_node = new DoublyListNode<ValueT, KeyT>(dataIn);

	if (!list_head) {
		list_head = new_node;
		list_tail = new_node;
	}
	else {
		list_tail->next = new_node;
		new_node->prev = list_tail;
		list_tail = new_node;
	}
}

template <typename ValueT, typename KeyT>
void DoublyLinkedList<ValueT, KeyT>::append(ValueT dataIn, DoublyListNode<ValueT, KeyT>* node)
{
	if (!node) return;
	if (node == list_tail) append(dataIn);
	else {
		DoublyListNode<ValueT, KeyT>* new_node = new DoublyListNode<ValueT, KeyT>(dataIn);
		new_node->next = node->next;
		node->next = new_node;
		new_node->prev = node;
		if (new_node->next)
			new_node->next->prev = new_node;
	}
}

/* DELETE */
template <typename ValueT, typename KeyT>
void DoublyLinkedList<ValueT, KeyT>::deleteNode(DoublyListNode<ValueT, KeyT>* node) {
	if (node) {
		if (node == list_head) {
			list_head = node->next;
			node->next->prev = nullptr;
		}
		else if (node == list_tail) {
			list_tail = node->prev;
			node->prev->next = nullptr;
		}
		else {
		if (node->next->prev)
			node->next->prev = node->prev;
		if (node->prev)
			node->prev->next = node->next;
		}
		delete node;
	}
}

