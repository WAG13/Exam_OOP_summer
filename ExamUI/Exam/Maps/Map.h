#ifndef MAP_H
#define MAP_H

#include <vector>
#include <utility>

template<typename ValueT, typename KeyT>
class Map
{
public:
	virtual ~Map() {};

	virtual ValueT get(const KeyT& key) const = 0;
	virtual bool contains(const KeyT& key) const = 0;
	///
	/// @brief Adds new key-value pair OR sets new value for already existing key.
	///
	virtual void set(const KeyT& key, ValueT value) = 0;

	virtual void remove(const KeyT& key) = 0;

	virtual std::vector<KeyT> getKeys() const = 0;
	virtual std::vector<ValueT> getValues() const = 0;
	virtual std::vector<std::pair<KeyT, ValueT>> getKVPs() const = 0;
};


#endif
