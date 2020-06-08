#pragma once

#include <vector>

template<typename KeyT>
using GetHashFuncType = size_t(*)(KeyT const&, size_t buckets_count);

template <typename KeyT, typename ValueT, GetHashFuncType<KeyT> Hash>
class HashTable {
public:
	virtual bool empty() const = 0;
	virtual size_t size() const = 0;
	virtual void clear() = 0;
	virtual bool insert(KeyT const& key, ValueT const& value) = 0;
	virtual bool remove(KeyT const& key, ValueT const& value) = 0;
	virtual bool remove(KeyT const& key) = 0;
	virtual bool contains(KeyT const& key, ValueT const& value) const = 0;
	virtual std::vector<ValueT> find(KeyT const& key) const = 0;
	virtual std::vector<std::pair<KeyT, ValueT>> getAllItems() const = 0;
	virtual void enableTwoChoiceHashing(GetHashFuncType<KeyT> second_hash) = 0;
	virtual void disableTwoChoiceHashing() = 0;
	virtual void rehash(size_t add_count) = 0;
};