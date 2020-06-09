#pragma once

#include "HashTable.h"
#include "BucketHashTable.h"
#include "CoalescedHashTable.h"

template <typename KeyT, typename ValueT>
class HashType {
public:
	virtual HashTable<KeyT, ValueT>* create(std::function<size_t(KeyT const&, size_t, size_t)> _hash, size_t capacity = 1) = 0;
};

template <typename KeyT, typename ValueT, typename CollisionContainer>
class SeparateHash : public HashType<KeyT, ValueT> {
public:
	HashTable<KeyT, ValueT>* create(std::function<size_t(KeyT const&, size_t, size_t)> _hash, size_t capacity = 1) override {
		return new BucketHashTable<KeyT, ValueT, CollisionContainer>{ _hash, capacity };
	}
};

template <typename KeyT, typename ValueT>
class CoalescedHash : public HashType<KeyT, ValueT> {
public:
	HashTable<KeyT, ValueT>* create(std::function<size_t(KeyT const&, size_t, size_t)> _hash, size_t capacity = 1) override {
		return new CoalescedHashTable<KeyT, ValueT>{ _hash, capacity };
	}
};