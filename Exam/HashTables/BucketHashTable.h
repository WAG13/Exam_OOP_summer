#pragma once

#include "HashTable.h"
#include <initializer_list>
#include <vector>

template <typename KeyT, typename ValueT, typename Hash, typename CollisionContainer>
class BucketHashTable : public HashTable<KeyT, ValueT, Hash> {
private:
	std::vector<CollisionContainer> buckets;
	size_t _size;
	Hash _second_hash;
	bool two_choice_enabled;

	size_t getIndex(KeyT const& key) {
		size_t index = Hash(key);
		if (two_choice_enabled) {
			size_t secondary_index = _second_hash(key);
			if (buckets[index].size() > buckets[secondary_index].size()) {
				index = secondary_index;
			}
		}
		return index;
	}

public:
	BucketHashTable() : _size(0) {}
	BucketHashTable(std::initializer_list<std::pair<KeyT, ValueT>> init) {
		for (auto const& item : init) {
			this->insert(item.first, item.second);
		}
		_size = init.size();
	}

	bool empty() const override {
		return buckets.empty();
	}

	size_t size() const override {
		return _size;
	}

	void clear() override {
		buckets.clear();
		_size = 0;
	}

	void insert(KeyT const& key, ValueT const& value) override {
		size_t index = this->getIndex(key);
		buckets[index].push_back({ key, value });
		_size++;
	}

	void remove(KeyT const& key, ValueT const& value) override {
		size_t index = Hash(key);
		for (auto iter = buckets[index].begin(); iter != buckets[index].end(); iter++) {
			if (iter->second == value) {
				buckets[index].erase(iter);
				_size--;
			}
		}

		if (two_choice_enabled) {
			size_t secondary_index = _second_hash(key);
			for (auto iter = buckets[secondary_index].begin(); iter != buckets[secondary_index].end(); iter++) {
				if (iter->second == value) {
					buckets[secondary_index].erase(iter);
					_size--;
				}
			}
		}
	}

	std::vector<ValueT> find(KeyT const& key) const {
		std::vector<ValueT> result;
		size_t index = Hash(key);
		for (auto const& item : buckets[index]) {
			result.push_back(item.second);
		}

		if (two_choice_enabled) {
			size_t secondary_index = _second_hash(key);
			if (index != secondary_index) {
				for (auto const& item : buckets[secondary_index]) {
					result.push_back(item.second);
				}
			}
		}

		return result;
	}

	void enableTwoChoiceHashing(Hash second_hash) {
		_second_hash = second_hash;
		two_choice_enabled = true;
	}

	void disableTwoChoiceHashing() {
		two_choice_enabled = false;
	}

	~BucketHashTable() {}
};