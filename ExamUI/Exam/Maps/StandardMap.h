#ifndef STANDARDMAP_H
#define STANDARDMAP_H

#include <vector>
#include <utility>
#include <map>
#include "Map.h"

template<typename ValueT, typename KeyT>
class StandardMap : public Map<ValueT, KeyT>
{
public:
	virtual ~StandardMap() {};

	ValueT get(const KeyT& key) const override;
	bool contains(const KeyT& key) const override;
	///
	/// @brief Adds new key-value pair OR sets new value for already existing key.
	///
	void set(const KeyT& key, ValueT value) override;

	void remove(const KeyT& key) override;

	std::vector<KeyT> getKeys() const override;
	std::vector<ValueT> getValues() const override;
	std::vector<std::pair<KeyT, ValueT>> getKVPs() const override;

private:
    std::map<KeyT, ValueT> map;
};

template<typename ValueT, typename KeyT>
ValueT StandardMap<ValueT, KeyT>::get(const KeyT& key) const
{
	return map.at(key);
}

template<typename ValueT, typename KeyT>
bool StandardMap<ValueT, KeyT>::contains(const KeyT& key) const
{
    return map.find(key) != map.end();
}

template<typename ValueT, typename KeyT>
void StandardMap<ValueT, KeyT>::set(const KeyT& key, ValueT value)
{
	map[key] = value;
}

template<typename ValueT, typename KeyT>
void StandardMap<ValueT, KeyT>::remove(const KeyT& key)
{
	map.erase(map.find(key));
}

template<typename ValueT, typename KeyT>
std::vector<KeyT> StandardMap<ValueT, KeyT>::getKeys() const
{
	std::vector<KeyT> keys;
	for (auto iter = map.cbegin(); iter != map.cend(); ++iter) {
		keys.push_back(iter->first);
	}
	return keys;
}

template<typename ValueT, typename KeyT>
std::vector<ValueT> StandardMap<ValueT, KeyT>::getValues() const
{
	std::vector<ValueT> values;
	for (auto iter = map.cbegin(); iter != map.cend(); ++iter) {
		values.push_back(iter->second);
	}
	return values;
}

template<typename ValueT, typename KeyT>
std::vector<std::pair<KeyT, ValueT>> StandardMap<ValueT, KeyT>::getKVPs() const
{
	std::vector<std::pair<KeyT, ValueT>> kvps;
	for (auto iter = map.cbegin(); iter != map.cend(); ++iter) {
		kvps.push_back(*iter);
	}
	return kvps;
}


#endif
