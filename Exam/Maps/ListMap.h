#ifndef LISTMAP_H
#define LISTMAP_H

#include <vector>
#include <utility>

#include "Map.h"
#include "../Lists/DoublyCircularLinkedList.h"
#include "../Lists/DoublyLinkedList.h"
#include "../Lists/ListNode.h"
#include "../Lists/ListTypes.h"

//////////////////////////
//Strategy pattern
//////////////////////////

template<typename ValueT, typename KeyT>
using MapListType = ListType<std::pair<KeyT, ValueT>, KeyT>;


template<typename ValueT, typename KeyT>
using MapListTypeDouble = ListTypeDoubly<std::pair<KeyT, ValueT>, KeyT>;


template<typename ValueT, typename KeyT>
using MapListTypeDoubleCircular = ListTypeDoublyCircular<std::pair<KeyT, ValueT>, KeyT>;



template<typename ValueT, typename KeyT>
class ListMap : public Map<ValueT, KeyT>
{
public:
	ListMap(MapListType<ValueT, KeyT>* type);

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
	std::unique_ptr<DoublyLinkedListBase<std::pair<KeyT, ValueT>, KeyT>> list;

	static KeyT getKey(std::pair<KeyT, ValueT> kvp);
};

template<typename ValueT, typename KeyT>
ListMap<ValueT, KeyT>::ListMap(MapListType<ValueT, KeyT>* type)
	: list(type->createEmptyList(&getKey))
{}

template<typename ValueT, typename KeyT>
KeyT ListMap<ValueT, KeyT>::getKey(std::pair<KeyT, ValueT> kvp)
{
	return kvp.first;
}



template<typename ValueT, typename KeyT>
ValueT ListMap<ValueT, KeyT>::get(const KeyT& key) const
{
	return list->searchByKey(key)->getData().second;
}

template<typename ValueT, typename KeyT>
bool ListMap<ValueT, KeyT>::contains(const KeyT& key) const
{
	return list->searchByKey(key);
}

template<typename ValueT, typename KeyT>
void ListMap<ValueT, KeyT>::set(const KeyT& key, ValueT value)
{
	DoublyListNode<std::pair<KeyT, ValueT>, KeyT>* node = list->searchByKey(key);
	if (!node) {
		list->insertNewNode(std::pair<KeyT, ValueT>(key, value));
		return;
	}
	std::pair<KeyT, ValueT> kvp = node->getData();
	kvp.second = value;
	node->setData(kvp);
}

template<typename ValueT, typename KeyT>
void ListMap<ValueT, KeyT>::remove(const KeyT& key)
{
	list->deleteByKey(key);
}

template<typename ValueT, typename KeyT>
std::vector<KeyT> ListMap<ValueT, KeyT>::getKeys() const
{
	std::vector<KeyT> keys;
	for (auto it = list->begin(); it != list->end(); ++it)
		keys.push_back(it->first);
	return keys;
}

template<typename ValueT, typename KeyT>
std::vector<ValueT> ListMap<ValueT, KeyT>::getValues() const
{
	std::vector<ValueT> values;
	for (auto it = list->begin(); it != list->end(); ++it)
		values.push_back(it->second);
	return values;
}

template<typename ValueT, typename KeyT>
std::vector<std::pair<KeyT, ValueT>> ListMap<ValueT, KeyT>::getKVPs() const
{
	std::vector<std::pair<KeyT, ValueT>> kvps;
	for (auto it = list->begin(); it != list->end(); ++it)
		kvps.push_back(*it);
	return kvps;
}

#endif
