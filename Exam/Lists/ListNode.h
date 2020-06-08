#pragma once

template <typename NodeT, typename ValueT, typename KeyT>
class List;

template <typename ValueT, typename KeyT>
class LinkedList;

template <typename ValueT, typename KeyT>
class DoublyLinkedList;

template <typename ValueT, typename KeyT>
class DoublyCircularLinkedList;

template <typename ValueT, typename KeyT>
class Node
{
	friend class List<Node, ValueT, KeyT>;
	friend class LinkedList<ValueT, KeyT>;
	friend class DoublyLinkedList<ValueT, KeyT>;
	friend class DoublyCircularLinkedList<ValueT, KeyT>;
public:
	Node(ValueT dataIn) {
		data = dataIn;
	}

	virtual ~Node() {};

	ValueT getData() const {
		return data;
	};

	void setData(ValueT dataIn) { data = dataIn; };

	KeyT getKeyByValue(KeyT(*convector)(ValueT)) const {
		return (*convector)(data);
	};

protected:
	ValueT data;
};

template <typename ValueT, typename KeyT>
class ListNode : public Node<ValueT, KeyT>
{
	friend class List<ListNode, ValueT, KeyT>;
	friend class LinkedList<ValueT, KeyT>;
public:
	ListNode(ValueT dataIn) : Node(dataIn){
		next = nullptr;
	}

	~ListNode() {};

private:
	ListNode* next; 
};

template <typename ValueT, typename KeyT>
class DoublyListNode : public Node<ValueT, KeyT>
{
	friend class List<DoublyListNode, ValueT, KeyT>;
	friend class DoublyLinkedList<ValueT, KeyT>;
	friend class DoublyCircularLinkedList<ValueT, KeyT>;
public:
	DoublyListNode(ValueT dataIn) : Node(dataIn) {
		next = nullptr;
		prev = nullptr;
	}

	~DoublyListNode() {};

private:
	DoublyListNode* next;
	DoublyListNode* prev;
};


