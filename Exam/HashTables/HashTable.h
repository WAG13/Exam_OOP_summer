#pragma once

#include <vector>

template <typename KeyT, typename ValueT, typename Hash>
class HashTable {
public:
	virtual bool empty() const = 0;
	virtual size_t size() const = 0;
	virtual void clear() = 0;
	virtual void insert(KeyT const &key, ValueT const &value) = 0;
	virtual void remove(KeyT const& key, ValueT const& value) = 0;
	virtual std::vector<ValueT> find(KeyT const& key) const = 0;
	virtual void enableTwoChoiceHashing(Hash secondary_hash) = 0;
	virtual void disableTwoChoiceHashing() = 0;
};