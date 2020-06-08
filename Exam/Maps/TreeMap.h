#ifndef TREEMAP_H
#define TREEMAP_H

#include <vector>
#include <utility>

#include "../Trees/SearchTree.h"
#include "../Trees/AVLTree.h"
#include "../Trees/BPlusTree.h"

using namespace lists;

//Strategy pattern

template<typename ValueT, typename KeyT>
class TreeType
{
public:
	virtual SearchTree<ValueT, KeyT>* createEmptyTree(GetKeyFunc<ValueT, KeyT> getKey, Comparator<KeyT> lessThan) = 0;
};



template<typename ValueT, typename KeyT>
class TreeTypeAVL : public TreeType
{
public:
	SearchTree<ValueT, KeyT>* createEmptyTree(GetKeyFunc<ValueT, KeyT> getKey, Comparator<KeyT> lessThan) override;
};

template<typename ValueT, typename KeyT>
SearchTree<ValueT, KeyT>* TreeTypeAVL<ValueT, KeyT>::createEmptyTree(GetKeyFunc<ValueT, KeyT> getKey, Comparator<KeyT> lessThan)
{
	return new AVLTree(getKey, lessThan);
}



template<typename ValueT, typename KeyT>
class TreeTypeBPlus : public TreeType
{
public:
	TreeTypeBPlus(size_t t);
	SearchTree<ValueT, KeyT>* createEmptyTree(GetKeyFunc<ValueT, KeyT> getKey, Comparator<KeyT> lessThan) override;

private:
	size_t t;
};

template<typename ValueT, typename KeyT>
TreeTypeBPlus<ValueT, KeyT>::TreeTypeBPlus(size_t t)
	: t(t)
{
}

template<typename ValueT, typename KeyT>
SearchTree<ValueT, KeyT>* TreeTypeBPlus<ValueT, KeyT>::createEmptyTree(GetKeyFunc<ValueT, KeyT> getKey, Comparator<KeyT> lessThan)
{
	return new BPlusTree(getKey, lessThan, this->t);
}




template<typename ValueT, typename KeyT>
class TreeMap : Map<ValueT, KeyT>
{
public:
	TreeMap(TreeType<std::pair<KeyT, ValueT>, KeyT>* type, Comparator<KeyT> keyComparator);
	TreeMap(TreeType<std::pair<KeyT, ValueT>, KeyT>* type);

	ValueT get(const KeyT& key) const override;
	///
	/// @brief Adds new key-value pair OR modifies sets new value for already existing key.
	///
	void set(const KeyT& key, ValueT value) override;

	void remove(const KeyT& key) override;

	std::vector<KeyT> getKeys() const override;
	std::vector<ValueT> getValues() const override;
	std::vector<std::pair<ValueT, KeyT>> getKVPs() const override;

private:
	SearchTree<std::pair<KeyT, ValueT>, KeyT>* tree;

	static KeyT getKey(const std::pair<KeyT, ValueT>& kvp);
}

template<typename ValueT, typename KeyT>
TreeMap<ValueT, KeyT>::TreeMap(TreeType<std::pair<KeyT, ValueT>, KeyT>* type, Comparator<KeyT> keyComparator)
{
	tree = type->createEmptyTree(TreeMap<ValueT, KeyT>::getKey, comparator);
}

template<typename ValueT, typename KeyT>
TreeMap<ValueT, KeyT>::TreeMap(TreeType<std::pair<KeyT, ValueT>, KeyT>* type)
	: TreeMap(type, details::lessThan<KeyT>)
{}

template<typename ValueT, typename KeyT>
TreeMap<ValueT, KeyT>::getKey(const std::pair<KeyT, ValueT>& kvp)
{
	return kvp.first;
}



template<typename ValueT, typename KeyT>
ValueT TreeMap<ValueT, KeyT>::get(const KeyT& key) const
{
	return tree->get(key);
}

template<typename ValueT, typename KeyT>
void TreeMap<ValueT, KeyT>::set(const KeyT& key, ValueT value)
{
	std::pair<KeyT, ValueT>& kvp = tree->get(key);
	kvp.second = value;
}

template<typename ValueT, typename KeyT>
void TreeMap<ValueT, KeyT>::remove(const KeyT& key)
{
	tree->remove(key);
}

#endif
