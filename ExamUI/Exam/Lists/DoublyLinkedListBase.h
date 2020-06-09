#pragma once

#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <functional>
#include "../Iterator.h"
#include "ListNode.h"


template<typename ValueT, typename KeyT>
struct ListIteratorImpl;


template <typename ValueT, typename KeyT>
class DoublyLinkedListBase //linked list of DoublyListNode objects
{
public:
	DoublyLinkedListBase(KeyT(*myKeyGen)(ValueT));
	virtual ~DoublyLinkedListBase();
	void insertNewNode(ValueT);									//insert new node in order in the list
	void show();
	DoublyListNode<ValueT, KeyT>* searchByValue(ValueT);		//searches for a value in the list and returns the point to object that contains that value
	DoublyListNode<ValueT, KeyT>* searchByKey(KeyT);			//searches for a value in the list and returns the point to object that contains that key
	void deleteByValue(ValueT);									//delete value in the list that contains that value
	void deleteByKey(KeyT);										//delete value in the list that contains that key
	std::vector<std::pair<KeyT, ValueT>> toVector() const;		//converts list to vector of pairs that contains that key and value

	ForwardIterator<ValueT> begin();
	ForwardIterator<ValueT> end();

protected:
	friend ListIteratorImpl<ValueT, KeyT>;

	DoublyListNode<ValueT, KeyT>* list_head;											//stores the pointer of first object in the list
	DoublyListNode<ValueT, KeyT>* list_tail;											//stored the pointer of the last object in the list
	KeyT(*keyGen)(ValueT);
	bool compare(ValueT data1, ValueT data2);

	DoublyListNode<ValueT, KeyT>* findNearest(ValueT dataIn);							//returns nearest smallest node witch value is < dataIn
	virtual void prepend(ValueT) = 0;							//inserts new node before the first node in the list
	virtual void prepend(ValueT, DoublyListNode<ValueT, KeyT>*) = 0;					//inserts new node before given node in the list
	virtual void append(ValueT) = 0;							//inserts new node after the last node in the list
	virtual void append(ValueT, DoublyListNode<ValueT, KeyT>*) = 0;					//inserts new node after given node in the list
	virtual void deleteNode(DoublyListNode<ValueT, KeyT>*) = 0;						//inserts new node after given node in the list
};

template<typename ValueT>
class DoublyLinkedListBaseSimple : public DoublyLinkedListBase<ValueT, ValueT>
{
	DoublyLinkedListBaseSimple()
		: DoublyLinkedListBase<ValueT, ValueT>(lists::detail::getValueAsKey<ValueT>)
	{}
};

template <typename ValueT, typename KeyT>
DoublyLinkedListBase<ValueT, KeyT>::DoublyLinkedListBase(KeyT(*myKeyGen)(ValueT))
{
	list_head = nullptr;
	list_tail = nullptr;
	keyGen = myKeyGen;
}

template <typename ValueT, typename KeyT>
DoublyLinkedListBase<ValueT, KeyT>::~DoublyLinkedListBase()
{
	if (!list_head)
	{
		DoublyListNode<ValueT, KeyT>* currentPtr = list_head;
		DoublyListNode<ValueT, KeyT>* tempPtr;

		while (currentPtr != 0)
		{
			tempPtr = currentPtr;
			currentPtr = currentPtr->next;
			delete tempPtr;
		}
	}
}

template <typename ValueT, typename KeyT>
bool DoublyLinkedListBase<ValueT, KeyT>::compare(ValueT data1, ValueT data2) {
	return (keyGen(data1) < keyGen(data2));
}

/* FIND */
template <typename ValueT, typename KeyT>
DoublyListNode<ValueT, KeyT>* DoublyLinkedListBase<ValueT, KeyT>::findNearest(ValueT dataIn)
{
	if (!list_head) return nullptr;
	else {
		DoublyListNode<ValueT, KeyT>* temp = list_head;
		if (compare(dataIn, temp->data)) return temp;
		while (temp->next && temp->next != list_head) {
			if (compare(dataIn, temp->next->data)) return temp;
			temp = temp->next;
		}
		return temp;
	}
}

