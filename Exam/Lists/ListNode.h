#pragma once

template <typename ValueT, typename KeyT>
class DoublyLinkedListBase;

template <typename ValueT, typename KeyT>
class DoublyLinkedList;

template <typename ValueT, typename KeyT>
class DoublyCircularLinkedList;

template <typename ValueT, typename KeyT>
class ListIteratorImpl;

template <typename ValueT, typename KeyT>
class Node
{
	friend class DoublyLinkedListBase<ValueT, KeyT>;
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
class DoublyListNode : public Node<ValueT, KeyT>
{
	friend class DoublyLinkedListBase<ValueT, KeyT>;
	friend class DoublyLinkedList<ValueT, KeyT>;
	friend class DoublyCircularLinkedList<ValueT, KeyT>;
	friend struct ListIteratorImpl<ValueT, KeyT>;
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


