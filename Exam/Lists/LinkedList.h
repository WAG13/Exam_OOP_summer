#pragma once

#include <iostream>
#include <string>
#include <functional>
#include "List.h"
#include "ListNode.h"

template <typename ValueT, typename KeyT>
class LinkedList : public List<ListNode<ValueT, KeyT>, ValueT, KeyT> //linked list of ListNode objects
{
public:
	LinkedList(KeyT(*myKeyGen)(ValueT) = NULL);
	~LinkedList() override;							
private:
	void prepend(ValueT) override;								//inserts new node before the first node in the list
	void prepend(ValueT, ListNode<ValueT, KeyT>*) override;		//inserts new node before given node in the list
	void append(ValueT) override;								//inserts new node after the last node in the list
	void append(ValueT, ListNode<ValueT, KeyT>*) override;		//inserts new node after given node in the list
	void deleteNode(ListNode<ValueT, KeyT>*) override;			//inserts new node after given node in the list
};

template <typename ValueT, typename KeyT>
LinkedList<ValueT, KeyT>::LinkedList(KeyT (*myKeyGen)(ValueT)) : List(myKeyGen)
{
	list_head = nullptr;
	list_tail = nullptr;
}

template <typename ValueT, typename KeyT>
LinkedList<ValueT, KeyT>::~LinkedList()
{
	if (!list_head) 
	{
		ListNode<ValueT, KeyT>* currentPtr = list_head;
		ListNode<ValueT, KeyT>* tempPtr;

		while (currentPtr != 0) 
		{
			tempPtr = currentPtr;
			currentPtr = currentPtr->next;
			delete tempPtr;
		}
	}
}

/* ADD */
template <typename ValueT, typename KeyT>
void LinkedList<ValueT, KeyT>::prepend(ValueT dataIn)
{
	ListNode<ValueT, KeyT>* new_node = new ListNode<ValueT, KeyT>(dataIn);

	if (!list_head) {
		list_head = new_node;
		list_tail = new_node;
	}
	else {
		new_node->next = list_head; 
		list_head = new_node; 
	}
}

template <typename ValueT, typename KeyT>
void LinkedList<ValueT, KeyT>::prepend(ValueT dataIn, ListNode<ValueT, KeyT>* node)
{
	if (node == list_head) prepend(dataIn);
	else {
		ListNode<ValueT, KeyT>* new_node = new ListNode<ValueT, KeyT>(dataIn);
		new_node->next = node;

		ListNode<ValueT, KeyT>* temp = list_head;
		while (temp->next != node) temp = temp->next;
		temp->next = new_node;
	}
}

template <typename ValueT, typename KeyT>
void LinkedList<ValueT, KeyT>::append(ValueT dataIn)
{
	ListNode<ValueT, KeyT>* new_node = new ListNode<ValueT, KeyT>(dataIn);

	if (!list_head) {
		list_head = new_node;
		list_tail = new_node;
	}
	else {
		list_tail->next = new_node;
		list_tail = new_node;
	}
}

template <typename ValueT, typename KeyT>
void LinkedList<ValueT, KeyT>::append(ValueT dataIn, ListNode<ValueT, KeyT>* node)
{
	if (node == list_tail) append(dataIn);
	else {
		ListNode<ValueT, KeyT>* new_node = new ListNode<ValueT, KeyT>(dataIn);
		new_node->next = node->next;
		node->next = new_node;
	}
}

/* DELETE */
template <typename ValueT, typename KeyT>
void LinkedList<ValueT, KeyT>::deleteNode(ListNode<ValueT, KeyT>* node) {
	if (node) {
		if (node == list_head) {
			list_head = node->next;
		}
		
		else {
			ListNode<ValueT, KeyT>* temp = list_head;
			while (temp->next != node) temp = temp->next;
			temp->next = node->next;
			node->next = nullptr;
			if (node == list_tail) list_tail = temp;
		}
		delete node;
	}
}