template <typename ValueT, typename KeyT>
DoublyListNode<ValueT, KeyT>* DoublyLinkedListBase<ValueT, KeyT>::searchByValue(ValueT value) {
	if (list_head) {
		DoublyListNode<ValueT, KeyT>* temp = list_head;
		bool circle = true;
		while (temp && circle) {
			if (temp == list_tail) circle = false;
			if (temp->data == value) return temp;
			temp = temp->next;
		}
	}
	return nullptr;
}

template <typename ValueT, typename KeyT>
DoublyListNode<ValueT, KeyT>* DoublyLinkedListBase<ValueT, KeyT>::searchByKey(KeyT key) {
	if (list_head) {
		DoublyListNode<ValueT, KeyT>* temp = list_head;
		bool circle = true;
		while (temp && circle) {
			if (temp == list_tail) circle = false;
			if (keyGen(temp->data) == key) return temp;
			temp = temp->next;
		}
	}
	return nullptr;
}

/* ADD */
template <typename ValueT, typename KeyT>
void DoublyLinkedListBase<ValueT, KeyT>::insertNewNode(ValueT dataIn)
{
	if (!list_head) { prepend(dataIn); }
	else
	{
		if (compare(dataIn, list_head->data))
		{
			prepend(dataIn);
		}
		else if (!compare(dataIn, list_tail->data))
		{
			append(dataIn);
		}
		else
		{
			append(dataIn, findNearest(dataIn));
		}
	}
}

/* DELETE */
template <typename ValueT, typename KeyT>
void DoublyLinkedListBase<ValueT, KeyT>::deleteByValue(ValueT value) {
	deleteNode(searchByValue(value));
}

template <typename ValueT, typename KeyT>
void DoublyLinkedListBase<ValueT, KeyT>::deleteByKey(KeyT key) {
	deleteNode(searchByKey(key));
}

/* OUTPUT */
template <typename ValueT, typename KeyT>
void DoublyLinkedListBase<ValueT, KeyT>::show()
{
	if (!list_head)
	{
		std::cout << "The list is empty" << std::endl;
	}
	else
	{
		DoublyListNode<ValueT, KeyT>* current_node = list_head;

		bool circle = true;
		while (current_node && circle) {
			if (current_node == list_tail) circle = false;
			std::cout << "(" << current_node->data << " - " << keyGen(current_node->data) << ")  ";
			current_node = current_node->next;
		}
		std::cout << std::endl;
	}
}

template <typename ValueT, typename KeyT>
std::vector<std::pair<KeyT, ValueT>> DoublyLinkedListBase<ValueT, KeyT>::toVector() const {
	std::vector<std::pair<KeyT, ValueT>> vector_list;
	if (list_head) {
		DoublyListNode<KeyT, ValueT>* current_node = list_head;
		bool circle = true;
		while (current_node && circle) {
			if (current_node == list_tail) circle = false;
			KeyT key = current_node->getKeyByValue(keyGen);
			ValueT value = current_node->data;
			vector_list.push_back(std::make_pair(key, value));
			current_node = current_node->next;
		}
	}
	return vector_list;
}


/* ITERATOR */

template<typename ValueT, typename KeyT>
struct ListIteratorImpl : public ForwardIteratorImpl<ValueT>
{
	DoublyListNode<ValueT, KeyT>* node;
	DoublyLinkedListBase<ValueT, KeyT>* list;
	ListIteratorImpl(DoublyLinkedListBase<ValueT, KeyT>* list, DoublyListNode<ValueT, KeyT>* node)
		: node(node), list(list)
	{}

	void increment() override
	{
		if (node->next == list->list_head)
			node = nullptr;
		else
			node = node->next;
	}

	bool isEnd() const override
	{
		return !node;
	}

	ValueT& getRef() override
	{
		return node->data;
	}
	ValueT* getPtr() override
	{
		return &node->data;
	}
	const ValueT& getRef() const override
	{
		return node->data;
	}
	const ValueT* getPtr() const override
	{
		return &node->data;
	}
};


template<typename ValueT, typename KeyT>
ForwardIterator<ValueT> DoublyLinkedListBase<ValueT, KeyT>::begin()
{
	return ForwardIterator<ValueT>(new ListIteratorImpl<ValueT, KeyT>(this, this->list_head));
}

template<typename ValueT, typename KeyT>
ForwardIterator<ValueT> DoublyLinkedListBase<ValueT, KeyT>::end()
{
	return ForwardIterator<ValueT>(new ListIteratorImpl<ValueT, KeyT>(this, nullptr));
}
