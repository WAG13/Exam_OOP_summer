#pragma once

#include "../HashTables/HashTable.h"
#include "../HashTables/HashTypes.h"
#include "CapacityGrowPolicy.h"
#include "Set.h"
#include <functional>

template <typename T>
using SetHashType = HashType<T, T>;

template <typename T>
class HashSet : public Set<T> {
private:
	HashTable* table;
	CapacityGrowPolicy* grow_policy;

	size_t defaultHash(KeyT const& key, size_t index, size_t capacity) {
		return (std::hash<KeyT>{}(key) + index) % capacity;
	}

	struct HashSetIteratorImpl : public ForwardIteratorImpl<T>
	{
		std::vector<T> values;
		size_t currentIndex = 0;
		bool isNull = false;
		TreeSetIteratorImpl(HashTable* table)
		{
			if (!table) isNull = true;
			else {
				auto pairs = table->getAllItems();
				values.resize(pairs.size());
				for (size_t i = 0; i < pairs.size(); i++) {
					values[i] = pairs[i].first;
				}
			}
		}

		void increment() override
		{
			currentIndex++;
		}

		bool isEnd() const override
		{
			return isNull || currentIndex >= values.size();
		}

		T& getRef() override
		{
			return values[currentIndex];
		}

		T* getPtr() override
		{
			return &values[currentIndex];
		}

		const T& getRef() const override
		{
			return values[currentIndex];
		}

		const T* getPtr() const override
		{
			return &values[currentIndex];
		}
	};

public:
	HashSet(SetHashType<T>* hash_type, std::function<size_t(KeyT const&, size_t, size_t)> hash_func = defaultHash) :
		table(hash_type->create(hash_func, 1)), grow_policy(new ExponentialGrowPolicy{}) {}

	void insert(T const& element) override {
		if (!table->insert(element, element)) {
			if (!table->contains(element, element)) {
				table->rehash(grow_policy->grow() - table->capacity);
				table->insert(element, element);
			}
		}
	}

	void remove(const T& element) override {
		table->remove(element, element);
	}

	bool contains(const T& element) override {
		return table->contains(element, element);
	}

	~HashSet() {
		delete table;
		delete grow_policy;
	}
};