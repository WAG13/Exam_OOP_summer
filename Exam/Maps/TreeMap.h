#ifndef TREEMAP_H
#define TREEMAP_H

#include <vector>
#include <utility>

#include "Map.h"
#include "../Trees/SearchTree.h"
#include "../Trees/AVLTree.h"
#include "../Trees/BPlusTree.h"
#include "../Trees/TreeTypes.h"

//Strategy pattern


template<typename ValueT, typename KeyT>
using MapTreeType = TreeType<std::pair<KeyT, ValueT>, KeyT>;


template<typename ValueT, typename KeyT>
using MapTreeTypeAVL = TreeTypeAVL<std::pair<KeyT, ValueT>, KeyT>;


template<typename ValueT, typename KeyT>
using MapTreeTypeBPlus = TreeTypeBPlus<std::pair<KeyT, ValueT>, KeyT>;




template<typename ValueT, typename KeyT>
class TreeMap : public Map<ValueT, KeyT>
{
public:
	TreeMap(MapTreeType<ValueT, KeyT>* type, lists::Comparator<KeyT> keyComparator);
	TreeMap(MapTreeType<ValueT, KeyT>* type);

	ValueT get(const KeyT& key) const override;
	bool contains(const KeyT& key) const override;
	///
	/// @brief Adds new key-value pair OR modifies sets new value for already existing key.
	///
	void set(const KeyT& key, ValueT value) override;

	void remove(const KeyT& key) override;

	std::vector<KeyT> getKeys() const override;
	std::vector<ValueT> getValues() const override;
	std::vector<std::pair<KeyT, ValueT>> getKVPs() const override;

private:
	std::unique_ptr<lists::SearchTree<std::pair<KeyT, ValueT>, KeyT>> tree;

	static KeyT getKey(const std::pair<KeyT, ValueT>& kvp);
};

template<typename ValueT, typename KeyT>
TreeMap<ValueT, KeyT>::TreeMap(MapTreeType<ValueT, KeyT>* type, lists::Comparator<KeyT> keyComparator)
	: tree(type->createEmptyTree(TreeMap<ValueT, KeyT>::getKey, keyComparator))
{}

template<typename ValueT, typename KeyT>
TreeMap<ValueT, KeyT>::TreeMap(MapTreeType<ValueT, KeyT>* type)
	: TreeMap(type, lists::detail::lessThan<KeyT>)
{}

template<typename ValueT, typename KeyT>
KeyT TreeMap<ValueT, KeyT>::getKey(const std::pair<KeyT, ValueT>& kvp)
{
	return kvp.first;
}



template<typename ValueT, typename KeyT>
ValueT TreeMap<ValueT, KeyT>::get(const KeyT& key) const
{
	return tree->get(key).second;
}

template<typename ValueT, typename KeyT>
bool TreeMap<ValueT, KeyT>::contains(const KeyT& key) const
{
	return tree->contains(key);
}

template<typename ValueT, typename KeyT>
void TreeMap<ValueT, KeyT>::set(const KeyT& key, ValueT value)
{
	if (!tree->contains(key)) {
		tree->add(std::pair<KeyT, ValueT>(key, value));
		return;
	}
	
	std::pair<KeyT, ValueT>& kvp = tree->get(key);
	kvp.second = value;
}

template<typename ValueT, typename KeyT>
void TreeMap<ValueT, KeyT>::remove(const KeyT& key)
{
	tree->remove(key);
}

template<typename ValueT, typename KeyT>
std::vector<KeyT> TreeMap<ValueT, KeyT>::getKeys() const
{
	std::vector<KeyT> keys;
	tree->forEach([&keys](const std::pair<KeyT, ValueT>& kvp) {
		keys.push_back(kvp.first);
	});
	return keys;
}

template<typename ValueT, typename KeyT>
std::vector<ValueT> TreeMap<ValueT, KeyT>::getValues() const
{
	std::vector<ValueT> values;
	tree->forEach([&values](const std::pair<KeyT, ValueT>& kvp) {
		values.push_back(kvp.second);
	});
	return values;
}

template<typename ValueT, typename KeyT>
std::vector<std::pair<KeyT, ValueT>> TreeMap<ValueT, KeyT>::getKVPs() const
{
	std::vector<std::pair<KeyT, ValueT>> kvps;
	tree->forEach([&kvps](const std::pair<KeyT, ValueT>& kvp) {
		kvps.push_back(kvp);
	});
	return kvps;
}

#endif
