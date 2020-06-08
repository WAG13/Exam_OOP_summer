#pragma once

#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <functional>

template <typename NodeT, typename ValueT, typename KeyT>
class List //linked list of NodeT objects
{
public:
	List(KeyT(*myKeyGen)(ValueT) = NULL);
	virtual ~List();
	void insertNewNode(ValueT);									//insert new node in order in the list
	void show();									
	NodeT* searchByValue(ValueT);								//searches for a value in the list and returns the point to object that contains that value
	NodeT* searchByKey(KeyT);									//searches for a value in the list and returns the point to object that contains that key
	void deleteByValue(ValueT);									//delete value in the list that contains that value
	void deleteByKey(KeyT);										//delete value in the list that contains that key
	std::vector<std::pair<ValueT, KeyT>> toVector() const;		//converts list to vector of pairs that contains that key and value
protected:
	NodeT* list_head;											//stores the pointer of first object in the list
	NodeT* list_tail;											//stored the pointer of the last object in the list
	KeyT(*keyGen)(ValueT);
	bool compare(ValueT data1, ValueT data2);
	
	NodeT* findNearest(ValueT dataIn);							//returns nearest smallest node witch value is < dataIn
	virtual void prepend(ValueT) = 0;							//inserts new node before the first node in the list
	virtual void prepend(ValueT, NodeT*) = 0;					//inserts new node before given node in the list
	virtual void append(ValueT) = 0;							//inserts new node after the last node in the list
	virtual void append(ValueT, NodeT*) = 0;					//inserts new node after given node in the list
	virtual void deleteNode(NodeT*) = 0;						//inserts new node after given node in the list
};
/* function for getting keys */
template <typename ValueT, typename KeyT>
KeyT keyGenDefault(ValueT data) {
	return data;
}

template <typename NodeT, typename ValueT, typename KeyT>
List<NodeT, ValueT, KeyT>::List(KeyT (*myKeyGen)(ValueT))
{
	list_head = nullptr;
	list_tail = nullptr;
	(myKeyGen)
		? keyGen = myKeyGen
		: keyGen = keyGenDefault<ValueT, KeyT>;
}

template <typename NodeT, typename ValueT, typename KeyT>
List<NodeT, ValueT, KeyT>::~List()
{
	if (!list_head) 
	{
		NodeT* currentPtr = list_head;
		NodeT* tempPtr;

		while (currentPtr != 0) 
		{
			tempPtr = currentPtr;
			currentPtr = currentPtr->next;
			delete tempPtr;
		}
	}
}

template <typename NodeT, typename ValueT, typename KeyT>
bool List<NodeT, ValueT, KeyT>::compare(ValueT data1, ValueT data2) {
	return (keyGen(data1) < keyGen(data2));
}

/* FIND */
template <typename NodeT, typename ValueT, typename KeyT>
NodeT* List<NodeT, ValueT, KeyT >::findNearest(ValueT dataIn)
{
	if (!list_head) return nullptr;
	else {
		NodeT* temp = list_head;
		if (compare(dataIn,temp->data)) return temp;
		while (temp->next || temp->next != list_head) {
			if (compare(dataIn, temp->next->data)) return temp;
			temp = temp->next;
		}
		return temp;
	}
}

template <typename NodeT, typename ValueT, typename KeyT>
NodeT* List<NodeT, ValueT, KeyT>::searchByValue(ValueT value) {
	if (list_head) {
		NodeT* temp = list_head;
		while (temp != list_tail->next) {
			if (temp->data == value) return temp;
			temp = temp->next;
		}
	}
	return nullptr;
}

template <typename NodeT, typename ValueT, typename KeyT>
NodeT* List<NodeT, ValueT, KeyT>::searchByKey(KeyT value) {
	if (list_head) {
		NodeT* temp = list_head;
		while (temp != list_tail->next) {
			if (temp->data == value) return temp;
			temp = temp->next;
		}
	}
	return nullptr;
}

/* ADD */
template <typename NodeT, typename ValueT, typename KeyT>
void List<NodeT, ValueT, KeyT>::insertNewNode(ValueT dataIn)
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
template <typename NodeT, typename ValueT, typename KeyT>
void List<NodeT, ValueT, KeyT>::deleteByValue(ValueT value) {
	deleteNode(searchByValue(value));
}

template <typename NodeT, typename ValueT, typename KeyT>
void List<NodeT, ValueT, KeyT>::deleteByKey(KeyT key) {
	deleteNode(searchByKey(key));
}

/* OUTPUT */
template <typename NodeT, typename ValueT, typename KeyT>
void List<NodeT, ValueT, KeyT>::show()
{
	if (!list_head)
	{
		std::cout << "The list is empty" << std::endl;
	}
	else
	{
		NodeT* current_node = list_head;

		while (current_node != list_tail->next)
		{
			std::cout << "(" << current_node->data <<" - "<< keyGen(current_node->data) << ")  ";
			current_node = current_node->next; 
		}
		std::cout << std::endl;
	}
}

template <typename NodeT, typename ValueT, typename KeyT>
std::vector<std::pair<ValueT, KeyT>> List<NodeT, ValueT, KeyT>::toVector() const {
	std::vector<std::pair<ValueT, KeyT>> vector_list;
	if (list_head){
		NodeT* current_node = list_head;

		while (current_node != list_tail->next){
			ValueT value = current_node->data;
			KeyT key = current_node->getKeyByValue(keyGen);
			vector_list.push_back(std::make_pair(value, key));
			current_node = current_node->next;
		}
	}
	return vector_list;
}