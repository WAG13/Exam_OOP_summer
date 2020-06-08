#pragma once

#include "../Trees/SearchTree.h"
#include "../Trees/AVLTree.h"
#include "../Trees/BPlusTree.h"

template<typename ValueT, typename KeyT>
class TreeType
{
public:
	virtual lists::SearchTree<ValueT, KeyT>* createEmptyTree(lists::GetKeyFunc<ValueT, KeyT> getKey, lists::Comparator<KeyT> lessThan) = 0;
};


template<typename ValueT, typename KeyT>
class TreeTypeAVL : public TreeType<ValueT, KeyT>
{
public:
	lists::SearchTree<ValueT, KeyT>* createEmptyTree(lists::GetKeyFunc<ValueT, KeyT> getKey, lists::Comparator<KeyT> lessThan) override
	{
		return new lists::AVLTree(getKey, lessThan);
	}
};


template<typename ValueT, typename KeyT>
class TreeTypeBPlus : public TreeType<ValueT, KeyT>
{
public:
	TreeTypeBPlus(size_t t) : t(t)
	{
	}
	lists::SearchTree<ValueT, KeyT>* createEmptyTree(lists::GetKeyFunc<ValueT, KeyT> getKey, lists::Comparator<KeyT> lessThan) override
	{
		return new lists::BPlusTree(getKey, lessThan, this->t);
	}

private:
	size_t t;
};